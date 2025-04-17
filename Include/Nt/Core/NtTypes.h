#pragma once

using sChar = signed char;
using Char = char;
using wChar = wchar_t;
using Char16 = char16_t;
using Char32 = char32_t;

#	ifdef __cpp_char8_t
using Char8 = char8_t;
using cString8 = const Char8*;
#	endif

using cString = const Char*;
using cwString = const wChar*;
using cString32 = const Char16*;
using cString64 = const Char32*;

using Void = void;
using cVoid = const void;
using VoidPtr = void*;
using cVoidPtr = const void*;

using Bool = bool;
using Short = short;
using Int = int;
using Long = long;
using LLong = long long;

using uChar = unsigned char;
using uShort = unsigned short;
using uInt = unsigned int;
using uLong = unsigned long;
using uLLong = unsigned long long;

using Byte = uChar;
using Word = uShort;
using DWord = uLong;
using QWord = uLLong;

using Float = float;
using Double = double;
using LDouble = long double;