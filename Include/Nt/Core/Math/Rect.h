#pragma once

#include <Nt/Core/Math/Vector.h>

#pragma warning(push)
#pragma warning(disable: 4544)
#pragma warning(disable: 4804)

namespace Nt {
	template <typename _Ty>
	struct Rect {
		NT_CONSTEXPR Rect() noexcept;
		NT_CONSTEXPR Rect(const Vector2D<_Ty>& leftTop, const Vector2D<_Ty>& rightBottom) noexcept;
		NT_CONSTEXPR Rect(const _Ty& left, const _Ty& top, const _Ty& right, const _Ty& bottom) noexcept;
		NT_CONSTEXPR Rect(const Rect& rect) noexcept;
		NT_CONSTEXPR Rect(Rect&& rect) noexcept;
#ifdef _WINDEF_
		NT_CONSTEXPR Rect(const RECT& rect) noexcept;
#endif
		NT_CONSTEXPR ~Rect() noexcept = default;

		NT_CONSTEXPR Rect Copy() const noexcept;

		NT_CONSTEXPR Rect& Fill(const _Ty& value) noexcept;
		NT_CONSTEXPR Rect& Fill(const Vector2D<_Ty>& vector) noexcept;
		NT_CONSTEXPR Rect& Inflate(const Vector2D<_Ty>& vector) noexcept;

		NT_CONSTEXPR Bool Intersect(const Rect<_Ty>& otherRect) const noexcept;
		NT_CONSTEXPR Bool Intersect(const Vector2D<_Ty>& point) const noexcept;

		NT_CONSTEXPR Rect<_Ty> GetClamp(const _Ty& min, const _Ty& max);
		NT_CONSTEXPR Rect<_Ty> GetClamp(const Vector2D<_Ty>& min, const Vector2D<_Ty>& max);

		NT_CONSTEXPR Rect<_Ty>& operator += (const Rect<_Ty>& rect) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator -= (const Rect<_Ty>& rect) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator *= (const Rect<_Ty>& rect) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator /= (const Rect<_Ty>& rect) noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_CONSTEXPR Rect<_Ty>& operator %= (const Rect<_Ty>& rect) noexcept;

		NT_CONSTEXPR Rect<_Ty> operator + (const Rect<_Ty>& rect) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator - (const Rect<_Ty>& rect) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator * (const Rect<_Ty>& rect) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator / (const Rect<_Ty>& rect) const noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_CONSTEXPR Rect<_Ty> operator % (const Rect<_Ty>& rect) const noexcept;

		NT_CONSTEXPR Bool operator == (const Rect<_Ty>& rect) const noexcept;

		NT_CONSTEXPR Rect<_Ty>& operator += (const Vector2D<_Ty>& vector) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator -= (const Vector2D<_Ty>& vector) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator *= (const Vector2D<_Ty>& vector) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator /= (const Vector2D<_Ty>& vector) noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_CONSTEXPR Rect<_Ty>& operator %= (const Vector2D<_Ty>& vector) noexcept;

		NT_CONSTEXPR Rect<_Ty> operator + (const Vector2D<_Ty>& vector) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator - (const Vector2D<_Ty>& vector) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator * (const Vector2D<_Ty>& vector) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator / (const Vector2D<_Ty>& vector) const noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_CONSTEXPR Rect<_Ty> operator % (const Vector2D<_Ty>& vector) const noexcept;


		NT_CONSTEXPR Rect<_Ty>& operator += (const _Ty& num) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator -= (const _Ty& num) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator *= (const _Ty& num) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator /= (const _Ty& num) noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_CONSTEXPR Rect<_Ty>& operator %= (const _Ty& num) noexcept;

		NT_CONSTEXPR Rect<_Ty> operator + (const _Ty& num) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator - (const _Ty& num) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator * (const _Ty& num) const noexcept;
		NT_CONSTEXPR Rect<_Ty> operator / (const _Ty& num) const noexcept;
		template <Requires(std::is_integral_v<_Ty>)>
		NT_CONSTEXPR Rect<_Ty> operator % (const _Ty& num) const noexcept;

		NT_CONSTEXPR Bool operator == (const _Ty& num) const noexcept;

		NT_CONSTEXPR Rect<_Ty>& operator = (const Rect<_Ty>& rect) noexcept;
		NT_CONSTEXPR Rect<_Ty>& operator = (Rect<_Ty>&& rect) noexcept;

		NT_CONSTEXPR const _Ty& operator [] (const uInt& index) const;
		NT_CONSTEXPR _Ty& operator [] (const uInt& index);

#ifdef _WINDEF_
		NT_CONSTEXPR operator RECT() const noexcept;
#endif
		
		template <typename _U>
		NT_CONSTEXPR operator Rect<_U>() const noexcept;

		union {
			struct {
				Vector2D<_Ty> LeftTop;
				Vector2D<_Ty> RightBottom;
			};

			struct {
				_Ty Left;
				_Ty Top;
				_Ty Right;
				_Ty Bottom;
			};

			_Ty Array[4];
		};
	};

#define NT_DECLARE_RECT_TYPE(type) \
	using type##Rect = Rect<type>;

	NT_DECLARE_RECT_TYPE(Float);
	NT_DECLARE_RECT_TYPE(Double);
	NT_DECLARE_RECT_TYPE(LDouble);

	NT_DECLARE_RECT_TYPE(Byte);
	NT_DECLARE_RECT_TYPE(Word);
	NT_DECLARE_RECT_TYPE(DWord);
	NT_DECLARE_RECT_TYPE(QWord);

	NT_DECLARE_RECT_TYPE(Bool);
	NT_DECLARE_RECT_TYPE(Char);
	NT_DECLARE_RECT_TYPE(Short);
	NT_DECLARE_RECT_TYPE(Int);
	NT_DECLARE_RECT_TYPE(Long);
	NT_DECLARE_RECT_TYPE(LLong);

	NT_DECLARE_RECT_TYPE(uChar);
	NT_DECLARE_RECT_TYPE(uShort);
	NT_DECLARE_RECT_TYPE(uInt);
	NT_DECLARE_RECT_TYPE(uLong);
	NT_DECLARE_RECT_TYPE(uLLong);

#undef NT_DECLARE_RECT_TYPE
}

#include <Nt/Core/Math/Rect.inl>

#pragma warning(pop)