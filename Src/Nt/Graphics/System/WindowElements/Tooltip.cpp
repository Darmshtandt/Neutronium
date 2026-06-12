#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOMSG
#undef NOWINSTYLES

#include <windows.h>

#include <Nt/Graphics/System/WindowElements/Tooltip.h>

namespace Nt {
	Tooltip::Info::Info(const TTTOOLINFO& info) noexcept :
		Text(info.lpszText),
		Rect(info.rect),
		Flags(DisplayFlags(info.uFlags)),
		ParentHandle(info.hwnd),
		ID(info.uId),
		Param(info.lParam),
		Reserved(info.lpReserved) 
	{
	}

	TTTOOLINFO Tooltip::Info::ToWinApiStruct() const noexcept {
		TTTOOLINFO info;
		info.cbSize = sizeof(info);
		info.uFlags = Flags;
		info.hwnd = ParentHandle;
		info.uId = ID;
		info.rect = Rect;
		info.hinst = GetModuleHandle(nullptr);
		info.lpszText = const_cast<wChar*>(Text.c_str());
		info.lParam = Param;
		info.lpReserved = Reserved;
		return info;
	}

	Tooltip::HitTestInfo::HitTestInfo(const TTHITTESTINFO& info) noexcept :
		ParentHandle(info.hwnd),
		Point(info.pt),
		ToolInfo(info.ti) {
	}

	TTHITTESTINFO Tooltip::HitTestInfo::ToWinApiStruct() const noexcept {
		TTHITTESTINFO hitInfo;
		hitInfo.hwnd = ParentHandle;
		hitInfo.pt = Point;
		hitInfo.ti = ToolInfo.ToWinApiStruct();
		return hitInfo;
	}

	Tooltip::Title::Title(const TTGETTITLE& title) noexcept :
		BitmapID(title.uTitleBitmap),
		TextLength(title.cch),
		pText(title.pszTitle) {
	}

	TTGETTITLE Tooltip::Title::ToWinApiStruct() const noexcept {
		TTGETTITLE title;
		title.dwSize = sizeof(title);
		title.uTitleBitmap = BitmapID;
		title.cch = TextLength;
		title.pszTitle = pText;
		return title;
	}

	Tooltip::Tooltip(const HWND& hwnd) : HandleWindow(hwnd) 
	{
	}

	void Tooltip::Create() {
		Create(IntRect(), "");
	}

	void Tooltip::Create([[maybe_unused]] const IntRect& windowRect, [[maybe_unused]] const String& name) {
		m_Name = L"";
		m_ClassName = TOOLTIPS_CLASS;
		m_WindowRect = IntRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
		m_Styles |= WS_POPUP;

		_CreateWindow();
	}

	void Tooltip::AddTool(const HandleWindow& parent, const DisplayFlags& flags, const uInt& id, const String& text) {
		if (!parent.IsCreated())
			Raise("The parent window passed not created");

		Info info = { };
		info.Flags = flags;
		info.ID = id;
		info.Text = text;
		info.ParentHandle = parent.GetHandle();

		TTTOOLINFO winApiStruct = info.ToWinApiStruct();
		_SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<Long>(&winApiStruct));
	}

	void Tooltip::RemoveTool(const HandleWindow& parent, const uInt& ID) {
		if (!parent.IsCreated())
			Raise("The parent window passed not created");

		TTTOOLINFO winApiStruct;
		winApiStruct.uId = ID;
		winApiStruct.hwnd = parent.GetHandle();
		_SendMessage(TTM_DELTOOL, 0, reinterpret_cast<Long>(&winApiStruct));
	}

	Bool Tooltip::AdjustRect(const Bool& isTextRect, IntRect* pRect) const {
		RequireNotNull(pRect);

		RECT adjustableRect;
		const Bool result = _SendMessage(TTM_ADJUSTRECT, isTextRect,
			reinterpret_cast<Long>(&adjustableRect));
		if (result)
			*pRect = adjustableRect;
		return result;
	}

	void Tooltip::Update() {
		_SendMessage(TTM_UPDATE, 0, 0);
	}

	void Tooltip::Hide() {
		_SendMessage(TTM_POP, 0, 0);
	}

	void Tooltip::Show() {
		_SendMessage(TTM_POPUP, 0, 0);
	}

	Nt::HandleWindow Tooltip::ShowFromPoint(const Int2D& position) {
		POINT point = position;
		const Long lParam = reinterpret_cast<Long>(&point);
		const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(TTM_WINDOWFROMPOINT, 0, lParam));
		return HandleWindow(hwnd);
	}

	Bool Tooltip::ToggleActivate(const Bool& isActive) {
		return _SendMessage(TTM_ACTIVATE, isActive, 0);
	}

	void Tooltip::ToggleTrackActivate(const HandleWindow& parent, const uInt& id, const Bool& isActive) {
		if (!parent.IsCreated())
			Raise("The parent window passed not created");

		TTTOOLINFO info = { };
		info.cbSize = sizeof(info);
		info.hwnd = parent.GetHandle();
		info.uId = id;
		_SendMessage(TTM_TRACKACTIVATE, isActive, reinterpret_cast<Long>(&info));
	}

	void Tooltip::TrackPosition(const Int2D& position) {
		_SendMessage(TTM_TRACKPOSITION, 0, MAKELONG(position.x, position.y));
	}

	Bool Tooltip::HitTest(HitTestInfo* pInfo) const {
		TTHITTESTINFO info = RequireNotNull(pInfo)->ToWinApiStruct();

		const Bool result = _SendMessage(TTM_HITTEST, 0, 0);
		if (result)
			(*pInfo) = info;

		return result;
	}

	Nt::Tooltip::Info Tooltip::EnumTools(const uInt& parentID) {
		wChar buffer[80];

		TOOLINFO info = { };
		info.cbSize = sizeof(info);
		info.lpszText = buffer;
		_SendMessage(TTM_ENUMTOOLS, parentID, reinterpret_cast<Long>(&info));
		return info;
	}

	Nt::Int2D Tooltip::GeSize(Info* pInfo) const {
		TOOLINFO toolInfo = RequireNotNull(pInfo)->ToWinApiStruct();

		const Long size = _SendMessage(TTM_GETBUBBLESIZE, 0, reinterpret_cast<Long>(&toolInfo));
		(*pInfo) = toolInfo;

		return Int2D(LOWORD(size), HIWORD(size));
	}

	Bool Tooltip::GetCurrentTool(Info* pInfo) const {
		TOOLINFO toolInfo = RequireNotNull(pInfo)->ToWinApiStruct();

		const Bool result = _SendMessage(TTM_GETCURRENTTOOL, 0, reinterpret_cast<Long>(&toolInfo));
		if (result)
			(*pInfo) = toolInfo;

		return result;
	}

	Int Tooltip::GetDelayTime(const DelayTimeFlags& flags) const {
		return _SendMessage(TTM_GETDELAYTIME, flags, 0);
	}

	Nt::IntRect Tooltip::GetMargin() const {
		RECT rect;
		_SendMessage(TTM_GETMARGIN, 0, reinterpret_cast<Long>(&rect));
		return rect;
	}

	Int Tooltip::GetMaxTipWidth() const {
		return _SendMessage(TTM_GETMAXTIPWIDTH, 0, 0);
	}

	Nt::String Tooltip::GetText(const HandleWindow& parent, const uInt& id) const {
		if (!parent.IsCreated())
			Raise("The parent window passed not created");

		TOOLINFO info = { };
		info.cbSize = sizeof(info);
		info.hwnd = parent.GetHandle();
		info.uId = id;
		_SendMessage(TTM_GETTEXT, 4096, reinterpret_cast<Long>(&info));

		return info.lpszText;
	}

	Nt::Byte3D Tooltip::GetTipBackgroundColor() const {
		return ColorRefToVector(_SendMessage(TTM_GETTIPBKCOLOR, 0, 0));
	}

	Nt::Byte3D Tooltip::GetTipTextColor() const {
		return ColorRefToVector(_SendMessage(TTM_GETTIPTEXTCOLOR, 0, 0));
	}

	Nt::Tooltip::Title Tooltip::GetTitle() const {
		TTGETTITLE title = { };
		title.dwSize = sizeof(title);
		_SendMessage(TTM_GETTITLE, 0, reinterpret_cast<Long>(&title));
		return title;
	}

	uInt Tooltip::GetToolCount() const {
		return _SendMessage(TTM_GETTOOLCOUNT, 0, 0);
	}

	Nt::Tooltip::Info Tooltip::GetToolInfo() const {
		TTTOOLINFO info;
		info.cbSize = sizeof(info);
		if (!_SendMessage(TTM_GETTOOLINFO, 0, reinterpret_cast<Long>(&info)))
			Raise("Failed to get tool info");

		return info;
	}

	Bool Tooltip::SetToolRect(const HandleWindow& parent, const uInt& id, const IntRect& newRect) {
		if (!parent.IsCreated())
			Raise("The parent window passed not created");

		TTTOOLINFO info = { };
		info.cbSize = sizeof(info);
		info.hwnd = parent.GetHandle();
		info.uId = id;
		info.rect = newRect;
		return _SendMessage(TTM_NEWTOOLRECT, 0, reinterpret_cast<Long>(&info));
	}

	void Tooltip::SetDelaytime(const DelayTimeFlags& flags, const uInt& delay) {
		_SendMessage(TTM_SETDELAYTIME, flags, MAKELONG(delay, 0));
	}

	void Tooltip::SetMargin(const IntRect& newMargin) {
		RECT rect = newMargin;
		_SendMessage(TTM_SETMARGIN, 0, reinterpret_cast<Long>(&rect));
	}

	Int Tooltip::SetMaxtipWidth(const Int& maxWidth /*= DEFAULT_VALUE*/) {
		return _SendMessage(TTM_SETMAXTIPWIDTH, 0, maxWidth);
	}

	Nt::Byte3D Tooltip::SetTipBackgroundColor(const Byte3D& newColor) {
		return ColorRefToVector(_SendMessage(TTM_SETTIPBKCOLOR, VectorToColorRef(newColor), 0));
	}

	Nt::Byte3D Tooltip::SetTipTextColor(const Byte3D& newColor) {
		return ColorRefToVector(_SendMessage(TTM_SETTIPTEXTCOLOR, VectorToColorRef(newColor), 0));
	}

	Bool Tooltip::SetTitle(const IconType& iconType, const std::wstring& text) {
		return _SendMessage(TTM_SETTITLE, iconType, reinterpret_cast<Long>(text.c_str()));
	}

	Bool Tooltip::SetTitle(const Icon& icon, const std::wstring& text) {
		return _SendMessage(TTM_SETTITLE, reinterpret_cast<uInt>(icon.GetHandle()), reinterpret_cast<Long>(text.c_str()));
	}

	void Tooltip::SetToolInfo(const Info& info) {
		TTTOOLINFOW toolInfo = info.ToWinApiStruct();
		_SendMessage(TTM_SETTOOLINFO, 0, reinterpret_cast<Long>(&toolInfo));
	}

	void Tooltip::SetWindowTheme(const std::wstring& themeName) {
		_SendMessage(TTM_SETWINDOWTHEME, 0, reinterpret_cast<Long>(themeName.c_str()));
	}

	void Tooltip::SetTipText(const HandleWindow& parent, const uInt& id, const std::wstring& text) {
		(void)text;
		if (!parent.IsCreated())
			Raise("The parent window passed not created");

		TTTOOLINFO info = { };
		info.cbSize = sizeof(info);
		info.hinst = GetModuleHandle(nullptr);
		info.hwnd = parent.GetHandle();
		info.uId = id;
		_SendMessage(TTM_UPDATETIPTEXT, 0, reinterpret_cast<Long>(&info));
	}

}