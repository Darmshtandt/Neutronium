#pragma once

namespace Nt::GDI {
	__inline HBITMAP CreateCompatibleBitmap(const HDC& hdc, const uInt2D& size) {
		if (size.x == 0 && size.y == 0)
			Log::Warning("Size = 0");
		else if (size.x == 0)
			Log::Warning("Width = 0");
		else if (size.y == 0)
			Log::Warning("Height = 0");

		HBITMAP compabilleBitmap = ::CreateCompatibleBitmap(RequireNotNull(hdc), size.x, size.y);
		if (compabilleBitmap == nullptr)
			Log::Warning(String("Failed to create compatible handle bitmap.\nError code:") + String(GetLastError()));
		return compabilleBitmap;
	}
	__inline HBITMAP CopyHandleBitmap(const HBITMAP& hBitmap) {
		BITMAP bitmap;
		if (GetObject(hBitmap, sizeof(bitmap), &bitmap) == 0)
			Raise("Failed to get bitmap object");

		HDC hdc = RequireNotNull(GetDC(nullptr));

		HBITMAP hNewBitmap = ::CreateCompatibleBitmap(hdc, bitmap.bmWidth, bitmap.bmHeight);
		if (hNewBitmap == nullptr) {
			ReleaseDC(nullptr, hdc);

			Raise("Failed to create new bitmap");
		}

		HDC hdcMem = ::CreateCompatibleDC(hdc);
		if (hdcMem == nullptr) {
			DeleteObject(hNewBitmap);
			ReleaseDC(nullptr, hdc);

			Raise("Failed to create compatible HDC");
		}

		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hdcMem, hNewBitmap);
		if (!BitBlt(hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdc, 0, 0, SRCCOPY)) {
			::SelectObject(hdcMem, hOldBitmap);
			DeleteObject(hdcMem);
			DeleteObject(hNewBitmap);
			ReleaseDC(nullptr, hdc);

			Raise("Failed to copy bitmap");
		}

		::SelectObject(hdcMem, hOldBitmap);

		DeleteObject(hdcMem);
		ReleaseDC(nullptr, hdc);

		return hNewBitmap;
	}

	__inline HDC CreateCompatibleDC(const HDC& hdc) {
		HDC compabilleDC = ::CreateCompatibleDC(RequireNotNull(hdc));
		if (compabilleDC == nullptr)
			Log::Warning(String("Failed to create compatible HDC.\nError code:") + String(GetLastError()));
		return compabilleDC;
	}
	__inline HGDIOBJ SelectObject(const HDC& hdc, const HGDIOBJ& hObject) {
		HGDIOBJ hGDIObj = ::SelectObject(RequireNotNull(hdc), RequireNotNull(hObject));
		if (hGDIObj == nullptr)
			Log::Warning(String("Failed to select object.\nError code:") + String(GetLastError()));
		return hGDIObj;
	}
	__inline void GetDIBits(const HDC& hdc, const HBITMAP& hBitmap, const uInt& start, 
			const uInt& height, void* pBits, BITMAPINFO* pBmpInfo, const uInt& usage) {
		RequireNotNull(hdc);
		RequireNotNull(pBits);
		RequireNotNull(pBmpInfo);

		if (height == 0) {
			Log::Warning("Height = 0");
			return;
		}

		const Bool result = ::GetDIBits(hdc, hBitmap, start, height, pBits, pBmpInfo, usage);
		if (!result)
			Log::Warning(String("Failed to get bitmap bits.\nError code: ") + String(GetLastError()));
	}

	__inline Int SetStretchBltMode(const HDC& hdc, const Int& mode) {
		const Int result = ::SetStretchBltMode(RequireNotNull(hdc), mode);
		if (result == 0)
			Log::Warning(String("Failed to set StretchBlt mode.\nError code: ") + String(GetLastError()));
		return result;
	}
	__inline Bool StretchBlt(const HDC& hdcDest, const IntRect& rectDest, 
			const HDC& hdcSrc, const IntRect& rectSrc, const DWord& rop) {
		RequireNotNull(hdcDest);
		RequireNotNull(hdcSrc);

		if (rectDest.Right == 0 && rectDest.Bottom == 0)
			Log::Warning("Destination size = 0");
		else if (rectDest.Right == 0)
			Log::Warning("Destination width = 0");
		else if (rectDest.Bottom == 0)
			Log::Warning("Destination height = 0");

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
			Log::Warning(String("Failed to draw bitmap.\nError code: ") + String(GetLastError()));
		return result;
	}

	__inline HBITMAP ScaleHBitmap(const HDC& hdc, const HBITMAP& hBitmap, 
			const IntRect& bitmapRect, const IntRect& scaleRect) {
		HDC hdcDest = GDI::CreateCompatibleDC(RequireNotNull(hdc));
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