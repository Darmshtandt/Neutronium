#pragma once

#ifdef _WINDEF_
#include <Nt/Core/Math/Rect.h>

namespace Nt::GDI {
	HBITMAP CreateCompatibleBitmap(const HDC& hdc, const uInt2D& size);
	HBITMAP CopyHandleBitmap(const HBITMAP& hBitmap);

	HDC CreateCompatibleDC(const HDC& hdc);
	HGDIOBJ SelectObject(const HDC& hdc, const HGDIOBJ& hObject);
	void GetDIBits(const HDC& hdc, const HBITMAP& hBitmap, const uInt& start, 
			const uInt& height, void* pBits, BITMAPINFO* pBmpInfo, const uInt& usage);

	Int SetStretchBltMode(const HDC& hdc, const Int& mode);
	Bool StretchBlt(const HDC& hdcDest, const IntRect& rectDest, 
			const HDC& hdcSrc, const IntRect& rectSrc, const DWord& rop);

	HBITMAP ScaleHBitmap(const HDC& hdc, const HBITMAP& hBitmap, 
			const IntRect& bitmapRect, const IntRect& scaleRect);

	void RenderHBitmap(HDC hdc, HBITMAP hBitmap, const IntRect& scaleRect = { });
}
#endif