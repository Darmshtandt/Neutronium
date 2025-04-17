#pragma once

#include <cmath>
#include <vcruntime.h>

#include <Nt/Core/Math/Math.h>
#include <Nt/Core/Math/VectorScalars.h>

namespace Nt {
	class String;

	template <typename _Ty, uInt size>
	struct Vector : public VectorScalars<_Ty, size> {
		using ValueType = _Ty;

		constexpr static uInt Size = size;

		_CONSTEXPR20 Vector() noexcept = default;
		_CONSTEXPR20 Vector(const std::initializer_list<_Ty>& list) noexcept;

		template <uInt S = size, typename = typename std::enable_if_t<S == 3>>
		_NODISCARD _CONSTEXPR20 Vector GetCross(const Vector& vector) const noexcept;

		_CONSTEXPR20 Vector& Fill(const _Ty& scalar) noexcept;
		_CONSTEXPR20 Vector& Set(const uInt& index, const _Ty& scalar);

		_NODISCARD _CONSTEXPR20 _Ty LengthSquare() const noexcept;
		_NODISCARD _CONSTEXPR20 _Ty Length() const noexcept;
		_NODISCARD _CONSTEXPR20 _Ty GetSquareDistance(const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 _Ty GetDistance(const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector GetNormalize() const noexcept;
		_NODISCARD _CONSTEXPR20 _Ty Dot(const Vector& vector) const noexcept;

		_NODISCARD _CONSTEXPR20 String ToString() const noexcept;
		_NODISCARD _CONSTEXPR20 void FromString(const String& string) const noexcept;

		_NODISCARD _CONSTEXPR20 Vector GetClamp(const _Ty& min, const _Ty& max) const noexcept;

		_NODISCARD _CONSTEXPR20 Vector Copy() const noexcept;

		_NODISCARD _CONSTEXPR20 Vector& Apply(const Vector& other, auto operation) noexcept;
		_NODISCARD _CONSTEXPR20 Vector& Apply(const _Ty& scalar, auto operation) noexcept;
		_NODISCARD _CONSTEXPR20 Vector ApplyCopy(const Vector& other, auto operation) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector ApplyCopy(const _Ty& scalar, auto operation) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector ApplyCopy(auto operation) const noexcept;

		_NODISCARD _CONSTEXPR20 Bool Compare(const Vector& vector, auto comparator) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool Compare(const _Ty& scalar, auto comparator) const noexcept;

		_NODISCARD _CONSTEXPR20 Vector Round() const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Ceil() const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Floor() const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Trunc() const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Abs() const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Sqrt() const noexcept;

		_NODISCARD _CONSTEXPR20 Vector Fract() const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Permute() const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Fade() const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Lerp(const Vector& a, const Vector& b) const noexcept;

		_NODISCARD _CONSTEXPR20 Vector Min(const Vector& other) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Max(const Vector& other) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Min(const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector Max(const _Ty& scalar) const noexcept;


// ============================================================================
//		Operators with vector
// ----------------------------------------------------------------------------
		_NODISCARD _CONSTEXPR20 Vector operator + (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator - (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator * (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator / (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator % (const Vector& vector) const noexcept;

		_CONSTEXPR20 Vector operator += (const Vector& vector) noexcept;
		_CONSTEXPR20 Vector operator -= (const Vector& vector) noexcept;
		_CONSTEXPR20 Vector operator *= (const Vector& vector) noexcept;
		_CONSTEXPR20 Vector operator /= (const Vector& vector) noexcept;
		_CONSTEXPR20 Vector operator %= (const Vector& vector) noexcept;

		_NODISCARD _CONSTEXPR20 Bool operator > (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator < (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator >= (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator <= (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator == (const Vector& vector) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator != (const Vector& vector) const noexcept;
		
// ============================================================================
//		Operators with scalars
// ----------------------------------------------------------------------------
		_NODISCARD _CONSTEXPR20 Vector operator + (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator - (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator * (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator / (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator % (const _Ty& scalar) const noexcept;

		_CONSTEXPR20 Vector operator += (const _Ty& scalar) noexcept;
		_CONSTEXPR20 Vector operator -= (const _Ty& scalar) noexcept;
		_CONSTEXPR20 Vector operator *= (const _Ty& scalar) noexcept;
		_CONSTEXPR20 Vector operator /= (const _Ty& scalar) noexcept;
		_CONSTEXPR20 Vector operator %= (const _Ty& scalar) noexcept;

		friend _NODISCARD _CONSTEXPR20 Vector operator + (const _Ty& scalar, const Vector& vector) noexcept {
			return vector.Copy().Apply(scalar, [](auto left, auto right) { return (right + left); });
		}
		friend _NODISCARD _CONSTEXPR20 Vector operator - (const _Ty& scalar, const Vector& vector) noexcept {
			return vector.Copy().Apply(scalar, [](auto left, auto right) { return (right - left); });
		}
		friend _NODISCARD _CONSTEXPR20 Vector operator * (const _Ty& scalar, const Vector& vector) noexcept {
			return vector.Copy().Apply(scalar, [](auto left, auto right) { return (right * left); });
		}
		friend _NODISCARD _CONSTEXPR20 Vector operator / (const _Ty& scalar, const Vector& vector) noexcept {
			return vector.Copy().Apply(scalar, [](auto left, auto right) { return (right / left); });
		}
		friend _NODISCARD _CONSTEXPR20 Vector operator % (const _Ty& scalar, const Vector& vector) noexcept {
			return vector.Copy().Apply(scalar, [](auto left, auto right) { return (right % left); });
		}

		_NODISCARD _CONSTEXPR20 Bool operator > (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator < (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator >= (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator <= (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator == (const _Ty& scalar) const noexcept;
		_NODISCARD _CONSTEXPR20 Bool operator != (const _Ty& scalar) const noexcept;

		friend _NODISCARD _CONSTEXPR20 Bool operator < (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare(scalar, [](auto left, auto right) { return (right < left); });
		}
		friend _NODISCARD _CONSTEXPR20 Bool operator > (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare(scalar, [](auto left, auto right) { return (right > left); });
		}
		friend _NODISCARD _CONSTEXPR20 Bool operator <= (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare(scalar, [](auto left, auto right) { return (right <= left); });
		}
		friend _NODISCARD _CONSTEXPR20 Bool operator >= (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare(scalar, [](auto left, auto right) { return (right >= left); });
		}
		friend _NODISCARD _CONSTEXPR20 Bool operator == (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare(scalar, [](auto left, auto right) { return (right == left); });
		}
		friend _NODISCARD _CONSTEXPR20 Bool operator != (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare(scalar, [](auto left, auto right) { return (right != left); });
		}

// ============================================================================
//		Others operators
// ----------------------------------------------------------------------------
		_NODISCARD _CONSTEXPR20 const _Ty& operator [] (const uInt& index) const;
		_NODISCARD _CONSTEXPR20 _Ty& operator [] (const uInt& index);

		_NODISCARD _CONSTEXPR20 Vector operator + () const noexcept;
		_NODISCARD _CONSTEXPR20 Vector operator - () const noexcept;

		_CONSTEXPR20 Vector& operator = (const Vector<_Ty, size>& vector);

		template <typename _U>
		_NODISCARD _CONSTEXPR20 operator Vector<_U, size>() const noexcept {
			Vector<_U, size> vector;
			for (uInt i = 0; i < size; ++i)
				vector.Array[i] = (_U)this->Array[i];

			return vector;
		}

		//constinit inline static Vector<_Ty, size> Zero = GetFilled(_Ty(0));
		//_CONSTEXPR20 static Vector<_Ty, size> Identity = GetFilled(_Ty(1));
	};

	template <typename _Ty> struct Vector<_Ty, 0>;
	template <typename _Ty> struct Vector<_Ty, 1>;
}

#include <Nt/Core/Math/VectorND.h>
#include <Nt/Core/Math/Vector.inl>