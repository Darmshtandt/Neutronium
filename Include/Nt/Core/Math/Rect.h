#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	template <typename _Ty>
	struct Rect {
		_CONSTEXPR20 Rect() noexcept :
			LeftTop(), 
			RightBottom() 
		{
		}
		_CONSTEXPR20 Rect(const Vector2D<_Ty>& leftTop, const Vector2D<_Ty>& rightBottom) noexcept :
			LeftTop(leftTop),
			RightBottom(rightBottom) 
		{
		}
		_CONSTEXPR20 Rect(const _Ty& left, const _Ty& top, const _Ty& right, const _Ty& bottom) noexcept :
			Left(left),
			Top(top),
			Right(right),
			Bottom(bottom) 
		{
		}
		_CONSTEXPR20 Rect(const Rect& rect) noexcept :
			LeftTop(rect.LeftTop),
			RightBottom(rect.RightBottom)
		{
		}
#ifdef _WINDEF_
		_CONSTEXPR20 Rect(const RECT& rect) noexcept :
			Left(rect.left),
			Top(rect.top),
			Right(rect.right),
			Bottom(rect.bottom) 
		{
		}
#endif

		_CONSTEXPR20 Rect<_Ty> Copy() const noexcept {
			return Rect<_Ty>(*this);
		}

		_CONSTEXPR20 void Fill(const _Ty& value) noexcept {
			LeftTop.Fill(value);
			RightBottom.Fill(value);
		}
		_CONSTEXPR20 void Fill(const Vector2D<_Ty>& vector) noexcept {
			LeftTop = vector;
			RightBottom = vector;
		}

		_CONSTEXPR20 Bool Intersect(const Rect<_Ty>& otherRect) const noexcept {
			return (LeftTop < (otherRect.LeftTop + otherRect.RightBottom) && otherRect.LeftTop < (LeftTop + RightBottom));
		}
		_CONSTEXPR20 Bool Intersect(const Vector2D<_Ty>& point) const noexcept {
			return (point.x >= Left && point.x <= (Left + Right) && point.y >= Top && point.y <= (Top + Bottom));
		}

		_CONSTEXPR20 Nt::Rect<_Ty> GetClamp(const _Ty& min, const _Ty& max) {
			Nt::Rect<_Ty> rect = (*this);
			for (uInt i = 0; i < 4; ++i) {
				if (rect.Array[i] < min)
					rect.Array[i] = min;
				else if (rect.Array[i] > max)
					rect.Array[i] = max;
			}

			return rect;
		}
		_CONSTEXPR20 Nt::Rect<_Ty> GetClamp(const Vector2D<_Ty>& min, const Vector2D<_Ty>& max) {
			return Nt::Rect<_Ty>(LeftTop.GetClamp(min.x, max.x), RightBottom.GetClamp(min.y, max.y));
		}

		_CONSTEXPR20 Rect<_Ty>& operator += (const Rect<_Ty>& rect) noexcept {
			LeftTop += rect.LeftTop;
			RightBottom += rect.RightBottom;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator -= (const Rect<_Ty>& rect) noexcept {
			LeftTop -= rect.LeftTop;
			RightBottom -= rect.RightBottom;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator *= (const Rect<_Ty>& rect) noexcept {
			LeftTop *= rect.LeftTop;
			RightBottom *= rect.RightBottom;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator /= (const Rect<_Ty>& rect) noexcept {
			LeftTop /= rect.LeftTop;
			RightBottom /= rect.RightBottom;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator %= (const Rect<_Ty>& rect) noexcept {
			LeftTop %= rect.LeftTop;
			RightBottom %= rect.RightBottom;
			return (*this);
		}

		_CONSTEXPR20 Rect<_Ty> operator + (const Rect<_Ty>& rect) const noexcept {
			return Copy() += rect;
		}
		_CONSTEXPR20 Rect<_Ty> operator - (const Rect<_Ty>& rect) const noexcept {
			return Copy() -= rect;
		}
		_CONSTEXPR20 Rect<_Ty> operator * (const Rect<_Ty>& rect) const noexcept {
			return Copy() *= rect;
		}
		_CONSTEXPR20 Rect<_Ty> operator / (const Rect<_Ty>& rect) const noexcept {
			return Copy() /= rect;
		}
		_CONSTEXPR20 Rect<_Ty> operator % (const Rect<_Ty>& rect) const noexcept {
			return Copy() %= rect;
		}

		_CONSTEXPR20 Bool operator == (const Rect<_Ty>& rect) const noexcept {
			return (LeftTop == rect.LeftTop && RightBottom == rect.RightBottom);
		}


		_CONSTEXPR20 Rect<_Ty>& operator += (const Vector2D<_Ty>& vector) noexcept {
			LeftTop += vector;
			RightBottom += vector;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator -= (const Vector2D<_Ty>& vector) noexcept {
			LeftTop -= vector;
			RightBottom -= vector;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator *= (const Vector2D<_Ty>& vector) noexcept {
			LeftTop *= vector;
			RightBottom *= vector;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator /= (const Vector2D<_Ty>& vector) noexcept {
			LeftTop /= vector;
			RightBottom /= vector;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator %= (const Vector2D<_Ty>& vector) noexcept {
			LeftTop %= vector;
			RightBottom %= vector;
			return (*this);
		}

		_CONSTEXPR20 Rect<_Ty> operator + (const Vector2D<_Ty>& vector) const noexcept {
			return Copy() += vector;
		}
		_CONSTEXPR20 Rect<_Ty> operator - (const Vector2D<_Ty>& vector) const noexcept {
			return Copy() += vector;
		}
		_CONSTEXPR20 Rect<_Ty> operator * (const Vector2D<_Ty>& vector) const noexcept {
			return Copy() += vector;
		}
		_CONSTEXPR20 Rect<_Ty> operator / (const Vector2D<_Ty>& vector) const noexcept {
			return Copy() += vector;
		}
		_CONSTEXPR20 Rect<_Ty> operator % (const Vector2D<_Ty>& vector) const noexcept {
			return Copy() += vector;
		}


		_CONSTEXPR20 Rect<_Ty>& operator += (const _Ty& num) noexcept {
			LeftTop += num;
			RightBottom += num;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator -= (const _Ty& num) noexcept {
			LeftTop -= num;
			RightBottom -= num;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator *= (const _Ty& num) noexcept {
			LeftTop *= num;
			RightBottom *= num;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator /= (const _Ty& num) noexcept {
			LeftTop /= num;
			RightBottom /= num;
			return (*this);
		}
		_CONSTEXPR20 Rect<_Ty>& operator %= (const _Ty& num) noexcept {
			LeftTop %= num;
			RightBottom %= num;
			return (*this);
		}

		_CONSTEXPR20 Rect<_Ty> operator + (const _Ty& num) const noexcept {
			return Copy() += num;
		}
		_CONSTEXPR20 Rect<_Ty> operator - (const _Ty& num) const noexcept {
			return Copy() -= num;
		}
		_CONSTEXPR20 Rect<_Ty> operator * (const _Ty& num) const noexcept {
			return Copy() *= num;
		}
		_CONSTEXPR20 Rect<_Ty> operator / (const _Ty& num) const noexcept {
			return Copy() /= num;
		}
		_CONSTEXPR20 Rect<_Ty> operator % (const _Ty& num) const noexcept {
			return Copy() %= num;
		}

		_CONSTEXPR20 Bool operator == (const _Ty& num) const noexcept {
			return (LeftTop == num && RightBottom == num);
		}

		_CONSTEXPR20 Rect<_Ty>& operator = (const Rect<_Ty>& rect) noexcept {
			LeftTop = rect.LeftTop;
			RightBottom = rect.RightBottom;

			return *this;
		}
		_CONSTEXPR20 const _Ty& operator [] (const uInt& index) const {
			if (index >= 4)
				Raise("Out of range");

			if (index < 2)
				return LeftTop[index];

			return RightBottom[index % 2];
		}
		_CONSTEXPR20 _Ty& operator [] (const uInt& index) {
			const Rect* pConstThis = this;
			return const_cast<_Ty&>((*pConstThis)[index]);
		}

#ifdef _WINDEF_
		_CONSTEXPR20 operator RECT() const noexcept {
			RECT rect;
			rect.left = static_cast<Long>(Left);
			rect.top = static_cast<Long>(Top);
			rect.right = static_cast<Long>(Right);
			rect.bottom = static_cast<Long>(Bottom);
			return rect;
		}
#endif

		template <typename _U>
		_CONSTEXPR20 operator Rect<_U>() const noexcept {
			return {
				Vector2D<_U>(LeftTop),
				Vector2D<_U>(RightBottom)
			};
		}

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