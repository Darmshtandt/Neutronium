#pragma once

#ifdef _WINDEF_
using WindowID = HWND;
using ContextID = HGLRC;
using DescriptorID = HDC;
#else

struct WindowID__ {
	int unused;
};
struct ContextID__ {
	int unused;
};
struct DescriptorID__ {
	int unused;
};

using WindowID = WindowID__*;
using ContextID = ContextID__*;
using DescriptorID = DescriptorID__*;
#endif