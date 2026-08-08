#pragma once

#include <Nt/Core/NtTypes.h>

namespace Nt {
	//template <typename _Ty, uInt size>
	//struct VectorConstants1D {
	//	VectorConstants1D() = default;
	//	VectorConstants1D(const VectorConstants1D&) = delete;
	//	VectorConstants1D(VectorConstants1D&&) = delete;

	//	NT_CONSTEXPR static Vector<_Ty, size> Left = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(0, _Ty(-1));
	//	NT_CONSTEXPR static Vector<_Ty, size> Right = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(0, _Ty(-1));
	//};

	//template <typename _Ty, uInt size>
	//struct VectorConstants2D : public VectorConstants1D<_Ty, size> {
	//	VectorConstants2D() = default;
	//	VectorConstants2D(const VectorConstants2D&) = delete;
	//	VectorConstants2D(VectorConstants2D&&) = delete;

	//	NT_CONSTEXPR static Vector<_Ty, size> Up = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(1, _Ty(1));
	//	NT_CONSTEXPR static Vector<_Ty, size> Down = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(1, _Ty(-1));
	//};

	//template <typename _Ty, uInt size>
	//struct VectorConstants3D : public VectorConstants2D<_Ty, size> {
	//	VectorConstants3D() = default;
	//	VectorConstants3D(const VectorConstants3D&) = delete;
	//	VectorConstants3D(VectorConstants3D&&) = delete;

	//	NT_CONSTEXPR static Vector<_Ty, size> Forward = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(2, _Ty(1));
	//	NT_CONSTEXPR static Vector<_Ty, size> Back = Vector<_Ty, size>::GetFilled(_Ty(0)).Set(2, _Ty(-1));
	//};

	template <typename _Ty> struct Vector3D;
	template <typename _Ty> struct Vector4D;

	template <typename _Ty>
	struct Vector2D : public Vector<_Ty, 2> {
		NT_CONSTEXPR Vector2D() noexcept :
			Vector<_Ty, 2>()
		{
		}
		template <typename _U>
		NT_CONSTEXPR Vector2D(const Vector<_U, 2>& vec) noexcept :
			Vector<_Ty, 2>(_Ty(vec.x), _Ty(vec.y))
		{
		}
		NT_CONSTEXPR Vector2D(const Vector<_Ty, 2>& vector) noexcept :
			Vector<_Ty, 2>(vector)
		{
		}
		NT_CONSTEXPR Vector2D(const _Ty& x, const _Ty& y) noexcept :
			Vector<_Ty, 2>(x, y)
		{
		}
		NT_CONSTEXPR Vector2D(const std::initializer_list<_Ty>& list) noexcept :
			Vector<_Ty, 2>(list)
		{
		}
		NT_CONSTEXPR Vector2D(const Vector3D<_Ty>& vector3D) noexcept;
		NT_CONSTEXPR Vector2D(const Vector4D<_Ty>& vector4D) noexcept;

#ifdef _WINDEF_
		NT_CONSTEXPR Vector2D(const POINT& point) noexcept :
			Vector<_Ty, 2>({ _Ty(point.x), _Ty(point.y) })
		{
		}
		NT_CONSTEXPR Vector2D(const SIZE& sizeStruct) noexcept :
			Vector<_Ty, 2>({ _Ty(sizeStruct.cx), _Ty(sizeStruct.cy) })
		{
		}

		NT_CONSTEXPR operator POINT() const noexcept {
			return { Long(this->x), Long(this->y) };
		}
		NT_CONSTEXPR operator SIZE() const noexcept {
			return { Long(this->x), Long(this->y) };
		}
#endif
		
		NT_NODISCARD_CONSTEXPR NT_FORCE_INLINE _Ty GetCross(const Vector2D<_Ty>& vector) const noexcept {
			return (this->x * vector.y - this->y * vector.x);
		}

		template <typename _U>
		NT_NODISCARD_CONSTEXPR NT_FORCE_INLINE operator Vector2D<_U>() const noexcept {
			return this->operator Vector<_U, 2>();
		}
	};

	template <typename _Ty>
	struct Vector3D : public Vector<_Ty, 3> {
		NT_CONSTEXPR Vector3D() noexcept :
			Vector<_Ty, 3>()
		{
		}
		NT_CONSTEXPR Vector3D(const Vector<_Ty, 3>& vec) noexcept :
			Vector<_Ty, 3>(vec)
		{
		}
		NT_CONSTEXPR Vector3D(const _Ty& x, const _Ty& y, const _Ty& z) noexcept :
			Vector<_Ty, 3>(x, y, z)
		{
		}
		NT_CONSTEXPR Vector3D(const Vector2D<_Ty>& vec, const _Ty& z) noexcept :
			Vector<_Ty, 3>(vec.x, vec.y, z)
		{
		}
		NT_CONSTEXPR Vector3D(const _Ty& x, const Vector2D<_Ty>& vec) noexcept :
			Vector<_Ty, 3>(x, vec.x, vec.y)
		{
		}
		NT_CONSTEXPR Vector3D(const std::initializer_list<_Ty>& list) noexcept :
			Vector<_Ty, 3>(list)
		{
		}
		NT_CONSTEXPR Vector3D(const Vector4D<_Ty>& vector4D) noexcept;

		template <typename _U>
		NT_NODISCARD_CONSTEXPR NT_FORCE_INLINE operator Vector3D<_U>() const noexcept {
			return this->operator Vector<_U, 3>();
		}
	};

	template <typename _Ty>
	struct Vector4D : public Vector<_Ty, 4> {
		NT_CONSTEXPR Vector4D() noexcept :
			Vector<_Ty, 4>()
		{
		}
		NT_CONSTEXPR Vector4D(const Vector<_Ty, 4>& vec) noexcept :
			Vector<_Ty, 4>(vec)
		{
		}
		NT_CONSTEXPR Vector4D(const _Ty& x, const _Ty& y, const _Ty& z, const _Ty& w) noexcept :
			Vector<_Ty, 4>(x, y, z, w)
		{
		}
		NT_CONSTEXPR Vector4D(const Vector2D<_Ty>& vec, const _Ty& z, const _Ty& w) noexcept :
			Vector<_Ty, 4>(vec.x, vec.y, z, w)
		{
		}
		NT_CONSTEXPR Vector4D(const _Ty& x, const Vector2D<_Ty>& vec, const _Ty& w) noexcept :
			Vector<_Ty, 4>(x, vec.x, vec.y, w)
		{
		}
		NT_CONSTEXPR Vector4D(const _Ty& x, const _Ty& y, const Vector2D<_Ty>& vec) noexcept :
			Vector<_Ty, 4>(x, y, vec.x, vec.y)
		{
		}
		NT_CONSTEXPR Vector4D(const Vector2D<_Ty>& vec_1, const Vector2D<_Ty>& vec_2) noexcept :
			Vector<_Ty, 4>(vec_1.x, vec_1.y, vec_2.x, vec_2.y)
		{
		}

		NT_CONSTEXPR Vector4D(const Vector3D<_Ty>& vec, const _Ty& w) noexcept :
			Vector<_Ty, 4>(vec.x, vec.y, vec.z, w)
		{
		}
		NT_CONSTEXPR Vector4D(const _Ty& x, const Vector3D<_Ty>& vec) noexcept :
			Vector<_Ty, 4>(x, vec.x, vec.y, vec.z)
		{
		}
		NT_CONSTEXPR Vector4D(const std::initializer_list<_Ty>& list) noexcept :
			Vector<_Ty, 4>(list)
		{
		}

		template <typename _U>
		NT_NODISCARD_CONSTEXPR NT_FORCE_INLINE operator Vector4D<_U>() const noexcept {
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
	NT_CONSTEXPR Bool is_Vector_v = is_Vector<_Ty>::value;


	template<typename _Ty>
	inline NT_CONSTEXPR Vector2D<_Ty>::Vector2D(const Vector3D<_Ty>& vector3D) noexcept :
		Vector<_Ty, 2>({ vector3D.x, vector3D.y })
	{
	}
	template<typename _Ty>
	inline NT_CONSTEXPR Vector2D<_Ty>::Vector2D(const Vector4D<_Ty>& vector4D) noexcept :
		Vector<_Ty, 2>({ vector4D.x, vector4D.y })
	{
	}
	template<typename _Ty>
	inline NT_CONSTEXPR Vector3D<_Ty>::Vector3D(const Vector4D<_Ty>& vector4D) noexcept :
		Vector<_Ty, 3>({ vector4D.x, vector4D.y, vector4D.z })
	{
	}

#define NT_DECLARE_VECTOR_TYPES(type) \
	using type##2D = Vector2D<type>;\
	using type##3D = Vector3D<type>;\
	using type##4D = Vector4D<type>;

	NT_DECLARE_VECTOR_TYPES(Float);
	NT_DECLARE_VECTOR_TYPES(Double);
	NT_DECLARE_VECTOR_TYPES(LDouble);

	NT_DECLARE_VECTOR_TYPES(Byte);
	NT_DECLARE_VECTOR_TYPES(Word);
	NT_DECLARE_VECTOR_TYPES(DWord);
	NT_DECLARE_VECTOR_TYPES(QWord);

	NT_DECLARE_VECTOR_TYPES(Bool);
	NT_DECLARE_VECTOR_TYPES(Char);
	NT_DECLARE_VECTOR_TYPES(Short);
	NT_DECLARE_VECTOR_TYPES(Int);
	NT_DECLARE_VECTOR_TYPES(Long);
	NT_DECLARE_VECTOR_TYPES(LLong);

	NT_DECLARE_VECTOR_TYPES(uChar);
	NT_DECLARE_VECTOR_TYPES(uShort);
	NT_DECLARE_VECTOR_TYPES(uInt);
	NT_DECLARE_VECTOR_TYPES(uLong);
	NT_DECLARE_VECTOR_TYPES(uLLong);

#undef NT_DECLARE_VECTOR_TYPES
}