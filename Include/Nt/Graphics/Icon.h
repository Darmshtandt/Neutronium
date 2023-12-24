#pragma once

namespace Nt {
	enum ResourceFlags : uInt {
		RESOURCEFLAG_DEFAULTCOLOR = LR_DEFAULTCOLOR,
		RESOURCEFLAG_MONOCHROME = LR_MONOCHROME,
		RESOURCEFLAG_COLOR = LR_COLOR,
		RESOURCEFLAG_COPYRETURNORG = LR_COPYRETURNORG,
		RESOURCEFLAG_COPYDELETEORG = LR_COPYDELETEORG,
		RESOURCEFLAG_LOADFROMFILE = LR_LOADFROMFILE,
		RESOURCEFLAG_LOADTRANSPARENT = LR_LOADTRANSPARENT,
		RESOURCEFLAG_DEFAULTSIZE = LR_DEFAULTSIZE,
		RESOURCEFLAG_VGACOLOR = LR_VGACOLOR,
		RESOURCEFLAG_LOADMAP3DCOLORS = LR_LOADMAP3DCOLORS,
		RESOURCEFLAG_CREATEDIBSECTION = LR_CREATEDIBSECTION,
		RESOURCEFLAG_COPYFROMRESOURCE = LR_COPYFROMRESOURCE,
		RESOURCEFLAG_SHARED = LR_SHARED,
	};
	enum DrawFlags : uInt {
		DRAWFLAG_DEFAULT_SIZE = DI_DEFAULTSIZE,
		DRAWFLAG_IMAGE = DI_IMAGE,
		DRAWFLAG_MASK = DI_MASK,
		DRAWFLAG_NORMAL = DI_NORMAL,
		DRAWFLAG_NOMIRROR = DI_NOMIRROR,
	};

	class Icon {
	public:
		struct Info {
			uInt2D Hotstpot;
			GDI::Bitmap BitmapMask;
			GDI::Bitmap BitmapColor;
		};
		struct InfoEx : public Info {
			uShort ResourceID;
			String ResourceName;
			String ModuleName;
		};
		struct ExtractedIcons {
			std::vector<Icon> SmallIcons;
			std::vector<Icon> LargeIcons;
		};

	public:
		Icon() = default;
		Icon(const HICON& hIcon) : m_Handle(hIcon)
		{
		}
		Icon(const Icon& icon) : m_hInstance(icon.m_hInstance)
		{
			Icon copiedIcon = icon.Copy();
			m_Handle = copiedIcon.m_Handle;
			copiedIcon.m_Handle = nullptr;
		}
		~Icon() {
			Destroy();
		}

		void Create(const uInt2D& size, const Byte& planes, const Byte& bitsPixel, const Byte* pAndBits, const Byte* pXorBits) {
			if (m_Handle) {
				Log::Warning("Icon already created");
				return;
			}

			m_Handle = CreateIcon(m_hInstance, size.x, size.y, planes, bitsPixel, pAndBits, pXorBits);
			if (m_Handle == nullptr)
				Raise("Failed to create Icon");
		}
		void CreateFromResource(Byte* pBuffer, const uInt& bufferSize, const uInt& version = 0x00030000) {
			if (m_Handle) {
				Log::Warning("Icon already created");
				return;
			}

			m_Handle = CreateIconFromResource(pBuffer, bufferSize, TRUE, version);
			if (m_Handle == nullptr)
				Raise("Failed to create Icon");
		}
		void CreateFromResourceEx(Byte* pBuffer, const uInt& bufferSize, const uInt2D& iconSize, const ResourceFlags& flags, const uInt& version = 0x00030000) {
			if (m_Handle) {
				Log::Warning("Icon already created");
				return;
			}

			m_Handle = CreateIconFromResourceEx(pBuffer, bufferSize, TRUE, version, iconSize.x, iconSize.y, flags);
			if (m_Handle == nullptr)
				Raise("Failed to create Icon");
		}
		void CreateIndirect(const Info& info) {
			if (m_Handle) {
				Log::Warning("Icon already created");
				return;
			}

			ICONINFO iconInfo = { };
			iconInfo.fIcon = TRUE;
			iconInfo.xHotspot = info.Hotstpot.x;
			iconInfo.yHotspot = info.Hotstpot.y;
			if (!info.BitmapColor.IsCreated())
				Raise("Color bitmap pointer is null");

			iconInfo.hbmColor = info.BitmapColor.GetHandle();
			iconInfo.hbmMask = info.BitmapMask.GetHandle();

			m_Handle = CreateIconIndirect(&iconInfo);
			if (m_Handle == nullptr)
				Raise("Failed to create Icon");
		}
		Bool Destroy() noexcept {
			if (m_Handle == nullptr)
				return true;

			const Bool result = DestroyIcon(m_Handle);
			m_Handle = nullptr;
			return result;
		}

		void LoadFromFile(const Nt::String& iconName) {
			if (m_Handle) {
				Log::Warning("Icon already created");
				return;
			}
			
			const std::wstring wIconName = iconName.wstr();

			m_Handle = LoadIcon(m_hInstance, wIconName.c_str());
			if (m_Handle == nullptr)
				Raise("Failed to load Icon");
		}
		Icon Copy() const noexcept {
			return CopyIcon(m_Handle);
		}
		Icon Duplicate() const noexcept {
			return DuplicateIcon(m_hInstance, m_Handle);
		}

		Bool Draw(const HandleWindow& window, const Int2D& position) const {
			if (m_Handle == nullptr)
				Raise("Failed to create Icon");
			return DrawIcon(window.GetDC(), position.x, position.y, m_Handle);
		}
		Bool DrawEx(const HandleWindow& window, const IntRect& rect, const Int& fameIndex, const Bool& enableBackground, const Byte3D& color, const DrawFlags& flags) const {
			if (m_Handle == nullptr)
				Raise("Failed to create Icon");

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

		static Int LookupIdFromDirectory(Byte* buffer) {
			return LookupIconIdFromDirectory(buffer, TRUE);
		}
		static Int LookupIdFromDirectoryEx(Byte* buffer, const uInt2D& size, const Bool& isMonochrome) {
			return LookupIconIdFromDirectoryEx(buffer, TRUE, size.x, size.y, (isMonochrome) ? LR_DEFAULTCOLOR : LR_MONOCHROME);
		}
		static Int LookupIdFromDirectoryEx(Byte* buffer, const Bool& isMonochrome) {
			return LookupIconIdFromDirectoryEx(buffer, TRUE, 0, 0, (isMonochrome) ? LR_DEFAULTCOLOR : LR_MONOCHROME);
		}

		static Icon ExtractAssociated(const std::wstring& iconPath, Word* pData) {
			return ExtractAssociatedIcon(GetModuleHandle(nullptr), const_cast<wChar*>(iconPath.c_str()), pData);
		}
		static Icon Extract(const std::wstring& exeFilePath, const Int& index) {
			return ExtractIcon(GetModuleHandle(nullptr), const_cast<wChar*>(exeFilePath.c_str()), index);
		}
		static ExtractedIcons& ExtractEx(const std::wstring& exeFilePath, const Int& startIndex, const uInt& iconsCount) {
			const uInt maxIconsCount = GetIconsCount(exeFilePath);
			if (startIndex > maxIconsCount)
				Log::Warning("Start index out of range");
			else if (startIndex + iconsCount > maxIconsCount)
				Log::Warning("IconsCout argument with given start index is out of range");

			HICON* hSmallIconsArray = new HICON[iconsCount];
			HICON* hLargeIconsArray = new HICON[iconsCount];

			uInt result = ExtractIconEx(&exeFilePath.c_str()[0], startIndex, hLargeIconsArray, hSmallIconsArray, iconsCount);
			if (result == UINT_MAX)
				Raise("Failed to extract icon");

			ExtractedIcons icons;
			for (uInt i = 0; i < iconsCount; ++i) {
				icons.SmallIcons.push_back(hSmallIconsArray[i]);
				icons.LargeIcons.push_back(hLargeIconsArray[i]);
			}

			delete(hSmallIconsArray);
			delete(hLargeIconsArray);

			return icons;
		}
		static uInt GetIconsCount(const std::wstring& exeFilePath) {
			return ExtractIconEx(&exeFilePath.c_str()[0], -1, nullptr, nullptr, 0);
		}

		Info& GetInfo() const {
			if (m_Handle == nullptr)
				Raise("Failed to create Icon");

			ICONINFO iconInfo = { };
			if (!GetIconInfo(m_Handle, &iconInfo))
				Raise("Failed to get icon info");

			Info info;
			info.Hotstpot = { iconInfo.xHotspot, iconInfo.yHotspot };
			info.BitmapColor = iconInfo.hbmColor;
			info.BitmapMask = iconInfo.hbmMask;
			return info;
		}
		InfoEx& GetInfoEx() const {
			if (m_Handle == nullptr)
				Raise("Failed to create Icon");

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

		HICON GetHandle() const noexcept {
			return m_Handle;
		}
		Bool IsCreated() const noexcept {
			return m_Handle;
		}

	private:
		HICON m_Handle = nullptr;
		HINSTANCE m_hInstance = GetModuleHandle(nullptr);
	};
}