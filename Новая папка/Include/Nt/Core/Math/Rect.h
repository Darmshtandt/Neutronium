#pragma once

namespace Nt {
	template <typename _Ty>
	struct Rect {
		constexpr Rect() noexcept :
			LeftTop(), 
			RightBottom() {
		}
		constexpr Rect(const Vector2D<_Ty>& LeftTop, const Vector2D<_Ty>& RightBottom) noexcept :
			LeftTop(LeftTop),
			RightBottom(RightBottom) {
		}
		constexpr Rect(const _Ty& Left, const _Ty& Top, const _Ty& Right, const _Ty& Bottom) noexcept :
			Left(Left),
			Top(Top),
			Right(Right),
			Bottom(Bottom) {
		}
#ifdef _WINDEF_
		constexpr Rect(const RECT& Rect) noexcept :
			Left(Rect.left),
			Top(Rect.top),
			Right(Rect.right),
			Bottom(Rect.bottom) {
		}
#endif

		constexpr Bool Intersect(const Rect<_Ty>& SecondaryRect) const noexcept {
			return (LeftTop < (SecondaryRect.LeftTop + SecondaryRect.RightBottom) && SecondaryRect.LeftTop < (LeftTop + RightBottom));
		}

		constexpr Rect<_Ty>& operator = (const Rect<_Ty>& Rect) noexcept {
			LeftTop = Rect.LeftTop;
			RightBottom = Rect.RightBottom;
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
			RECT Rect;
			Rect.left = static_cast<Long>(Left);
			Rect.top = static_cast<Long>(Top);
			Rect.right = static_cast<Long>(Right);
			Rect.bottom = static_cast<Long>(Bottom);
			return Rect;
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

		};
	};
}