#pragma once

#include <Nt/Graphics/System/HandleWindow.h>
#include <Nt/Graphics/System/Icon.h>

namespace Nt {
	enum RasterBinaryOperations : uLong {
		RASTER_SOURCE = 0x00CC0020,  // Dest = Source
		RASTER_PATTERN = 0x00F00021, // Dest = Pattern
		RASTER_BLACK = 0x00000042, // Dest = BLACK
		RASTER_WHITE = 0x00FF0062, // Dest = WHITE

		RASTER_NOT_DEST = 0x00550009,    // Dest = ~Dest
		RASTER_NOT_SOURCE = 0x00330008, // Dest = ~Source

		RASTER_SOURCE_OR_DEST = 0x00EE0086,        // Dest = Source | Dest
		RASTER_NOT_SOURCE_OR_DEST = 0x00BB0226,  // Dest = (~Source) | Dest
		RASTER_NOT_SOURCE_OR_PATTERN = 0x00FB0A09, // Dest = (~Source) | Pattern

		RASTER_SOURCE_AND_PATTERN = 0x00C000CA, // Dest = Source & Pattern
		RASTER_SOURCE_AND_DEST = 0x008800C6, // Dest = Source & Dest
		RASTER_SOURCE_AND_NOT_DEST = 0x00440328, // Dest = Source & (~Dest)
		RASTER_NOT_SOURCE_AND_NOT_DEST = 0x001100A6, // Dest = (~Source) & (~Dest)

		RASTER_SOURCE_XOR_DEST = 0x00660046,  // Dest = Source ^ Dest
		RASTER_PATTERN_XOR_DEST = 0x005A0049, // Dest = Patten ^ Dest
	};

	class ImageList {
	public:
		enum CreateFlags {
			CREATEFLAG_MASK = 0x00000001,
			CREATEFLAG_COLOR = 0x00000000,
			CREATEFLAG_COLORDDB = 0x000000FE,
			CREATEFLAG_COLOR4 = 0x00000004,
			CREATEFLAG_COLOR8 = 0x00000008,
			CREATEFLAG_COLOR16 = 0x00000010,
			CREATEFLAG_COLOR24 = 0x00000018,
			CREATEFLAG_COLOR32 = 0x00000020,
			CREATEFLAG_PALETTE = 0x00000800,
		};
		enum StateFlags {
			STATEFLAG_NORMAL = 0x00000000,
			STATEFLAG_GLOW = 0x00000001,
			STATEFLAG_SHADOW = 0x00000002,
			STATEFLAG_SATURATE = 0x00000004,
			STATEFLAG_ALPHA = 0x00000008,
		};
		enum DrawStyles : uInt {
			DRAWSTYLE_BLEND = 0x00000004,
			DRAWSTYLE_BLEND25 = 0x00000002,
			DRAWSTYLE_BLEND50 = 0x00000004,
			DRAWSTYLE_FOCUS = 0x00000002,
			DRAWSTYLE_IMAGE = 0x00000020,
			DRAWSTYLE_MASK = 0x00000010,
			DRAWSTYLE_NORMAL = 0x00000000,
			DRAWSTYLE_SELECTED = 0x00000004,
			DRAWSTYLE_TRANSPARENT = 0x00000001
		};

		struct DrawParams {
			Int ImageIndex = 0;
			Int2D Position;
			IntRect CropRect;
			Byte3D BackgroundColor;
			Byte3D ForegroundColor;
			DrawStyles Styles = DRAWSTYLE_NORMAL;
			RasterBinaryOperations RasterOperation = RASTER_SOURCE;
			StateFlags State = STATEFLAG_NORMAL;
			Byte Alpha = 0;
			Byte GlowAndShadowEffect = 0;
		};

		struct Info {
			GDI::Bitmap Image;
			GDI::Bitmap Mask;
			IntRect Rect;
		};

	public:
		ImageList() = default;
#ifdef _WINDEF_
		NT_API ImageList(const HIMAGELIST& list);
#endif
		NT_API ~ImageList();

		NT_API void Create(const Int2D& imagesSize, const CreateFlags& flags, const uInt& initialCount, const uInt& growCount);
		NT_API Bool Destroy();

		NT_API void Load(const std::wstring& resourceName, const uInt& imagesWidth, const uInt& imagesCount, const Byte3D& colorMask, const ResourceFlags& flags);
		NT_API void LoadFromBitmap(const std::wstring& resourceName, const uInt& imagesWidth, const uInt& imagesCount, const Byte3D& colorMask);

		NT_API Bool Copy(ImageList* pImageList, const Int& destImageIndex, const Int& sourceImageIndex, const Bool& isSwap);
		NT_API ImageList Merge(const Int& imageIndex_1, ImageList* pImageList, const Int& imageIndex_2, const Int2D& offset) const;
		NT_API ImageList Duplicate() const;

		NT_API Int Add(const GDI::Bitmap* pBitmap, const GDI::Bitmap* pBitmapMask = nullptr);
		NT_API Bool Remove(const Int& index);
		NT_API Bool Clear();
		NT_API Bool Replace(const uInt& index, const GDI::Bitmap* pBitmap, const GDI::Bitmap* pBitmapMask = nullptr);

		NT_API Int AddIcon(const Icon& icon);
		NT_API Int ReplaceIcon(const Int& index, const Icon& icon);

		NT_API Int AddMasked(const GDI::Bitmap& bitmap, const Byte3D& mask);

		NT_API Bool Draw(const HandleWindow& window, const uInt& imageIndex, const Int2D& position, const DrawStyles& styles) const;
		NT_API Bool DrawEx(const HandleWindow& window, const uInt& imageIndex, const IntRect& rect, const Byte3D& background, const Byte3D& foreground, const DrawStyles& styles) const;
		NT_API Bool DrawIndirect(const HandleWindow& window, const DrawParams& drawParams) const;

		NT_API Bool BeginDrag(const Int& imageIndex, const Int2D& hotspot) const;
		NT_API Bool DragEnter(const HandleWindow& window, const Int2D& position) const noexcept;
		NT_API Bool DragLeave(const HandleWindow& window) const noexcept;
		NT_API Bool DragMove(const Int2D& position) noexcept;
		NT_API Bool DragShowNolock(const Bool& isShow);
		NT_API void EndDrag() const noexcept;

		NT_API Int GetImageCount() const;
		NT_API Byte3D GetBackgroundColor() const;
		NT_API Icon GetIcon(const Int& index, const DrawFlags& flags) const;
		NT_API ImageList GetDragImage(Int2D& dragPosition, Int2D& hotspot) const noexcept;
		NT_API Bool GetIconSize(Int2D& size) const;
		NT_API Info GetImageInfo(const Int& imageIndex) const;

#ifdef _WINDEF_
		NT_API HIMAGELIST GetHandle() const noexcept;
#endif

		NT_API Bool SetImageCount(const uInt& newCount);
		NT_API Byte3D SetBackgroundColor(const Byte3D& newColor);
		NT_API Bool SetOverlayImage(const Int& imageIndex, const Int& overlay);
		NT_API Bool SetDragCursorImage(const Int& imageIndex, const Int2D& hotspot);
		NT_API Bool SetIconSize(const uInt2D& size);

#ifdef _WINDEF_
	private:
		HIMAGELIST m_Handle = nullptr;
		HINSTANCE m_hInstance = GetModuleHandle(nullptr);
#endif
	};
}