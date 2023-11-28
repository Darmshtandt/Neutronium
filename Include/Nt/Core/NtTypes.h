#pragma once

namespace Nt {
	template<typename> struct Vector2D;
	template<typename> struct Vector3D;
	template<typename> struct Vector4D;
	template<typename> struct Rect;

// ============================================================================
//		Base
// ----------------------------------------------------------------------------
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
	
// ============================================================================
//		Vector2D
// ----------------------------------------------------------------------------
	using Float2D = Vector2D<Float>;
	using Double2D = Vector2D<Double>;
	using LDouble2D = Vector2D<LDouble>;

	using Byte2D = Vector2D<Byte>;
	using Word2D = Vector2D<Word>;
	using DWord2D = Vector2D<DWord>;
	using QWord2D = Vector2D<QWord>;

	using Bool2D = Vector2D<Bool>;
	using Char2D = Vector2D<Char>;
	using Short2D = Vector2D<Short>;
	using Int2D = Vector2D<Int>;
	using Long2D = Vector2D<Long>;
	using LLong2D = Vector2D<LLong>;

	using uChar2D = Vector2D<uChar>;
	using uShort2D = Vector2D<uShort>;
	using uInt2D = Vector2D<uInt>;
	using uLong2D = Vector2D<uLong>;
	using uLLong2D = Vector2D<uLLong>;

	using FloatPtr2D = Vector2D<Float*>;
	using DoublePtr2D = Vector2D<Double*>;
	using LDoublePtr2D = Vector2D<LDouble*>;

	using BoolPtr2D = Vector2D<Bool*>;
	using CharPtr2D = Vector2D<Char*>;
	using ShortPtr2D = Vector2D<Short*>;
	using IntPtr2D = Vector2D<Int*>;
	using LongPtr2D = Vector2D<Long*>;
	using LLongPtr2D = Vector2D<LLong*>;

	using uCharPtr2D = Vector2D<uChar*>;
	using uShortPtr2D = Vector2D<uShort*>;
	using uIntPtr2D = Vector2D<uInt*>;
	using uLongPtr2D = Vector2D<uLong*>;
	using uLLongPtr2D = Vector2D<uLLong*>;
	
// ============================================================================
//		Vector3D
// ----------------------------------------------------------------------------
	using Float3D = Vector3D<Float>;
	using Double3D = Vector3D<Double>;
	using LDouble3D = Vector3D<LDouble>;

	using Byte3D = Vector3D<Byte>;
	using Word3D = Vector3D<Word>;
	using DWord3D = Vector3D<DWord>;
	using QWord3D = Vector3D<QWord>;

	using Bool3D = Vector3D<Bool>;
	using Char3D = Vector3D<Char>;
	using Short3D = Vector3D<Short>;
	using Int3D = Vector3D<Int>;
	using Long3D = Vector3D<Long>;
	using LLong3D = Vector3D<LLong>;

	using uChar3D = Vector3D<uChar>;
	using uShort3D = Vector3D<uShort>;
	using uInt3D = Vector3D<uInt>;
	using uLong3D = Vector3D<uLong>;
	using uLLong3D = Vector3D<uLLong>;

	using FloatPtr3D = Vector3D<Float*>;
	using DoublePtr3D = Vector3D<Double*>;
	using LDoublePtr3D = Vector3D<LDouble*>;

	using BoolPtr3D = Vector3D<Bool*>;
	using CharPtr3D = Vector3D<Char*>;
	using ShortPtr3D = Vector3D<Short*>;
	using IntPtr3D = Vector3D<Int*>;
	using LongPtr3D = Vector3D<Long*>;
	using LLongPtr3D = Vector3D<LLong*>;

	using uCharPtr3D = Vector3D<uChar*>;
	using uShortPtr3D = Vector3D<uShort*>;
	using uIntPtr3D = Vector3D<uInt*>;
	using uLongPtr3D = Vector3D<uLong*>;
	using uLLongPtr3D = Vector3D<uLLong*>;


// ============================================================================
//		Vector4D
// ----------------------------------------------------------------------------
	using Float4D = Vector4D<Float>;
	using Double4D = Vector4D<Double>;
	using LDouble4D = Vector4D<LDouble>;

	using Byte4D = Vector4D<Byte>;
	using Word4D = Vector4D<Word>;
	using DWord4D = Vector4D<DWord>;
	using QWord4D = Vector4D<QWord>;

	using Bool4D = Vector4D<Bool>;
	using Char4D = Vector4D<Char>;
	using Short4D = Vector4D<Short>;
	using Int4D = Vector4D<Int>;
	using Long4D = Vector4D<Long>;
	using LLong4D = Vector4D<LLong>;

	using uChar4D = Vector4D<uChar>;
	using uShort4D = Vector4D<uShort>;
	using uInt4D = Vector4D<uInt>;
	using uLong4D = Vector4D<uLong>;
	using uLLong4D = Vector4D<uLLong>;

	using FloatPtr4D = Vector4D<Float*>;
	using DoublePtr4D = Vector4D<Double*>;
	using LDoublePtr4D = Vector4D<LDouble*>;

	using BoolPtr4D = Vector4D<Bool*>;
	using CharPtr4D = Vector4D<Char*>;
	using ShortPtr4D = Vector4D<Short*>;
	using IntPtr4D = Vector4D<Int*>;
	using LongPtr4D = Vector4D<Long*>;
	using LLongPtr4D = Vector4D<LLong*>;

	using uCharPtr4D = Vector4D<uChar*>;
	using uShortPtr4D = Vector4D<uShort*>;
	using uIntPtr4D = Vector4D<uInt*>;
	using uLongPtr4D = Vector4D<uLong*>;
	using uLLongPtr4D = Vector4D<uLLong*>;


// ============================================================================
//		Rect
// ----------------------------------------------------------------------------
	using FloatRect = Rect<Float>;
	using DoubleRect = Rect<Double>;
	using LDoubleRect = Rect<LDouble>;

	using ByteRect = Rect<Byte>;
	using WordRect = Rect<Word>;
	using DWordRect = Rect<DWord>;
	using QWordRect = Rect<QWord>;

	using BoolRect = Rect<Bool>;
	using CharRect = Rect<Char>;
	using ShortRect = Rect<Short>;
	using IntRect = Rect<Int>;
	using LongRect = Rect<Long>;
	using LLongRect = Rect<LLong>;

	using uCharRect = Rect<uChar>;
	using uShortRect = Rect<uShort>;
	using uIntRect = Rect<uInt>;
	using uLongRect = Rect<uLong>;
	using uLLongRect = Rect<uLLong>;

	using FloatPtrRect = Rect<Float*>;
	using DoublePtrRect = Rect<Double*>;
	using LDoublePtrRect = Rect<LDouble*>;

	using BoolPtrRect = Rect<Bool*>;
	using CharPtrRect = Rect<Char*>;
	using ShortPtrRect = Rect<Short*>;
	using IntPtrRect = Rect<Int*>;
	using LongPtrRect = Rect<Long*>;
	using LLongPtrRect = Rect<LLong*>;

	using uCharPtrRect = Rect<uChar*>;
	using uShortPtrRect = Rect<uShort*>;
	using uIntPtrRect = Rect<uInt*>;
	using uLongPtrRect = Rect<uLong*>;
	using uLLongPtrRect = Rect<uLLong*>;
}