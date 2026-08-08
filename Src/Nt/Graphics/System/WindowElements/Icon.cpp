#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOMSG

#include <windows.h>
#include <shellapi.h>

#include <Nt/Graphics/System/Icon.h>

namespace Nt {
	Icon::Icon(const HICON& hIcon) : m_Handle(hIcon) 
	{
	}
	Icon::Icon(const Icon& icon) : m_hInstance(icon.m_hInstance) {
		Icon copiedIcon = icon.Copy();
		m_Handle = copiedIcon.m_Handle;
		copiedIcon.m_Handle = nullptr;
	}
	Icon::Icon(Icon&& icon) noexcept :
		m_Handle(std::move(icon.m_Handle)),
		m_hInstance(std::move(icon.m_hInstance)) {
	}

	Icon::~Icon() {
		Destroy();
	}

	void Icon::Create(const uInt2D& size, const Byte& planes, const Byte& bitsPixel, const Byte* pAndBits, const Byte* pXorBits) {
		if (m_Handle != nullptr) {
			Log::Instance().Warning("Icon already created");
			return;
		}

		m_Handle = CreateIcon(m_hInstance, size.x, size.y, planes, bitsPixel, pAndBits, pXorBits);
		RequireNotNull(m_Handle, "Failed to create Icon");
	}

	void Icon::CreateFromResource(Byte* pBuffer, const uInt& bufferSize, const uInt& version /*= 0x00030000*/) {
		if (m_Handle != nullptr) {
			Log::Instance().Warning("Icon already created");
			return;
		}

		m_Handle = CreateIconFromResource(pBuffer, bufferSize, TRUE, version);
		RequireNotNull(m_Handle, "Failed to create Icon");
	}

	void Icon::CreateFromResourceEx(Byte* pBuffer, const uInt& bufferSize, const uInt2D& iconSize, const ResourceFlags& flags, const uInt& version /*= 0x00030000*/) {
		if (m_Handle != nullptr) {
			Log::Instance().Warning("Icon already created");
			return;
		}

		m_Handle = CreateIconFromResourceEx(pBuffer, bufferSize, TRUE, version, iconSize.x, iconSize.y, flags);
		RequireNotNull(m_Handle, "Failed to create Icon");
	}

	void Icon::CreateIndirect(const Info& info) {
		if (m_Handle != nullptr) {
			Log::Instance().Warning("Icon already created");
			return;
		}

		ICONINFO iconInfo = { };
		iconInfo.fIcon = TRUE;
		iconInfo.xHotspot = info.Hotstpot.x;
		iconInfo.yHotspot = info.Hotstpot.y;
		if (!info.BitmapColor.IsCreated())
			Raise("Color bitmap not created");

		iconInfo.hbmColor = info.BitmapColor.GetHandle();
		iconInfo.hbmMask = info.BitmapMask.GetHandle();

		m_Handle = CreateIconIndirect(&iconInfo);
		RequireNotNull(m_Handle, "Failed to create Icon");
	}

	Bool Icon::Destroy() noexcept {
		if (m_Handle == nullptr)
			return true;

		const Bool result = DestroyIcon(m_Handle);
		m_Handle = nullptr;
		return result;
	}

	void Icon::LoadFromFile(const Nt::String& iconName) {
		if (m_Handle != nullptr) {
			Log::Instance().Warning("Icon already created");
			return;
		}

		const std::wstring wIconName(iconName);

		m_Handle = LoadIcon(m_hInstance, wIconName.c_str());
		RequireNotNull(m_Handle, "Failed to load Icon");
	}

	Nt::Icon Icon::Copy() const noexcept {
		return CopyIcon(m_Handle);
	}

	Nt::Icon Icon::Duplicate() const noexcept {
		return DuplicateIcon(m_hInstance, m_Handle);
	}

	Bool Icon::Draw(const HandleWindow& window, const Int2D& position) const {
		RequireNotNull(m_Handle, "Icon not created");
		return DrawIcon(window.GetDC(), position.x, position.y, m_Handle);
	}

	Bool Icon::DrawEx(const HandleWindow& window, const IntRect& rect, const Int& fameIndex, const Bool& enableBackground, const Byte3D& color, const DrawFlags& flags) const {
		RequireNotNull(m_Handle, "Icon not created");

		if (enableBackground) {
			return DrawIconEx(
				window.GetDC(), rect.Left, rect.Top, m_Handle,
				rect.Right, rect.Bottom, fameIndex,
				Nt::CreateSolidBrush(color), flags);
		}

		return DrawIconEx(
			window.GetDC(), rect.Left, rect.Top, m_Handle,
			rect.Right, rect.Bottom, fameIndex, nullptr, flags);
	}

	Int Icon::LookupIdFromDirectory(Byte* buffer) {
		return LookupIconIdFromDirectory(buffer, TRUE);
	}

	Int Icon::LookupIdFromDirectoryEx(Byte* buffer, const uInt2D& size, const Bool& isMonochrome) {
		return LookupIconIdFromDirectoryEx(buffer, TRUE, size.x, size.y, (isMonochrome) ? LR_DEFAULTCOLOR : LR_MONOCHROME);
	}

	Int Icon::LookupIdFromDirectoryEx(Byte* buffer, const Bool& isMonochrome) {
		return LookupIconIdFromDirectoryEx(buffer, TRUE, 0, 0, (isMonochrome) ? LR_DEFAULTCOLOR : LR_MONOCHROME);
	}

	Nt::Icon Icon::ExtractAssociated(const std::wstring& iconPath, Word* pData) {
		return ExtractAssociatedIcon(GetModuleHandle(nullptr), const_cast<wChar*>(iconPath.c_str()), pData);
	}

	Nt::Icon Icon::Extract(const std::wstring& exeFilePath, const Int& index) {
		return ExtractIcon(GetModuleHandle(nullptr), const_cast<wChar*>(exeFilePath.c_str()), index);
	}

	Nt::Icon::ExtractedIcons Icon::ExtractEx(const std::wstring& exeFilePath, const Int& startIndex, const uInt& iconsCount) {
		const Int maxIconsCount = GetIconsCount(exeFilePath);
		if (startIndex > maxIconsCount)
			Log::Instance().Warning("Start index out of range");
		else if (startIndex + Int(iconsCount) > maxIconsCount)
			Log::Instance().Warning("IconsCout argument with given start index is out of range");

		HICON* hSmallIconsArray = new HICON[iconsCount];
		HICON* hLargeIconsArray = new HICON[iconsCount];

		const uInt result = ExtractIconEx(&exeFilePath.c_str()[0], startIndex, hLargeIconsArray, hSmallIconsArray, iconsCount);
		if (result == UINT_MAX)
			Raise("Failed to extract icon");

		ExtractedIcons icons;
		for (uInt i = 0; i < iconsCount; ++i) {
			icons.SmallIcons.push_back(hSmallIconsArray[i]);
			icons.LargeIcons.push_back(hLargeIconsArray[i]);
		}

		delete[](hSmallIconsArray);
		delete[](hLargeIconsArray);

		return icons;
	}

	uInt Icon::GetIconsCount(const std::wstring& exeFilePath) {
		return ExtractIconEx(&exeFilePath.c_str()[0], -1, nullptr, nullptr, 0);
	}

	Nt::Icon::Info Icon::GetInfo() const {
		RequireNotNull(m_Handle, "Icon not created");

		ICONINFO iconInfo = { };
		if (!GetIconInfo(m_Handle, &iconInfo))
			Raise("Failed to get icon info");

		Info info = {
			uInt2D(iconInfo.xHotspot, iconInfo.yHotspot),
			iconInfo.hbmMask,
			iconInfo.hbmColor
		};

		return info;
	}

	Nt::Icon::InfoEx Icon::GetInfoEx() const {
		RequireNotNull(m_Handle, "Icon not created");

		ICONINFOEX iconInfo = { };
		if (!GetIconInfoEx(m_Handle, &iconInfo))
			Raise("Failed to get icon info");

		InfoEx info;
		info.Hotstpot = { iconInfo.xHotspot, iconInfo.yHotspot };
		info.BitmapColor = GDI::Bitmap(iconInfo.hbmColor);
		info.BitmapMask = GDI::Bitmap(iconInfo.hbmMask);
		info.ResourceID = iconInfo.wResID;
		info.ResourceName = iconInfo.szResName;
		info.ModuleName = iconInfo.szModName;
		return info;
	}

	HICON Icon::GetHandle() const noexcept {
		return m_Handle;
	}

	Bool Icon::IsCreated() const noexcept {
		return m_Handle;
	}

	Nt::Icon& Icon::operator=(Icon&& icon) noexcept {
		if (this == &icon)
			return *this;

		Destroy();

		m_Handle = std::move(icon.m_Handle);
		m_hInstance = std::move(icon.m_hInstance);

		return *this;
	}

	Nt::Icon& Icon::operator=(const Icon& icon) {
		if (this == &icon)
			return *this;

		Destroy();

		m_Handle = icon.m_Handle;
		m_hInstance = icon.m_hInstance;

		return *this;
	}

}