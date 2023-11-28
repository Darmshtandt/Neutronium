#pragma once

// ============================================================================
//	Defines
// ----------------------------------------------------------------------------
#ifndef GLEW_STATIC
#	define GLEW_STATIC
#endif

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif

#ifndef UNICODE
#	define UNICODE
#endif

// ============================================================================
//	System
// ----------------------------------------------------------------------------
#include <Windows.h>
#include <commdlg.h>
#include <CommCtrl.h>
#include <GL\GLEW.h>
#include <GL\GL.h>
#include <GL\GLU.h>

// ============================================================================
//	STL and C
// ----------------------------------------------------------------------------
#include <functional>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <iterator>
#include <queue>
#include <list>
#include <set>

#include <cassert>
#include <cmath>

#pragma warning(disable : 4996)