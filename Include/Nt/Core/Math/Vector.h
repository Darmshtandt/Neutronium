#pragma once

#include <cmath>
#include <vcruntime.h>

#include <Nt/Core/Math/Math.h>
#include <Nt/Core/Math/VectorScalars.h>

#pragma warning(push)
#pragma warning(disable: 4544)
//#pragma warning(disable: 4804)

namespace Nt {
	class String;

	template<typename _Ty, typename... _Args>
	struct all_same : std::true_type
	{
	};

	template<typename _Ty, typename _U, typename... _Rest>
	struct all_same<_Ty, _U, _Rest...>
		: std::conditional_t<
		std::is_same_v<_Ty, _U>,
		all_same<_Ty, _Rest...>,
		std::false_type
		>
	{
	};

	template<typename _Ty, typename... _Args>
	NT_CONSTEXPR Bool all_same_v = all_same<_Ty, _Args...>::value;


#define CREATE_BINARY_OPERATION_STRUCT(name, resultType, operation) \
	template <typename _Ty> \
	struct name { \
		NT_NODISCARD_CONSTEXPR static NT_FORCE_INLINE \
		resultType Apply(const _Ty& a, const _Ty& b) noexcept { return (operation); } \
	}

#define CREATE_UNARY_OPERATION_STRUCT(name, resultType, operation) \
	template <typename _Ty> \
	struct name { \
		NT_NODISCARD_CONSTEXPR static NT_FORCE_INLINE \
		resultType Apply(const _Ty& a) noexcept { return (operation); } \
	}

	template <typename _Ty>
	struct BinOpDiv {
		NT_NODISCARD_CONSTEXPR static NT_FORCE_INLINE
		_Ty Apply(const _Ty& a, const _Ty& b) noexcept {
			if (b == _Ty(0))
				return _Ty(0);
			return a / b;
		}
	};

	CREATE_BINARY_OPERATION_STRUCT(BinOpAdd, _Ty, a + b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpSub, _Ty, a - b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpMul, _Ty, a * b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpMod, _Ty, a % b);

	CREATE_BINARY_OPERATION_STRUCT(BinOpEq, Bool, a == b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpNeq, Bool, a != b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpLt, Bool, a < b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpLte, Bool, a <= b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpGt, Bool, a > b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpGte, Bool, a >= b);

	CREATE_UNARY_OPERATION_STRUCT(UnOpNegate, _Ty, -a);
	CREATE_UNARY_OPERATION_STRUCT(UnOpLogicalNot, Bool, !a);

	CREATE_UNARY_OPERATION_STRUCT(UnOpRound, _Ty, std::round(a));
	CREATE_UNARY_OPERATION_STRUCT(UnOpCeil, _Ty, std::ceil(a));
	CREATE_UNARY_OPERATION_STRUCT(UnOpFloor, _Ty, std::floor(a));
	CREATE_UNARY_OPERATION_STRUCT(UnOpTrunc, _Ty, Nt::Trunc(a));
	CREATE_UNARY_OPERATION_STRUCT(UnOpSqrt, _Ty, std::sqrt(a));
	CREATE_UNARY_OPERATION_STRUCT(UnOpAbs, _Ty, Nt::Abs(a));
	CREATE_UNARY_OPERATION_STRUCT(UnOpFract, _Ty, Nt::Fract(a));
	CREATE_UNARY_OPERATION_STRUCT(UnOpPermute, _Ty, Nt::Permute(a));
	CREATE_UNARY_OPERATION_STRUCT(UnOpFade, _Ty, Nt::Fade(a));

	CREATE_BINARY_OPERATION_STRUCT(BinOpMin, _Ty, (a < b) ? a : b);
	CREATE_BINARY_OPERATION_STRUCT(BinOpMax, _Ty, (a > b) ? a : b);

	template <typename _Ty, uInt size>
	struct Vector : public VectorScalars<_Ty, size> {
		using ValueType = _Ty;

		constexpr static uInt Size = size;

		NT_CONSTEXPR Vector() noexcept;
		NT_CONSTEXPR Vector(const Vector& other) noexcept;
		NT_CONSTEXPR Vector(const std::initializer_list<_Ty>& list) noexcept;

		template <typename _U>
		NT_CONSTEXPR Vector(const Vector<_U, size>& other) noexcept;

		template <typename... _Args, Requires(sizeof...(_Args) == size && all_same_v<_Args...>)>
		NT_CONSTEXPR Vector(_Args... args) noexcept;

		template <uInt S = size, typename = typename std::enable_if_t<S == 3>>
		NT_NODISCARD_CONSTEXPR Vector GetCross(const Vector& vector) const noexcept;

		template <uInt index>
		NT_CONSTEXPR Vector& Assign() noexcept;
		template <uInt index>
		NT_CONSTEXPR Vector& Assign(const std::initializer_list<_Ty>& list) noexcept;

		template <uInt index, typename First, typename... Rest>
		NT_CONSTEXPR Vector& Assign(First value, Rest... rest) noexcept;

		NT_CONSTEXPR Vector& Fill(const _Ty& scalar) noexcept;

		template <uInt index>
		NT_CONSTEXPR Vector& Set(const _Ty& scalar);

		template <uInt index>
		NT_CONSTEXPR _Ty Get() const;

		template <uInt index>
		NT_NODISCARD_CONSTEXPR _Ty LengthSquareImpl() const noexcept;
		template <uInt index>
		NT_NODISCARD_CONSTEXPR _Ty DotImpl() const noexcept;
		template <uInt index, typename First, typename... Rest>
		NT_NODISCARD_CONSTEXPR _Ty DotImpl(First value, Rest... rest) const noexcept;

		NT_NODISCARD_CONSTEXPR _Ty LengthSquare() const noexcept;
		NT_NODISCARD_CONSTEXPR _Ty Length() const noexcept;
		NT_NODISCARD_CONSTEXPR _Ty GetSquareDistance(const Vector& vector) const noexcept;
		NT_NODISCARD _Ty GetDistance(const Vector& vector) const noexcept;
		NT_NODISCARD Vector GetNormalize() const noexcept;
		NT_NODISCARD_CONSTEXPR _Ty Dot(const Vector& vector) const noexcept;

		NT_NODISCARD String ToString() const noexcept;
		NT_NODISCARD Vector& FromString(const String& string) const noexcept;

		NT_NODISCARD_CONSTEXPR Vector GetClamp(const _Ty& min, const _Ty& max) const noexcept;

		NT_NODISCARD_CONSTEXPR Vector Copy() const noexcept;

		template <class _Op>
		NT_NODISCARD_CONSTEXPR Vector& Apply() noexcept;
		template <class _Op, uInt index>
		NT_NODISCARD_CONSTEXPR Vector& ApplyImpl() noexcept;
		template <class _Op, uInt index, typename First, typename... Rest>
		NT_NODISCARD_CONSTEXPR Vector& ApplyImpl(First value, Rest... rest) noexcept;
		template <class _Op>
		NT_NODISCARD_CONSTEXPR Vector& Apply(const Vector& other) noexcept;
		template <class _Op>
		NT_NODISCARD_CONSTEXPR Vector& Apply(const _Ty& scalar) noexcept;

		template <class _Op>
		NT_NODISCARD_CONSTEXPR Vector ApplyCopy(const Vector& other) const noexcept;
		template <class _Op>
		NT_NODISCARD_CONSTEXPR Vector ApplyCopy(const _Ty& scalar) const noexcept;
		template <class _Op>
		NT_NODISCARD_CONSTEXPR Vector ApplyCopy() const noexcept;

		template <class _Op>
		NT_NODISCARD_CONSTEXPR Bool Compare() const noexcept;
		template <class _Op, uInt index>
		NT_NODISCARD_CONSTEXPR Bool CompareImpl() const noexcept;
		template <class _Op, uInt index, typename First, typename... Rest>
		NT_NODISCARD_CONSTEXPR Bool CompareImpl(First value, Rest... rest) const noexcept;
		template <class _Op>
		NT_NODISCARD_CONSTEXPR Bool Compare(const Vector& other) const noexcept;
		template <class _Op>
		NT_NODISCARD_CONSTEXPR Bool Compare(const _Ty& scalar) const noexcept;

		NT_NODISCARD_CONSTEXPR Bool CompletelyNotEqual(const Vector& other) const noexcept;

		NT_NODISCARD_CONSTEXPR Vector Round() const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Ceil() const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Floor() const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Trunc() const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Abs() const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Sqrt() const noexcept;

		NT_NODISCARD_CONSTEXPR Vector Fract() const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Permute() const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Fade() const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Lerp(const Vector& a, const Vector& b) const noexcept;

		NT_NODISCARD_CONSTEXPR Vector Min(const Vector& other) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Max(const Vector& other) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Min(const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector Max(const _Ty& scalar) const noexcept;


// ============================================================================
//		Operators with vector
// ----------------------------------------------------------------------------
		NT_NODISCARD_CONSTEXPR Vector operator + (const Vector& vector) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector operator - (const Vector& vector) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector operator * (const Vector& vector) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector operator / (const Vector& vector) const noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_NODISCARD_CONSTEXPR Vector operator % (const Vector& vector) const noexcept;

		NT_CONSTEXPR Vector& operator += (const Vector& vector) noexcept;
		NT_CONSTEXPR Vector& operator -= (const Vector& vector) noexcept;
		NT_CONSTEXPR Vector& operator *= (const Vector& vector) noexcept;
		NT_CONSTEXPR Vector& operator /= (const Vector& vector) noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_CONSTEXPR Vector& operator %= (const Vector& vector) noexcept;

		NT_NODISCARD_CONSTEXPR Bool operator > (const Vector& vector) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator < (const Vector& vector) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator >= (const Vector& vector) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator <= (const Vector& vector) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator == (const Vector& vector) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator != (const Vector& vector) const noexcept;
		
// ============================================================================
//		Operators with scalars
// ----------------------------------------------------------------------------
		NT_NODISCARD_CONSTEXPR Vector operator + (const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector operator - (const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector operator * (const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Vector operator / (const _Ty& scalar) const noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_NODISCARD_CONSTEXPR Vector operator % (const _Ty& scalar) const noexcept;

		NT_CONSTEXPR Vector& operator += (const _Ty& scalar) noexcept;
		NT_CONSTEXPR Vector& operator -= (const _Ty& scalar) noexcept;
		NT_CONSTEXPR Vector& operator *= (const _Ty& scalar) noexcept;
		NT_CONSTEXPR Vector& operator /= (const _Ty& scalar) noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_CONSTEXPR Vector& operator %= (const _Ty& scalar) noexcept;

		friend NT_NODISCARD_CONSTEXPR Vector operator + (const _Ty& scalar, const Vector& vector) noexcept {
			Vector other;
			other.Fill(scalar);
			return other.Apply<BinOpAdd<_Ty>>(vector);
		}
		friend NT_NODISCARD_CONSTEXPR Vector operator - (const _Ty& scalar, const Vector& vector) noexcept {
			Vector other;
			other.Fill(scalar);
			return other.Apply<BinOpSub<_Ty>>(vector);
		}
		friend NT_NODISCARD_CONSTEXPR Vector operator * (const _Ty& scalar, const Vector& vector) noexcept {
			Vector other;
			other.Fill(scalar);
			return other.Apply<BinOpMul<_Ty>>(vector);
		}
		friend NT_NODISCARD_CONSTEXPR Vector operator / (const _Ty& scalar, const Vector& vector) noexcept {
			Vector other;
			other.Fill(scalar);
			return other.Apply<BinOpDiv<_Ty>>(vector);
		}
		template <Requires(std::is_integral_v<_Ty>)>
		friend NT_NODISCARD_CONSTEXPR Vector operator % (const _Ty& scalar, const Vector& vector) noexcept {
			Vector other;
			other.Fill(scalar);
			return other.Apply<BinOpMod<_Ty>>(vector);
		}

		NT_NODISCARD_CONSTEXPR Bool operator > (const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator < (const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator >= (const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator <= (const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator == (const _Ty& scalar) const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator != (const _Ty& scalar) const noexcept;

		friend NT_NODISCARD_CONSTEXPR Bool operator < (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare<BinOpLt>(scalar);
		}
		friend NT_NODISCARD_CONSTEXPR Bool operator > (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare<BinOpGt>(scalar);
		}
		friend NT_NODISCARD_CONSTEXPR Bool operator <= (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare<BinOpLte>(scalar);
		}
		friend NT_NODISCARD_CONSTEXPR Bool operator >= (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare<BinOpGte>(scalar);
		}
		friend NT_NODISCARD_CONSTEXPR Bool operator == (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare<BinOpEq>(scalar);
		}
		friend NT_NODISCARD_CONSTEXPR Bool operator != (const _Ty& scalar, const Vector& vector) noexcept {
			return Compare<BinOpNeq>(scalar);
		}

// ============================================================================
//		Others operators
// ----------------------------------------------------------------------------
		NT_NODISCARD_CONSTEXPR const _Ty& operator [] (const uInt& index) const;
		NT_NODISCARD_CONSTEXPR _Ty& operator [] (const uInt& index);

		NT_NODISCARD_CONSTEXPR Vector operator + () const noexcept;
		NT_NODISCARD_CONSTEXPR Vector operator - () const noexcept;
		NT_NODISCARD_CONSTEXPR Bool operator ! () const noexcept;

		NT_CONSTEXPR Vector& operator = (const Vector<_Ty, size>& vector) noexcept;

		template <typename _U>
		NT_NODISCARD_CONSTEXPR NT_FORCE_INLINE operator Vector<_U, size>() const noexcept {
			Vector<_U, size> vector;

			if NT_CONSTEXPR (size >= 2 && size <= 4) {
				vector.x = static_cast<_U>(this->x);
				vector.y = static_cast<_U>(this->y);

				if NT_CONSTEXPR(size >= 3)
					vector.z = static_cast<_U>(this->z);
				if NT_CONSTEXPR(size == 4)
					vector.w = static_cast<_U>(this->w);
			}
			else {
				for (uInt i = 0; i < size; ++i)
					vector.Array[i] = (_U)this->Array[i];
			}

			return vector;
		}

		//constinit inline static Vector<_Ty, size> Zero = GetFilled(_Ty(0));
		//NT_CONSTEXPR static Vector<_Ty, size> Identity = GetFilled(_Ty(1));
	};

	template <typename _Ty> struct Vector<_Ty, 0>;
	template <typename _Ty> struct Vector<_Ty, 1>;
}

#include <Nt/Core/Math/VectorND.h>
#include <Nt/Core/Math/Vector.inl>

#pragma warning(pop)