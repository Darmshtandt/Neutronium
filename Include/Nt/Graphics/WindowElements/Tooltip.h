#pragma once

namespace Nt {
	class Tooltip : public HandleWindow {
	public:
		enum Styles {
			STYLE_ALWAYSTIP = TTS_ALWAYSTIP,
			STYLE_BALLOON = TTS_BALLOON,
			STYLE_CLOSE = TTS_CLOSE,
			STYLE_NOANIMATE = TTS_NOANIMATE,
			STYLE_NOFADE = TTS_NOFADE,
			STYLE_NOPREFIX = TTS_NOPREFIX,
			STYLE_USEVISUALSTYLE = TTS_USEVISUALSTYLE,
		};
		enum DisplayFlags : uInt {
			FLAG_NULL = 0,
			FLAG_ABSOLUTE = TTF_ABSOLUTE,
			FLAG_CENTERTIP = TTF_CENTERTIP,
			FLAG_IDISHWND = TTF_IDISHWND,
			FLAG_PARSELINKS = TTF_PARSELINKS,
			FLAG_RTLREADING = TTF_RTLREADING,
			FLAG_SUBCLASS = TTF_SUBCLASS,
			FLAG_TRACK = TTF_TRACK,
			FLAG_TRANSPARENT = TTF_TRANSPARENT
		};
		enum DelayTimeFlags {
			DELAYFLAG_AUTOOPOP = TTDT_AUTOPOP,
			DELAYFLAG_INITIAL = TTDT_INITIAL,
			DELAYFLAG_RESHOW = TTDT_RESHOW,
			DELAYFLAG_AUTOMATIC = TTDT_AUTOMATIC
		};
		enum IconType {
			ICON_NONE = TTI_NONE,
			ICON_INFO = TTI_INFO,
			ICON_WARNING = TTI_WARNING,
			ICON_ERROR = TTI_ERROR,
			ICON_INFO_LARGE = TTI_INFO_LARGE,
			ICON_WARNING_LARGE = TTI_WARNING_LARGE,
			ICON_ERROR_LARGE = TTI_ERROR_LARGE
		};

		struct Info {
			Info() noexcept = default;
			Info(const TTTOOLINFO& info) noexcept :
				Text(info.lpszText),
				Rect(info.rect),
				Flags(DisplayFlags(info.uFlags)),
				Parent(info.hwnd),
				ID(info.uId),
				Param(info.lParam),
				Reserved(info.lpReserved)
			{
			}

			TTTOOLINFO ToWinApiStruct() const noexcept {
				TTTOOLINFO info;
				info.cbSize = sizeof(info);
				info.uFlags = Flags;
				info.hwnd = Parent.GetHandle();
				info.uId = ID;
				info.rect = Rect;
				info.hinst = GetModuleHandle(nullptr);
				info.lpszText = const_cast<wChar*>(Text.c_str());
				info.lParam = Param;
				info.lpReserved = Reserved;
				return info;
			}

			std::wstring Text;
			IntRect Rect;
			DisplayFlags Flags = FLAG_NULL;
			HandleWindow Parent;
			uInt ID = 0;
			Long Param = 0;
			void* Reserved = nullptr;
		};
		struct HitTestInfo {
			HitTestInfo() = default;
			HitTestInfo(const TTHITTESTINFO& info) :
				Parent(info.hwnd),
				Point(info.pt),
				ToolInfo(info.ti)
			{
			}

			TTHITTESTINFO ToWinApiStruct() const noexcept {
				TTHITTESTINFO hitInfo;
				hitInfo.hwnd = Parent.GetHandle();
				hitInfo.pt = Point;
				hitInfo.ti = ToolInfo.ToWinApiStruct();
				return hitInfo;
			}

			HandleWindow Parent;
			Int2D Point;
			Info ToolInfo;
		};

		struct Title {
			Title() = default;
			Title(const TTGETTITLE& title) :
				BitmapID(title.uTitleBitmap),
				TextLength(title.cch),
				pText(title.pszTitle)
			{
			}

			TTGETTITLE ToWinApiStruct() const noexcept {
				TTGETTITLE title;
				title.dwSize = sizeof(title);
				title.uTitleBitmap = BitmapID;
				title.cch = TextLength;
				title.pszTitle = pText;
				return title;
			}

			uInt BitmapID = -1;
			uInt TextLength = 0;
			wChar* pText = nullptr;
		};

	public:
		Tooltip() = default;
		Tooltip(const HWND& hwnd) : HandleWindow(hwnd) 
		{
		}

		void Create() {
			Create(IntRect(), "");
		}

		void AddTool(const HandleWindow& parent, const DisplayFlags& flags, const uInt& id, const String& text) {
			if (!parent.IsCreated())
				Raise("The parent window passed is not created");

			Info info = { };
			info.Flags = flags;
			info.ID = id;
			info.Text = text;
			info.Parent = parent;

			TTTOOLINFO winApiStruct = info.ToWinApiStruct();
			_SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<Long>(&winApiStruct));
		}
		void RemoveTool(const HandleWindow& parent, const uInt& ID) {
			if (!parent.IsCreated())
				Raise("The parent window passed is not created");

			TTTOOLINFO winApiStruct;
			winApiStruct.uId = ID;
			winApiStruct.hwnd = parent.GetHandle();
			_SendMessage(TTM_DELTOOL, 0, reinterpret_cast<Long>(&winApiStruct));
		}

		Bool AdjustRect(const Bool& isTextRect, IntRect* pRect) const {
			if (pRect == nullptr)
				Raise("The rect pointer passed is null");

			RECT adjustableRect;
			const Bool result = _SendMessage(TTM_ADJUSTRECT, isTextRect, 
				reinterpret_cast<Long>(&adjustableRect));
			if (result)
				*pRect = adjustableRect;
			return result;
		}

		void Update() {
			_SendMessage(TTM_UPDATE, 0, 0);
		}

		void Hide() {
			_SendMessage(TTM_POP, 0, 0);
		}
		void Show() {
			_SendMessage(TTM_POPUP, 0, 0);
		}
		HandleWindow ShowFromPoint(const Int2D& position) {
			POINT point = position;
			const Long lParam = reinterpret_cast<Long>(&point);
			const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(TTM_WINDOWFROMPOINT, 0, lParam));
			return HandleWindow(hwnd);
		}

		Bool ToggleActivate(const Bool& isActive) {
			return _SendMessage(TTM_ACTIVATE, isActive, 0);
		}
		void ToggleTrackActivate(const HandleWindow& parent, const uInt& id, const Bool& isActive) {
			if (!parent.IsCreated())
				Raise("The parent window passed is not created");

			TTTOOLINFO info = { };
			info.cbSize = sizeof(info);
			info.hwnd = parent.GetHandle();
			info.uId = id;
			_SendMessage(TTM_TRACKACTIVATE, isActive, reinterpret_cast<Long>(&info));
		}
		void TrackPosition(const Int2D& position) {
			_SendMessage(TTM_TRACKPOSITION, 0, MAKELONG(position.x, position.y));
		}

		Bool HitTest(HitTestInfo* pInfo) const {
			if (pInfo == nullptr)
				Raise("The pInfo passed in is null");

			TTHITTESTINFO info = pInfo->ToWinApiStruct();
			const Bool result = _SendMessage(TTM_HITTEST, 0, 0);
			if (result)
				(*pInfo) = info;

			return result;
		}

		Info EnumTools(const uInt& parentID) {
			wChar buffer[80];

			TOOLINFO info = { };
			info.cbSize = sizeof(info);
			info.lpszText = buffer;
			_SendMessage(TTM_ENUMTOOLS, parentID, reinterpret_cast<Long>(&info));
			return info;
		}

		Int2D GeSize(Info* pInfo) const {
			if (pInfo == nullptr)
				Raise("The pInfo passed in is null");

			TOOLINFO toolInfo = pInfo->ToWinApiStruct();
			const Long size = _SendMessage(TTM_GETBUBBLESIZE, 0, reinterpret_cast<Long>(&toolInfo));
			(*pInfo) = toolInfo;

			return Int2D(LOWORD(size), HIWORD(size));
		}
		Bool GetCurrentTool(Info* pInfo) const {
			if (pInfo == nullptr)
				Raise("The pInfo passed in is null");

			TOOLINFO toolInfo = pInfo->ToWinApiStruct();
			const Bool result = _SendMessage(TTM_GETCURRENTTOOL, 0, reinterpret_cast<Long>(&toolInfo));
			(*pInfo) = toolInfo;

			return result;
		}
		Int GetDelayTime(const DelayTimeFlags& flags) const {
			return _SendMessage(TTM_GETDELAYTIME, flags, 0);
		}
		IntRect GetMargin() const {
			RECT rect;
			_SendMessage(TTM_GETMARGIN, 0, reinterpret_cast<Long>(&rect));
			return rect;
		}
		Int GetMaxTipWidth() const {
			return _SendMessage(TTM_GETMAXTIPWIDTH, 0, 0);
		}
		String GetText(const HandleWindow& parent, const uInt& id) const {
			if (!parent.IsCreated())
				Raise("The parent window passed is not created");

			TOOLINFO info = { };
			info.cbSize = sizeof(info);
			info.hwnd = parent.GetHandle();
			info.uId = id;
			_SendMessage(TTM_GETTEXT, 4096, reinterpret_cast<Long>(&info));

			return info.lpszText;
		}
		Byte3D GetTipBackgroundColor() const {
			return ColorRefToVector(_SendMessage(TTM_GETTIPBKCOLOR, 0, 0));
		}
		Byte3D GetTipTextColor() const {
			return ColorRefToVector(_SendMessage(TTM_GETTIPTEXTCOLOR, 0, 0));
		}
		Title GetTitle() const {
			TTGETTITLE title = { };
			title.dwSize = sizeof(title);
			_SendMessage(TTM_GETTITLE, 0, reinterpret_cast<Long>(&title));
			return title;
		}
		uInt GetToolCount() const {
			return _SendMessage(TTM_GETTOOLCOUNT, 0, 0);
		}
		Info GetToolInfo() const {
			TTTOOLINFO info;
			info.cbSize = sizeof(info);
			if (!_SendMessage(TTM_GETTOOLINFO, 0, reinterpret_cast<Long>(&info)))
				Raise("Failed to get tool info");

			return info;
		}

		Bool SetToolRect(const HandleWindow& parent, const uInt& id, const IntRect& newRect) {
			if (!parent.IsCreated())
				Raise("The parent window passed is not created");

			TTTOOLINFO info = { };
			info.cbSize = sizeof(info);
			info.hwnd = parent.GetHandle();
			info.uId = id;
			info.rect = newRect;
			return _SendMessage(TTM_NEWTOOLRECT, 0, reinterpret_cast<Long>(&info));
		}
		void SetDelaytime(const DelayTimeFlags& flags, const uInt& delay) {
			_SendMessage(TTM_SETDELAYTIME, flags, MAKELONG(delay, 0));
		}
		void SetMargin(const IntRect& newMargin) {
			RECT rect = newMargin;
			_SendMessage(TTM_SETMARGIN, 0, reinterpret_cast<Long>(&rect));
		}
		Int SetMaxtipWidth(const Int& maxWidth = DEFAULT_VALUE) {
			return _SendMessage(TTM_SETMAXTIPWIDTH, 0, maxWidth);
		}
		Byte3D SetTipBackgroundColor(const Byte3D& newColor) {
			return ColorRefToVector(_SendMessage(TTM_SETTIPBKCOLOR, VectorToColorRef(newColor), 0));
		}
		Byte3D SetTipTextColor(const Byte3D& newColor) {
			return ColorRefToVector(_SendMessage(TTM_SETTIPTEXTCOLOR, VectorToColorRef(newColor), 0));
		}
		Bool SetTitle(const IconType& iconType, const std::wstring& text) {
			return _SendMessage(TTM_SETTITLE, iconType, reinterpret_cast<Long>(text.c_str()));
		}
		Bool SetTitle(const Icon& icon, const std::wstring& text) {
			return _SendMessage(TTM_SETTITLE, reinterpret_cast<uInt>(icon.GetHandle()), reinterpret_cast<Long>(text.c_str()));
		}
		void SetToolInfo(const Info& info) {
			TTTOOLINFOW toolInfo = info.ToWinApiStruct();
			_SendMessage(TTM_SETTOOLINFO, 0, reinterpret_cast<Long>(&toolInfo));
		}
		void SetWindowTheme(const std::wstring& themeName) {
			_SendMessage(TTM_SETWINDOWTHEME, 0, reinterpret_cast<Long>(themeName.c_str()));
		}
		void SetTipText(const HandleWindow& parent, const uInt& id, const std::wstring& text) {
			if (!parent.IsCreated())
				Raise("The parent window passed is not created");

			TTTOOLINFO info = { };
			info.cbSize = sizeof(info);
			info.hinst = GetModuleHandle(nullptr);
			info.hwnd = parent.GetHandle();
			info.uId = id;
			_SendMessage(TTM_UPDATETIPTEXT, 0, reinterpret_cast<Long>(&info));
		}

	private:
		using HandleWindow::GetParent;
		using HandleWindow::SetParent;

		void Create([[maybe_unused]] const IntRect& windowRect, [[maybe_unused]] const String& name) override {
			m_Name = L"";
			m_ClassName = TOOLTIPS_CLASS;
			m_WindowRect = IntRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
			m_Styles |= WS_POPUP;

			_CreateWindow();
		}
	};
}