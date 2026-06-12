#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOMSG

#include <windows.h>

#include <Nt/Graphics/System/WindowElements/ImageList.h>

namespace Nt {
	ImageList::ImageList(const HIMAGELIST& list) :
		m_Handle(list) {

	}
	ImageList::~ImageList() {
		Destroy();
	}

	void ImageList::Create(const Int2D& imagesSize, const CreateFlags& flags, const uInt& initialCount, const uInt& growCount) {
		m_Handle = ImageList_Create(imagesSize.x, imagesSize.y, flags, initialCount, growCount);
		RequireNotNull(m_Handle, "Failed to create image list");
	}

	Bool ImageList::Destroy() {
		if (m_Handle == nullptr)
			return true;

		const Bool result = ImageList_Destroy(m_Handle);
		m_Handle = nullptr;
		return result;
	}

	void ImageList::Load(const std::wstring& resourceName, const uInt& imagesWidth, const uInt& imagesCount, const Byte3D& colorMask, const ResourceFlags& flags) {
		m_Handle = ImageList_LoadImage(
			nullptr, resourceName.c_str(), imagesWidth, imagesCount,
			VectorToColorRef(colorMask), IMAGE_BITMAP, flags);

		RequireNotNull(m_Handle, "Failed to create image list");
	}

	void ImageList::LoadFromBitmap(const std::wstring& resourceName, const uInt& imagesWidth, const uInt& imagesCount, const Byte3D& colorMask) {
		Destroy();

		m_Handle = ImageList_LoadBitmap(
			nullptr, resourceName.c_str(), imagesWidth, imagesCount,
			VectorToColorRef(colorMask));

		RequireNotNull(m_Handle, "Failed to create image list");
	}

	Bool ImageList::Copy(ImageList* pImageList, const Int& destImageIndex, const Int& sourceImageIndex, const Bool& isSwap) {
		if (m_Handle == nullptr)
			return false;

		RequireNotNull(RequireNotNull(pImageList)->m_Handle);
		return ImageList_Copy(m_Handle, destImageIndex, pImageList->m_Handle, sourceImageIndex, (isSwap) ? ILCF_SWAP : ILCF_MOVE);
	}

	ImageList ImageList::Merge(const Int& imageIndex_1, ImageList* pImageList, const Int& imageIndex_2, const Int2D& offset) const {
		RequireNotNull(m_Handle, "ImageList not created");
		RequireNotNull(RequireNotNull(pImageList)->m_Handle);

		HIMAGELIST hImageList = ImageList_Merge(
			m_Handle, imageIndex_1, pImageList->m_Handle, imageIndex_2, offset.x, offset.y);

		return RequireNotNull(hImageList, "Failed to merge ImageList");
	}

	ImageList ImageList::Duplicate() const {
		RequireNotNull(m_Handle, "ImageList not created");

		return RequireNotNull(ImageList_Duplicate(m_Handle), "Failed to duplicate ImageList");
	}

	Int ImageList::Add(const GDI::Bitmap* pBitmap, const GDI::Bitmap* pBitmapMask /*= nullptr*/) {
		RequireNotNull(m_Handle, "ImageList not created");
		RequireNotNull(pBitmap);

		if (pBitmapMask != nullptr)
			return ImageList_Add(m_Handle, pBitmap->GetHandle(), pBitmapMask->GetHandle());
		return ImageList_Add(m_Handle, pBitmap->GetHandle(), nullptr);
	}

	Bool ImageList::Remove(const Int& index) {
		return ImageList_Remove(RequireNotNull(m_Handle, "ImageList not created"), index);
	}

	Bool ImageList::Clear() {
		return ImageList_RemoveAll(RequireNotNull(m_Handle, "ImageList not created"));
	}

	Bool ImageList::Replace(const uInt& index, const GDI::Bitmap* pBitmap, const GDI::Bitmap* pBitmapMask /*= nullptr*/) {
		RequireNotNull(m_Handle, "ImageList not created");
		RequireNotNull(pBitmap);

		if (pBitmapMask != nullptr)
			return ImageList_Replace(m_Handle, index, pBitmap->GetHandle(), pBitmapMask->GetHandle());
		return ImageList_Replace(m_Handle, index, pBitmap->GetHandle(), nullptr);
	}

	Int ImageList::AddIcon(const Icon& icon) {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_AddIcon(m_Handle, icon.GetHandle());
	}

	Int ImageList::ReplaceIcon(const Int& index, const Icon& icon) {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_ReplaceIcon(m_Handle, index, icon.GetHandle());
	}

	Int ImageList::AddMasked(const GDI::Bitmap& bitmap, const Byte3D& mask) {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_AddMasked(m_Handle, bitmap.GetHandle(), VectorToColorRef(mask));
	}

	Bool ImageList::Draw(const HandleWindow& window, const uInt& imageIndex, const Int2D& position, const DrawStyles& styles) const {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_Draw(m_Handle, imageIndex, window.GetDC(), position.x, position.y, styles);
	}

	Bool ImageList::DrawEx(const HandleWindow& window, const uInt& imageIndex, const IntRect& rect, const Byte3D& background, const Byte3D& foreground, const DrawStyles& styles) const {
		RequireNotNull(m_Handle, "ImageList not created");

		return ImageList_DrawEx(m_Handle, imageIndex, window.GetDC(),
			rect.Left, rect.Top, rect.Right, rect.Bottom,
			VectorToColorRef(background), VectorToColorRef(foreground), styles);
	}

	Bool ImageList::DrawIndirect(const HandleWindow& window, const DrawParams& drawParams) const {
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

	Bool ImageList::BeginDrag(const Int& imageIndex, const Int2D& hotspot) const {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_BeginDrag(m_Handle, imageIndex, hotspot.x, hotspot.y);
	}

	Bool ImageList::DragEnter(const HandleWindow& window, const Int2D& position) const noexcept {
		return ImageList_DragEnter(window.GetHandle(), position.x, position.y);
	}

	Bool ImageList::DragLeave(const HandleWindow& window) const noexcept {
		return ImageList_DragLeave(window.GetHandle());
	}

	Bool ImageList::DragMove(const Int2D& position) noexcept {
		return ImageList_DragMove(position.x, position.y);
	}

	Bool ImageList::DragShowNolock(const Bool& isShow) {
		return ImageList_DragShowNolock(isShow);
	}

	void ImageList::EndDrag() const noexcept {
		return ImageList_EndDrag();
	}

	Int ImageList::GetImageCount() const {
		return ImageList_GetImageCount(RequireNotNull(m_Handle, "ImageList not created"));
	}

	Nt::Byte3D ImageList::GetBackgroundColor() const {
		RequireNotNull(m_Handle, "ImageList not created");
		return ColorRefToVector(ImageList_GetBkColor(m_Handle));
	}

	Nt::Icon ImageList::GetIcon(const Int& index, const DrawFlags& flags) const {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_GetIcon(m_Handle, index, flags);
	}

	ImageList ImageList::GetDragImage(Int2D& dragPosition, Int2D& hotspot) const noexcept {
		POINT dragPoint = dragPosition;
		POINT hotspotPoint = hotspot;

		ImageList imageList = ImageList_GetDragImage(&dragPoint, &hotspotPoint);
		dragPosition = dragPoint;
		hotspot = hotspotPoint;

		return imageList;
	}

	Bool ImageList::GetIconSize(Int2D& size) const {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_GetIconSize(m_Handle, &size.x, &size.y);
	}

	Nt::ImageList::Info ImageList::GetImageInfo(const Int& imageIndex) const {
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

	HIMAGELIST ImageList::GetHandle() const noexcept {
		return m_Handle;
	}

	Bool ImageList::SetImageCount(const uInt& newCount) {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_SetImageCount(m_Handle, newCount);
	}

	Nt::Byte3D ImageList::SetBackgroundColor(const Byte3D& newColor) {
		return ColorRefToVector(ImageList_SetBkColor(m_Handle, VectorToColorRef(newColor)));
	}

	Bool ImageList::SetOverlayImage(const Int& imageIndex, const Int& overlay) {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_SetOverlayImage(m_Handle, imageIndex, overlay);
	}

	Bool ImageList::SetDragCursorImage(const Int& imageIndex, const Int2D& hotspot) {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_SetDragCursorImage(m_Handle, imageIndex, hotspot.x, hotspot.y);
	}

	Bool ImageList::SetIconSize(const uInt2D& size) {
		RequireNotNull(m_Handle, "ImageList not created");
		return ImageList_SetIconSize(m_Handle, size.x, size.y);
	}

}