#pragma once
#ifndef _WINDEF_

#include <Nt/Core/NtTypes.h>

#define CALLBACK    __stdcall

using HGLRC = void*;
using HWND = void*;
using HMENU = void*;
using HINSTANCE = void*;
using HRGN = void*;
using HDC = void*;

using LRESULT = Long;
using WPARAM = Long;
using LPARAM = Long;

struct RECT {
	Long left;
	Long top;
	Long right;
	Long bottom;
};

struct POINT {
	Long x;
	Long y;
};

struct CREATESTRUCT {
	void* lpCreateParams;
	HINSTANCE hInstance;
	HMENU hMenu;
	HWND hwndParent;
	Int cy;
	Int cx;
	Int y;
	Int x;
	Long style;
	wChar* lpszName;
	wChar* lpszClass;
	DWord dwExStyle;
};

struct PAINTSTRUCT {
	HDC hdc;
	Bool fErase;
	RECT rcPaint;
	Bool fRestore;
	Bool fIncUpdate;
	Byte rgbReserved[32];
};

struct MSG {
	HWND        hwnd;
	uInt        message;
	WPARAM      wParam;
	LPARAM      lParam;
	DWord       time;
	POINT       pt;
#ifdef _MAC
	DWord       lPrivate;
#endif
};

#endif