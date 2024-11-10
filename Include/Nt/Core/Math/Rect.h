#pragma once

namespace Nt {
	template <typename _Ty>
	struct Rect {
		constexpr Rect() noexcept :
			LeftTop(), 
			RightBottom() 
		{
		}
		constexpr Rect(const _Ty& num) noexcept :
			LeftTop(num),
			RightBottom(num)
		{
		}
		constexpr Rect(const Vector2D<_Ty>& vector) noexcept :
			LeftTop(vector),
			RightBottom(vector) 
		{
		}
		constexpr Rect(const Vector2D<_Ty>& LeftTop, const Vector2D<_Ty>& RightBottom) noexcept :
			LeftTop(LeftTop),
			RightBottom(RightBottom) 
		{
		}
		constexpr Rect(const _Ty& Left, const _Ty& Top, const _Ty& Right, const _Ty& Bottom) noexcept :
			Left(Left),
			Top(Top),
			Right(Right),
			Bottom(Bottom) 
		{
		}
#ifdef _WINDEF_
		constexpr Rect(const RECT& rect) noexcept :
			Left(rect.left),
			Top(rect.top),
			Right(rect.right),
			Bottom(rect.bottom) 
		{
		}
#endif

		constexpr Bool Intersect(const Rect<_Ty>& otherRect) const noexcept {
			return (LeftTop < (otherRect.LeftTop + otherRect.RightBottom) && otherRect.LeftTop < (LeftTop + RightBottom));
		}
		constexpr Bool Intersect(const Vector2D<_Ty>& point) const noexcept {
			return (point.x >= Left && point.x <= (Left + Right) && point.y >= Top && point.y <= (Top + Bottom));
		}

		constexpr Nt::Rect<_Ty> GetClamp(const _Ty& min, const _Ty& max) {
			Nt::Rect<_Ty> rect = (*this);
			for (uInt i = 0; i < 4; ++i) {
				if (rect.PointArray[i] < min)
					rect.PointArray[i] = min;
				else if (rect.PointArray[i] > max)
					rect.PointArray[i] = max;
			}

			return rect;
		}
		constexpr Nt::Rect<_Ty> GetClamp(const Vector2D<_Ty>& min, const Vector2D<_Ty>& max) {
			Nt::Rect<_Ty> rect = (*this);
			for (uInt i = 0; i < 4; ++i) {
				if (rect.PointArray[i] < min.xy[i / 2])
					rect.PointArray[i] = min.xy[i / 2];
				else if (rect.PointArray[i] > max.xy[i / 2])
					rect.PointArray[i] = max.xy[i / 2];
			}

			return rect;
		}

		constexpr Rect<_Ty>& operator += (const Rect<_Ty>& rect) noexcept {
			LeftTop += rect.LeftTop;
			RightBottom += rect.RightBottom;
			return (*this);
		}
		constexpr Rect<_Ty>& operator -= (const Rect<_Ty>& rect) noexcept {
			LeftTop -= rect.LeftTop;
			RightBottom -= rect.RightBottom;
			return (*this);
		}
		constexpr Rect<_Ty>& operator *= (const Rect<_Ty>& rect) noexcept {
			LeftTop *= rect.LeftTop;
			RightBottom *= rect.RightBottom;
			return (*this);
		}
		constexpr Rect<_Ty>& operator /= (const Rect<_Ty>& rect) noexcept {
			LeftTop /= rect.LeftTop;
			RightBottom /= rect.RightBottom;
			return (*this);
		}
		constexpr Rect<_Ty>& operator %= (const Rect<_Ty>& rect) noexcept {
			LeftTop %= rect.LeftTop;
			RightBottom %= rect.RightBottom;
			return (*this);
		}

		constexpr Rect<_Ty> operator + (const Rect<_Ty>& rect) const noexcept {
			return Rect(LeftTop + rect.LeftTop, RightBottom + rect.RightBottom);
		}
		constexpr Rect<_Ty> operator - (const Rect<_Ty>& rect) const noexcept {
			return Rect(LeftTop - rect.LeftTop, RightBottom - rect.RightBottom);
		}
		constexpr Rect<_Ty> operator * (const Rect<_Ty>& rect) const noexcept {
			return Rect(LeftTop * rect.LeftTop, RightBottom * rect.RightBottom);
		}
		constexpr Rect<_Ty> operator / (const Rect<_Ty>& rect) const noexcept {
			return Rect(LeftTop / rect.LeftTop, RightBottom / rect.RightBottom);
		}
		constexpr Rect<_Ty> operator % (const Rect<_Ty>& rect) const noexcept {
			return Rect(LeftTop % rect.LeftTo, RightBottom % rect.RightBottom);
		}

		constexpr Bool operator == (const Rect<_Ty>& rect) const noexcept {
			return (LeftTop == rect.LeftTop && RightBottom == rect.RightBottom);
		}

		constexpr Rect<_Ty>& operator += (const _Ty& num) noexcept {
			LeftTop += num;
			RightBottom += num;
			return (*this);
		}
		constexpr Rect<_Ty>& operator -= (const _Ty& num) noexcept {
			LeftTop -= num;
			RightBottom -= num;
			return (*this);
		}
		constexpr Rect<_Ty>& operator *= (const _Ty& num) noexcept {
			LeftTop *= num;
			RightBottom *= num;
			return (*this);
		}
		constexpr Rect<_Ty>& operator /= (const _Ty& num) noexcept {
			LeftTop /= num;
			RightBottom /= num;
			return (*this);
		}
		constexpr Rect<_Ty>& operator %= (const _Ty& num) noexcept {
			LeftTop %= num;
			RightBottom %= num;
			return (*this);
		}

		constexpr Rect<_Ty> operator + (const _Ty& num) const noexcept {
			return Rect(LeftTop + num, RightBottom + num);
		}
		constexpr Rect<_Ty> operator - (const _Ty& num) const noexcept {
			return Rect(LeftTop - num, RightBottom - num);
		}
		constexpr Rect<_Ty> operator * (const _Ty& num) const noexcept {
			return Rect(LeftTop * num, RightBottom * num);
		}
		constexpr Rect<_Ty> operator / (const _Ty& num) const noexcept {
			return Rect(LeftTop / num, RightBottom / num);
		}
		constexpr Rect<_Ty> operator % (const _Ty& num) const noexcept {
			return Rect(LeftTop % num, RightBottom % num);
		}

		constexpr Bool operator == (const _Ty& num) const noexcept {
			return (LeftTop == num && RightBottom == num);
		}

		constexpr Rect<_Ty>& operator = (const Rect<_Ty>& rect) noexcept {
			LeftTop = rect.LeftTop;
			RightBottom = rect.RightBottom;
			return *this;
		}
		constexpr const _Ty& operator [] (const uInt& index) const {
			if (index >= 4)
				Raise("Out of range");
			if (index < 2)
				return LeftTop[index];
			return RightBottom[index % 2];
		}
		constexpr _Ty& operator [] (const uInt& index) {
			const Rect* pConstThis = this;
			return const_cast<_Ty&>((*pConstThis)[index]);
		}

#ifdef _WINDEF_
		constexpr operator RECT() const noexcept {
			RECT rect;
			rect.left = static_cast<Long>(Left);
			rect.top = static_cast<Long>(Top);
			rect.right = static_cast<Long>(Right);
			rect.bottom = static_cast<Long>(Bottom);
			return rect;
		}
#endif

		template <typename _U>
		constexpr operator Rect<_U>() const noexcept {
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
			_Ty PointArray[4];
		};
	};
}