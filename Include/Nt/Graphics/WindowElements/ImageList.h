#pragma once

namespace Nt {
	enum RasterBinaryOperations : uLong {
		RASTER_SOURCE = SRCCOPY,  // Dest = Source
		RASTER_PATTERN = PATCOPY, // Dest = Pattern
		RASTER_BLACK = BLACKNESS, // Dest = BLACK
		RASTER_WHITE = WHITENESS, // Dest = WHITE

		RASTER_NOT_DEST = DSTINVERT,    // Dest = ~Dest
		RASTER_NOT_SOURCE = NOTSRCCOPY, // Dest = ~Source

		RASTER_SOURCE_OR_DEST = SRCPAINT,        // Dest = Source | Dest
		RASTER_NOT_SOURCE_OR_DEST = MERGEPAINT,  // Dest = (~Source) | Dest
		RASTER_NOT_SOURCE_OR_PATTERN = PATPAINT, // Dest = (~Source) | Pattern

		RASTER_SOURCE_AND_PATTERN = MERGECOPY,        // Dest = Source & Pattern
		RASTER_SOURCE_AND_DEST = SRCAND,              // Dest = Source & Dest
		RASTER_SOURCE_AND_NOT_DEST = SRCERASE,        // Dest = Source & (~Dest)
		RASTER_NOT_SOURCE_AND_NOT_DEST = NOTSRCERASE, // Dest = (~Source) & (~Dest)

		RASTER_SOURCE_XOR_DEST = SRCINVERT,  // Dest = Source ^ Dest
		RASTER_PATTERN_XOR_DEST = PATINVERT, // Dest = Patten ^ Dest
	};

	class ImageList {
	public:
		enum CreateFlags {
			CREATEFLAG_MASK = ILC_MASK,
			CREATEFLAG_COLOR = ILC_COLOR,
			CREATEFLAG_COLORDDB = ILC_COLORDDB,
			CREATEFLAG_COLOR4 = ILC_COLOR4,
			CREATEFLAG_COLOR8 = ILC_COLOR8,
			CREATEFLAG_COLOR16 = ILC_COLOR16,
			CREATEFLAG_COLOR24 = ILC_COLOR24,
			CREATEFLAG_COLOR32 = ILC_COLOR32,
			CREATEFLAG_PALETTE = ILC_PALETTE,
		};
		enum StateFlags {
			STATEFLAG_NORMAL = ILS_NORMAL,
			STATEFLAG_GLOW = ILS_GLOW,
			STATEFLAG_SHADOW = ILS_SHADOW,
			STATEFLAG_SATURATE = ILS_SATURATE,
			STATEFLAG_ALPHA = ILS_ALPHA,
		};
		enum DrawStyles : uInt {
			DRAWSTYLE_BLEND = ILD_BLEND,
			DRAWSTYLE_BLEND25 = ILD_BLEND25,
			DRAWSTYLE_BLEND50 = ILD_BLEND50,
			DRAWSTYLE_FOCUS = ILD_FOCUS,
			DRAWSTYLE_IMAGE = ILD_IMAGE,
			DRAWSTYLE_MASK = ILD_MASK,
			DRAWSTYLE_NORMAL = ILD_NORMAL,
			DRAWSTYLE_SELECTED = ILD_SELECTED,
			DRAWSTYLE_TRANSPARENT = ILD_TRANSPARENT
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
		ImageList(const HIMAGELIST& list) :
			m_Handle(list)
		{
		}
		~ImageList() {
			Destroy();
		}

		void Create(const Int2D& imagesSize, const CreateFlags& flags, const uInt& initialCount, const uInt& growCount) {
			m_Handle = ImageList_Create(imagesSize.x, imagesSize.y, flags, initialCount, growCount);
			RequireNotNull(m_Handle, "Failed to create image list");
		}
		Bool Destroy() {
			if (m_Handle == nullptr)
				return true;

			const Bool result = ImageList_Destroy(m_Handle);
			m_Handle = nullptr;
			return result;
		}

		void Load(const std::wstring& resourceName, const uInt& imagesWidth, const uInt& imagesCount, const Byte3D& colorMask, const ResourceFlags& flags) {
			m_Handle = ImageList_LoadImage(
				nullptr, resourceName.c_str(), imagesWidth, imagesCount, 
				VectorToColorRef(colorMask), IMAGE_BITMAP, flags);

			RequireNotNull(m_Handle, "Failed to create image list");
		}
		void LoadFromBitmap(const std::wstring& resourceName, const uInt& imagesWidth, const uInt& imagesCount, const Byte3D& colorMask) {
			Destroy();

			m_Handle = ImageList_LoadBitmap(
				nullptr, resourceName.c_str(), imagesWidth, imagesCount, 
				VectorToColorRef(colorMask));

			RequireNotNull(m_Handle, "Failed to create image list");
		}

		Bool Copy(ImageList* pImageList, const Int& destImageIndex, const Int& sourceImageIndex, const Bool& isSwap) {
			if (m_Handle == nullptr)
				return false;

			RequireNotNull(RequireNotNull(pImageList)->m_Handle);			
			return ImageList_Copy(m_Handle, destImageIndex, pImageList->m_Handle, sourceImageIndex, (isSwap) ? ILCF_SWAP : ILCF_MOVE);
		}
		ImageList Merge(const Int& imageIndex_1, ImageList* pImageList, const Int& imageIndex_2, const Int2D& offset) const {
			RequireNotNull(m_Handle, "ImageList not created");
			RequireNotNull(RequireNotNull(pImageList)->m_Handle);

			HIMAGELIST hImageList = ImageList_Merge(
				m_Handle, imageIndex_1, pImageList->m_Handle, imageIndex_2, offset.x, offset.y);

			return RequireNotNull(hImageList, "Failed to merge ImageList");
		}
		ImageList Duplicate() const {
			RequireNotNull(m_Handle, "ImageList not created");

			return RequireNotNull(ImageList_Duplicate(m_Handle), "Failed to duplicate ImageList");
		}

		Int Add(const GDI::Bitmap* pBitmap, const GDI::Bitmap* pBitmapMask = nullptr) {
			RequireNotNull(m_Handle, "ImageList not created");
			RequireNotNull(pBitmap);

			if (pBitmapMask != nullptr)
				return ImageList_Add(m_Handle, pBitmap->GetHandle(), pBitmapMask->GetHandle());
			return ImageList_Add(m_Handle, pBitmap->GetHandle(), nullptr);
		}
		Bool Remove(const Int& index) {
			return ImageList_Remove(RequireNotNull(m_Handle, "ImageList not created"), index);
		}
		Bool Clear() {
			return ImageList_RemoveAll(RequireNotNull(m_Handle, "ImageList not created"));
		}
		Bool Replace(const uInt& index, const GDI::Bitmap* pBitmap, const GDI::Bitmap* pBitmapMask = nullptr) {
			RequireNotNull(m_Handle, "ImageList not created");
			RequireNotNull(pBitmap);

			if (pBitmapMask != nullptr)
				return ImageList_Replace(m_Handle, index, pBitmap->GetHandle(), pBitmapMask->GetHandle());
			return ImageList_Replace(m_Handle, index, pBitmap->GetHandle(), nullptr);
		}

		Int AddIcon(const Icon& icon) {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_AddIcon(m_Handle, icon.GetHandle());
		}
		Int ReplaceIcon(const Int& index, const Icon& icon) {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_ReplaceIcon(m_Handle, index, icon.GetHandle());
		}

		Int AddMasked(const GDI::Bitmap& bitmap, const Byte3D& mask) {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_AddMasked(m_Handle, bitmap.GetHandle(), VectorToColorRef(mask));
		}

		Bool Draw(const HandleWindow& window, const uInt& imageIndex, const Int2D& position, const DrawStyles& styles) const {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_Draw(m_Handle, imageIndex, window.GetDC(), position.x, position.y, styles);
		}
		Bool DrawEx(const HandleWindow& window, const uInt& imageIndex, const IntRect& rect, const Byte3D& background, const Byte3D& foreground, const DrawStyles& styles) const {
			RequireNotNull(m_Handle, "ImageList not created");

			return ImageList_DrawEx(m_Handle, imageIndex, window.GetDC(),
				rect.Left, rect.Top, rect.Right, rect.Bottom,
				VectorToColorRef(background), VectorToColorRef(foreground), styles);
		}
		Bool DrawIndirect(const HandleWindow& window, const DrawParams& drawParams) const {
			RequireNotNull(m_Handle, "ImageList not created");

			IMAGELISTDRAWPARAMS params;
			params.cbSize = sizeof(params);
			params.himl = m_Handle;
			params.i = drawParams.ImageIndex;
			params.hdcDst = window.GetDC();
			params.x = drawParams.Position.x;
			params.y = drawParams.Position.y;
			params.cx = drawParams.CropRect.Right;
			params.cy = drawParams.CropRect.Bottom;
			params.xBitmap = drawParams.CropRect.Left;
			params.yBitmap = drawParams.CropRect.Right;
			params.rgbBk = VectorToColorRef(drawParams.BackgroundColor);
			params.rgbFg = VectorToColorRef(drawParams.ForegroundColor);
			params.fStyle = drawParams.Styles;
			params.dwRop = drawParams.RasterOperation;
			params.fState = drawParams.State;
			params.Frame = drawParams.Alpha;
			params.crEffect = drawParams.GlowAndShadowEffect;

			return ImageList_DrawIndirect(&params);
		}

		Bool BeginDrag(const Int& imageIndex, const Int2D& hotspot) const {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_BeginDrag(m_Handle, imageIndex, hotspot.x, hotspot.y);
		}
		Bool DragEnter(const HandleWindow& window, const Int2D& position) const noexcept {
			return ImageList_DragEnter(window.GetHandle(), position.x, position.y);
		}
		Bool DragLeave(const HandleWindow& window) const noexcept {
			return ImageList_DragLeave(window.GetHandle());
		}
		Bool DragMove(const Int2D& position) noexcept {
			return ImageList_DragMove(position.x, position.y);
		}
		Bool DragShowNolock(const Bool& isShow) {
			return ImageList_DragShowNolock(isShow);
		}
		void EndDrag() const noexcept {
			return ImageList_EndDrag();
		}

		Int GetImageCount() const {
			return ImageList_GetImageCount(RequireNotNull(m_Handle, "ImageList not created"));
		}
		Byte3D GetBackgroundColor() const {
			RequireNotNull(m_Handle, "ImageList not created");
			return ColorRefToVector(ImageList_GetBkColor(m_Handle));
		}
		Icon GetIcon(const Int& index, const DrawFlags& flags) const {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_GetIcon(m_Handle, index, flags);
		}
		ImageList GetDragImage(Int2D& dragPosition, Int2D& hotspot) const noexcept {
			POINT dragPoint = dragPosition;
			POINT hotspotPoint = hotspot;
			
			ImageList imageList = ImageList_GetDragImage(&dragPoint, &hotspotPoint);
			dragPosition = dragPoint;
			hotspot = hotspotPoint;

			return imageList;
		}
		Bool GetIconSize(Int2D& size) const {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_GetIconSize(m_Handle, &size.x, &size.y);
		}
		Info GetImageInfo(const Int& imageIndex) const {
			RequireNotNull(m_Handle, "ImageList not created");

			IMAGEINFO imageInfo;
			if (!ImageList_GetImageInfo(m_Handle, imageIndex, &imageInfo))
				Raise("Failed to get info");

			Info info = {
				imageInfo.hbmImage,
				imageInfo.hbmMask,
				imageInfo.rcImage
			};

			return info;
		}
		HIMAGELIST GetHandle() const noexcept {
			return m_Handle;
		}

		Bool SetImageCount(const uInt& newCount) {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_SetImageCount(m_Handle, newCount);
		}
		Byte3D SetBackgroundColor(const Byte3D& newColor) {
			return ColorRefToVector(ImageList_SetBkColor(m_Handle, VectorToColorRef(newColor)));
		}
		Bool SetOverlayImage(const Int& imageIndex, const Int& overlay) {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_SetOverlayImage(m_Handle, imageIndex, overlay);
		}
		Bool SetDragCursorImage(const Int& imageIndex, const Int2D& hotspot) {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_SetDragCursorImage(m_Handle, imageIndex, hotspot.x, hotspot.y);
		}
		Bool SetIconSize(const uInt2D& size) {
			RequireNotNull(m_Handle, "ImageList not created");
			return ImageList_SetIconSize(m_Handle, size.x, size.y);
		}

	private:
		HIMAGELIST m_Handle = nullptr;
		HINSTANCE m_hInstance = GetModuleHandle(nullptr);
	};
}