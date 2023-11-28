#pragma once

namespace Nt::GDI {
	__inline HBITMAP CreateCompatibleBitmap(const HDC& hdc, const uInt2D& size) {
		if (hdc == nullptr)
			Raise("HDC is nullptr");

		if (size.x == 0 && size.y == 0)
			Log::Warning("Size = 0");
		else if (size.x == 0)
			Log::Warning("Width = 0");
		else if (size.y == 0)
			Log::Warning("Height = 0");

		HBITMAP compabilleBitmap = ::CreateCompatibleBitmap(hdc, size.x, size.y);
		if (compabilleBitmap == nullptr)
			Raise(String("Failed to create compabile handle bitmap.\nError code:") + String(GetLastError()));
		return compabilleBitmap;
	}

	__inline HDC CreateCompatibleDC(const HDC& hdc) {
		if (hdc == nullptr)
			Raise("HDC is nullptr");

		HDC compabilleDC = ::CreateCompatibleDC(hdc);
		if (compabilleDC == nullptr)
			Raise(String("Failed to create compabile HDC.\nError code:") + String(GetLastError()));
		return compabilleDC;
	}
	__inline HGDIOBJ SelectObject(const HDC& hdc, const HGDIOBJ& hObject) {
		if (hdc == nullptr)
			Raise("HDC is nullptr");
		if (hObject == nullptr)
			Raise("hObject is nullptr");

		HGDIOBJ hGDIObj = ::SelectObject(hdc, hObject);
		if (hGDIObj == nullptr)
			Raise(String("Failed to select object.\nError code:") + String(GetLastError()));
		return hGDIObj;
	}
	__inline void GetDIBits(const HDC& hdc, const HBITMAP& hBitmap, const uInt& start, 
			const uInt& height, void* pBits, BITMAPINFO* pBmpInfo, const uInt& usage) {
		if (hdc == nullptr)
			Raise("HDC is nullptr");
		if (pBits == nullptr)
			Raise("pBits is nullptr");
		if (pBmpInfo == nullptr)
			Raise("pBmpInfo is nullptr");
		if (height == 0)
			Log::Warning("Height = 0");

		const Bool result = ::GetDIBits(hdc, hBitmap, start, height, pBits, pBmpInfo, usage);
		if (!result)
			Raise(String("Failed to get bitmap bits.\nError code: ") + String(GetLastError()));
	}

	__inline Int SetStretchBltMode(const HDC& hdc, const Int& mode) {
		if (hdc == nullptr)
			Raise("HDC is nullptr");

		const Int result = ::SetStretchBltMode(hdc, HALFTONE);
		if (result == 0)
			Raise(String("Failed to set StretchBlt mode.\nError code: ") + String(GetLastError()));
		return result;
	}
	__inline Bool StretchBlt(const HDC& hdcDest, const IntRect& rectDest, 
			const HDC& hdcSrc, const IntRect& rectSrc, const DWord& rop) {
		if (hdcDest == nullptr)
			Raise("hdcDest is nullptr");
		if (hdcSrc == nullptr)
			Raise("hdcSrc is nullptr");

		if (rectDest.Right == 0 && rectDest.Bottom == 0)
			Log::Warning("Dest size = 0");
		else if (rectDest.Right == 0)
			Log::Warning("Dest width = 0");
		else if (rectDest.Bottom == 0)
			Log::Warning("Dest height = 0");

		if (rectSrc.Right == 0 && rectSrc.Bottom == 0)
			Log::Warning("Source size = 0");
		else if (rectSrc.Right == 0)
			Log::Warning("Source width = 0");
		else if (rectSrc.Bottom == 0)
			Log::Warning("Source height = 0");

		SetLastError(0);
		const Bool result = ::StretchBlt(hdcDest, rectDest.Left, rectDest.Top,
			rectDest.Right, rectDest.Bottom, hdcSrc, 
			rectSrc.Left, rectSrc.Top, rectSrc.Right, rectSrc.Bottom, rop);
		if (result && GetLastError() > 0)
			Raise(String("Failed to draw bitmap.\nError code: ") + String(GetLastError()));
		return result;
	}

	__inline HBITMAP ScaleHBitmap(const HDC& hdc, const HBITMAP& hBitmap, 
			const IntRect& bitmapRect, const IntRect& scaleRect) {
		if (hdc == nullptr)
			Raise("HDC is nullptr");

		HDC hdcDest = GDI::CreateCompatibleDC(hdc);
		HBITMAP hBitmapDest =
			GDI::CreateCompatibleBitmap(hdc, scaleRect.RightBottom);
		HBITMAP hOldBitmapDest = (HBITMAP)GDI::SelectObject(hdcDest, hBitmapDest);

		HDC hdcSrc = GDI::CreateCompatibleDC(hdc);
		HBITMAP hOldBitmapSrc = (HBITMAP)GDI::SelectObject(hdcSrc, hBitmap);

		GDI::SetStretchBltMode(hdcDest, HALFTONE);
		GDI::StretchBlt(hdcDest, scaleRect, hdcSrc, bitmapRect, SRCCOPY);

		::SelectObject(hdcSrc, hOldBitmapSrc);
		::SelectObject(hdcSrc, hOldBitmapSrc);
		DeleteDC(hdcSrc);

		::SelectObject(hdcDest, hOldBitmapDest);
		::SelectObject(hdcDest, hOldBitmapDest);
		DeleteDC(hdcDest);
		return hBitmapDest;

	}

	__inline void RenderHBitmap(HDC hdc, HBITMAP hBitmap, const IntRect& scaleRect = { }) {
		HDC compabilleDC = GDI::CreateCompatibleDC(hdc);
		HBITMAP hBitmapOld = (HBITMAP)GDI::SelectObject(compabilleDC, hBitmap);

		BITMAP bitmap;
		GetObject(hBitmap, sizeof(bitmap), &bitmap);

		BITMAPINFO bmpInfo = { };
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = bitmap.bmWidth;
		bmpInfo.bmiHeader.biHeight = bitmap.bmHeight;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 32;
		bmpInfo.bmiHeader.biCompression = BI_RGB;

		if (bitmap.bmBits == nullptr) {
			GDI::GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, nullptr, &bmpInfo, DIB_RGB_COLORS);
			bitmap.bmBits = new BYTE[bmpInfo.bmiHeader.biSizeImage];
		}
		GDI::GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, bitmap.bmBits, &bmpInfo, DIB_RGB_COLORS);

		GDI::SetStretchBltMode(hdc, HALFTONE);
		GDI::StretchBlt(hdc, scaleRect, compabilleDC,
			{ 0, 0, bitmap.bmWidth, bitmap.bmHeight }, SRCCOPY);

		::SelectObject(compabilleDC, hBitmapOld);
		DeleteDC(compabilleDC);
	}
}