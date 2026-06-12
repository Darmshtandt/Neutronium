#pragma once

#include <Nt/Graphics/System/GDI/Bitmap.h>

namespace Nt {
	enum ResourceFlags : uInt {
		RESOURCEFLAG_DEFAULTCOLOR = 0x00000000,
		RESOURCEFLAG_MONOCHROME = 0x00000001,
		RESOURCEFLAG_COLOR = 0x00000002,
		RESOURCEFLAG_COPYRETURNORG = 0x00000004,
		RESOURCEFLAG_COPYDELETEORG = 0x00000008,
		RESOURCEFLAG_LOADFROMFILE = 0x00000010,
		RESOURCEFLAG_LOADTRANSPARENT = 0x00000020,
		RESOURCEFLAG_DEFAULTSIZE = 0x00000040,
		RESOURCEFLAG_VGACOLOR = 0x00000080,
		RESOURCEFLAG_LOADMAP3DCOLORS = 0x00001000,
		RESOURCEFLAG_CREATEDIBSECTION = 0x00002000,
		RESOURCEFLAG_COPYFROMRESOURCE = 0x00004000,
		RESOURCEFLAG_SHARED = 0x00008000,
	};
	enum DrawFlags : uInt {
		DRAWFLAG_DEFAULT_SIZE = 0x0008,
		DRAWFLAG_IMAGE = 0x0002,
		DRAWFLAG_MASK = 0x0001,
		DRAWFLAG_NORMAL = 0x0003,
		DRAWFLAG_NOMIRROR = 0x0010,
	};

	class Icon {
	public:
		struct Info {
			uInt2D Hotstpot;
			GDI::Bitmap BitmapMask;
			GDI::Bitmap BitmapColor;
		};
		struct InfoEx : public Info {
			uShort ResourceID = 0;
			String ResourceName;
			String ModuleName;
		};
		struct ExtractedIcons {
			std::vector<Icon> SmallIcons;
			std::vector<Icon> LargeIcons;
		};

	public:
		Icon() = default;
#ifdef _WINDEF_
		NT_API Icon(const HICON& hIcon);
#endif
		NT_API Icon(const Icon& icon);
		NT_API Icon(Icon&& icon) noexcept;
		NT_API ~Icon();

		NT_API void Create(const uInt2D& size, const Byte& planes, const Byte& bitsPixel, const Byte* pAndBits, const Byte* pXorBits);
		NT_API void CreateFromResource(Byte* pBuffer, const uInt& bufferSize, const uInt& version = 0x00030000);
		NT_API void CreateFromResourceEx(Byte* pBuffer, const uInt& bufferSize, const uInt2D& iconSize, const ResourceFlags& flags, const uInt& version = 0x00030000);
		NT_API void CreateIndirect(const Info& info);
		NT_API Bool Destroy() noexcept;

		NT_API void LoadFromFile(const Nt::String& iconName);
		NT_API Icon Copy() const noexcept;
		NT_API Icon Duplicate() const noexcept;

		NT_API Bool Draw(const HandleWindow& window, const Int2D& position) const;
		NT_API Bool DrawEx(const HandleWindow& window, const IntRect& rect, const Int& fameIndex, const Bool& enableBackground, const Byte3D& color, const DrawFlags& flags) const;

		NT_API static Int LookupIdFromDirectory(Byte* buffer);
		NT_API static Int LookupIdFromDirectoryEx(Byte* buffer, const uInt2D& size, const Bool& isMonochrome);
		NT_API static Int LookupIdFromDirectoryEx(Byte* buffer, const Bool& isMonochrome);

		NT_API static Icon ExtractAssociated(const std::wstring& iconPath, Word* pData);
		NT_API static Icon Extract(const std::wstring& exeFilePath, const Int& index);
		NT_API static ExtractedIcons ExtractEx(const std::wstring& exeFilePath, const Int& startIndex, const uInt& iconsCount);
		NT_API static uInt GetIconsCount(const std::wstring& exeFilePath);

		NT_API Icon& operator = (const Icon& icon);
		NT_API Icon& operator = (Icon&& icon) noexcept;

		NT_API Info GetInfo() const;
		NT_API InfoEx GetInfoEx() const;

#ifdef _WINDEF_
		NT_API HICON GetHandle() const noexcept;
		NT_API Bool IsCreated() const noexcept;

	private:
		HICON m_Handle = nullptr;
		HINSTANCE m_hInstance = GetModuleHandle(nullptr);
#endif
	};
}