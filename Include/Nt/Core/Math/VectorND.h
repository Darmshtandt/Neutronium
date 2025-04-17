#pragma once

#include <Nt/Core/NtTypes.h>

namespace Nt {
	//template <typename _Ty, uInt size>
	//struct VectorConstants1D {
	//	VectorConstants1D() = default;
	//	VectorConstants1D(const VectorConstants1D&) = delete;
	//	VectorConstants1D(VectorConstants1D&&) = delete;

	//	_CONSTEXPR20 static Vector<_Ty, size> Left = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(0, _Ty(-1));
	//	_CONSTEXPR20 static Vector<_Ty, size> Right = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(0, _Ty(-1));
	//};

	//template <typename _Ty, uInt size>
	//struct VectorConstants2D : public VectorConstants1D<_Ty, size> {
	//	VectorConstants2D() = default;
	//	VectorConstants2D(const VectorConstants2D&) = delete;
	//	VectorConstants2D(VectorConstants2D&&) = delete;

	//	_CONSTEXPR20 static Vector<_Ty, size> Up = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(1, _Ty(1));
	//	_CONSTEXPR20 static Vector<_Ty, size> Down = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(1, _Ty(-1));
	//};

	//template <typename _Ty, uInt size>
	//struct VectorConstants3D : public VectorConstants2D<_Ty, size> {
	//	VectorConstants3D() = default;
	//	VectorConstants3D(const VectorConstants3D&) = delete;
	//	VectorConstants3D(VectorConstants3D&&) = delete;

	//	_CONSTEXPR20 static Vector<_Ty, size> Forward = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(2, _Ty(1));
	//	_CONSTEXPR20 static Vector<_Ty, size> Back = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(2, _Ty(-1));
	//};

	template <typename _Ty> struct Vector3D;
	template <typename _Ty> struct Vector4D;

	template <typename _Ty>
	struct Vector2D : public Vector<_Ty, 2> {
		_CONSTEXPR20 Vector2D(const Vector<_Ty, 2>& vector) noexcept :
			Vector<_Ty, 2>(vector)
		{
		}
		_CONSTEXPR20 Vector2D(const _Ty& x = 0, const _Ty& y = 0) noexcept :
			Vector<_Ty, 2>({ x, y })
		{
		}
		_CONSTEXPR20 Vector2D(const std::initializer_list<_Ty>& list) noexcept :
			Vector<_Ty, 2>(list)
		{
		}
		_CONSTEXPR20 Vector2D(const Vector3D<_Ty>& vector3D) noexcept;
		_CONSTEXPR20 Vector2D(const Vector4D<_Ty>& vector4D) noexcept;

#ifdef _WINDEF_
		_CONSTEXPR20 Vector2D(const POINT& point) noexcept :
			Vector<_Ty, 2>({ point.x, point.y })
		{
		}
		_CONSTEXPR20 Vector2D(const SIZE& sizeStruct) noexcept :
			Vector<_Ty, 2>({ sizeStruct.cx, sizeStruct.cy })
		{
		}

		_CONSTEXPR20 operator POINT() const noexcept {
			return { Long(this->x), Long(this->y) };
		}
		_CONSTEXPR20 operator SIZE() const noexcept {
			return { Long(this->x), Long(this->y) };
		}
#endif
		
		_NODISCARD
		_CONSTEXPR20 _Ty GetCross(const Vector2D<_Ty>& vector) const noexcept {
			return (this->x * vector.y - this->y * vector.x);
		}

		template <typename _U>
		_NODISCARD _CONSTEXPR20 operator Vector2D<_U>() const noexcept {
			return this->operator Vector<_U, 2>();
		}
	};

	template <typename _Ty>
	struct Vector3D : public Vector<_Ty, 3> {
		_CONSTEXPR20 Vector3D(const Vector<_Ty, 3>& vector) noexcept :
			Vector<_Ty, 3>(vector)
		{
		}
		_CONSTEXPR20 Vector3D(const _Ty& x = 0, const _Ty& y = 0, const _Ty& z = 0) noexcept :
			Vector<_Ty, 3>({ x, y, z })
		{
		}
		_CONSTEXPR20 Vector3D(const Vector2D<_Ty>& xy, const _Ty& z = 0) noexcept :
			Vector<_Ty, 3>({ xy.x, xy.y, z })
		{
		}
		_CONSTEXPR20 Vector3D(const _Ty& x, const Vector2D<_Ty>& yz) noexcept :
			Vector<_Ty, 3>({ x, yz.y, yz.z })
		{
		}
		_CONSTEXPR20 Vector3D(const std::initializer_list<_Ty>& list) noexcept :
			Vector<_Ty, 3>(list)
		{
		}
		_CONSTEXPR20 Vector3D(const Vector4D<_Ty>& vector4D) noexcept;

		template <typename _U>
		_NODISCARD _CONSTEXPR20 operator Vector3D<_U>() const noexcept {
			return this->operator Vector<_U, 3>();
		}
	};

	template <typename _Ty>
	struct Vector4D : public Vector<_Ty, 4> {
		_CONSTEXPR20 Vector4D(const Vector<_Ty, 4>& vector) noexcept :
			Vector<_Ty, 4>(vector)
		{
		}
		_CONSTEXPR20 Vector4D(const _Ty& x = 0, const _Ty& y = 0, const _Ty& z = 0, const _Ty& w = 0) noexcept :
			Vector<_Ty, 4>({ x, y, z, w })
		{
		}
		_CONSTEXPR20 Vector4D(const Vector2D<_Ty>& xy, const _Ty& z = 0, const _Ty& w = 0) noexcept :
			Vector<_Ty, 4>({ xy.x, xy.y, z, w })
		{
		}
		_CONSTEXPR20 Vector4D(const _Ty& x, const Vector2D<_Ty>& yz, const _Ty& w = 0) noexcept :
			Vector<_Ty, 4>({ x, yz.y, yz.z, w })
		{
		}
		_CONSTEXPR20 Vector4D(const _Ty& x, const _Ty& y, const Vector2D<_Ty>& zw) noexcept :
			Vector<_Ty, 4>({ x, y, zw.z, zw.w })
		{
		}
		_CONSTEXPR20 Vector4D(const Vector2D<_Ty>& xy, const Vector2D<_Ty>& zw) noexcept :
			Vector<_Ty, 4>({ xy.x, xy.y, zw.z, zw.w })
		{
		}

		_CONSTEXPR20 Vector4D(const Vector3D<_Ty>& xyz, const _Ty& w = 0) noexcept :
			Vector<_Ty, 4>({ xyz.x, xyz.y, xyz.z, w })
		{
		}
		_CONSTEXPR20 Vector4D(const _Ty& x, const Vector3D<_Ty>& yzw) noexcept :
			Vector<_Ty, 4>({ x, yzw.y, yzw.z, yzw.w })
		{
		}
		_CONSTEXPR20 Vector4D(const std::initializer_list<_Ty>& list) noexcept :
			Vector<_Ty, 4>(list)
		{
		}

		template <typename _U>
		_NODISCARD _CONSTEXPR20 operator Vector4D<_U>() const noexcept {
			return this->operator Vector<_U, 4>();
		}
	};


	template <typename _Ty>
	struct is_Vector : std::false_type
	{
	};

	template <typename _Ty>
	struct is_Vector<Vector2D<_Ty>> : std::true_type
	{
	};

	template <typename _Ty>
	struct is_Vector<Vector3D<_Ty>> : std::true_type
	{
	};

	template <typename _Ty>
	struct is_Vector<Vector4D<_Ty>> : std::true_type
	{
	};


	template <typename _Ty>
	_CONSTEXPR20 Bool is_Vector_v = is_Vector<_Ty>::value;


	template<typename _Ty>
	inline _CONSTEXPR20 Vector2D<_Ty>::Vector2D(const Vector3D<_Ty>& vector3D) noexcept :
		Vector<_Ty, 2>({ vector3D.x, vector3D.y })
	{
	}
	template<typename _Ty>
	inline _CONSTEXPR20 Vector2D<_Ty>::Vector2D(const Vector4D<_Ty>& vector4D) noexcept :
		Vector<_Ty, 2>({ vector4D.x, vector4D.y })
	{
	}
	template<typename _Ty>
	inline _CONSTEXPR20 Vector3D<_Ty>::Vector3D(const Vector4D<_Ty>& vector4D) noexcept :
		Vector<_Ty, 3>({ vector4D.x, vector4D.y, vector4D.z })
	{
	}
	
	
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
}