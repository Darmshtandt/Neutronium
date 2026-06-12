#include <Nt/Core/Math/Rect.h>

namespace Nt {
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>::Rect() noexcept :
		LeftTop(),
		RightBottom()
	{
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>::Rect(const Vector2D<_Ty>& leftTop, const Vector2D<_Ty>& rightBottom) noexcept :
		LeftTop(leftTop),
		RightBottom(rightBottom)
	{
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>::Rect(const _Ty& left, const _Ty& top, const _Ty& right, const _Ty& bottom) noexcept :
		Left(left),
		Top(top),
		Right(right),
		Bottom(bottom)
	{
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>::Rect(const Rect& rect) noexcept :
		LeftTop(rect.LeftTop),
		RightBottom(rect.RightBottom)
	{
	}

	template <typename _Ty>
	constexpr Rect<_Ty>::Rect(Rect&& rect) noexcept :
		LeftTop(std::move(rect.LeftTop)),
		RightBottom(std::move(rect.RightBottom))
	{
	}
#ifdef _WINDEF_
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>::Rect(const RECT& rect) noexcept :
		Left(_Ty(rect.left)),
		Top(_Ty(rect.top)),
		Right(_Ty(rect.right)),
		Bottom(_Ty(rect.bottom))
	{
	}
#endif

	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::Copy() const noexcept {
		return Rect(*this);
	}

	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::Fill(const _Ty& value) noexcept {
		LeftTop.Fill(value);
		RightBottom.Fill(value);
		return *this;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::Fill(const Vector2D<_Ty>& vector) noexcept {
		LeftTop = vector;
		RightBottom = vector;
		return *this;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::Inflate(const Vector2D<_Ty>& vector) noexcept {
		LeftTop -= vector;
		RightBottom += vector * _Ty(2);
		return *this;
	}

	template <typename _Ty>
	NT_CONSTEXPR Bool Rect<_Ty>::Intersect(const Rect<_Ty>& otherRect) const noexcept {
		return (LeftTop < (otherRect.LeftTop + otherRect.RightBottom) && otherRect.LeftTop < (LeftTop + RightBottom));
	}
	template <typename _Ty>
	NT_CONSTEXPR Bool Rect<_Ty>::Intersect(const Vector2D<_Ty>& point) const noexcept {
		return (point.x >= Left && point.x <= (Left + Right) && point.y >= Top && point.y <= (Top + Bottom));
	}

	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::GetClamp(const _Ty& min, const _Ty& max) {
		Rect<_Ty> rect = (*this);
		for (uInt i = 0; i < 4; ++i) {
			if (rect.Array[i] < min)
				rect.Array[i] = min;
			else if (rect.Array[i] > max)
				rect.Array[i] = max;
		}

		return rect;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::GetClamp(const Vector2D<_Ty>& min, const Vector2D<_Ty>& max) {
		return Rect<_Ty>(LeftTop.GetClamp(min.x, max.x), RightBottom.GetClamp(min.y, max.y));
	}

	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator += (const Rect<_Ty>& rect) noexcept {
		LeftTop += rect.LeftTop;
		RightBottom += rect.RightBottom;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator -= (const Rect<_Ty>& rect) noexcept {
		LeftTop -= rect.LeftTop;
		RightBottom -= rect.RightBottom;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator *= (const Rect<_Ty>& rect) noexcept {
		LeftTop *= rect.LeftTop;
		RightBottom *= rect.RightBottom;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator /= (const Rect<_Ty>& rect) noexcept {
		LeftTop /= rect.LeftTop;
		RightBottom /= rect.RightBottom;
		return (*this);
	}
	template <typename _Ty>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator %= (const Rect<_Ty>& rect) noexcept {
		LeftTop %= rect.LeftTop;
		RightBottom %= rect.RightBottom;
		return (*this);
	}

	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator + (const Rect<_Ty>& rect) const noexcept {
		return Copy() += rect;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator - (const Rect<_Ty>& rect) const noexcept {
		return Copy() -= rect;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator * (const Rect<_Ty>& rect) const noexcept {
		return Copy() *= rect;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator / (const Rect<_Ty>& rect) const noexcept {
		return Copy() /= rect;
	}
	template <typename _Ty>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator % (const Rect<_Ty>& rect) const noexcept {
		return Copy() %= rect;
	}

	template <typename _Ty>
	NT_CONSTEXPR Bool Rect<_Ty>::operator == (const Rect<_Ty>& rect) const noexcept {
		return (LeftTop == rect.LeftTop && RightBottom == rect.RightBottom);
	}


	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator += (const Vector2D<_Ty>& vector) noexcept {
		LeftTop += vector;
		RightBottom += vector;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator -= (const Vector2D<_Ty>& vector) noexcept {
		LeftTop -= vector;
		RightBottom -= vector;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator *= (const Vector2D<_Ty>& vector) noexcept {
		LeftTop *= vector;
		RightBottom *= vector;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator /= (const Vector2D<_Ty>& vector) noexcept {
		LeftTop /= vector;
		RightBottom /= vector;
		return (*this);
	}
	template <typename _Ty>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator %= (const Vector2D<_Ty>& vector) noexcept {
		LeftTop %= vector;
		RightBottom %= vector;
		return (*this);
	}

	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator + (const Vector2D<_Ty>& vector) const noexcept {
		return Copy() += vector;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator - (const Vector2D<_Ty>& vector) const noexcept {
		return Copy() -= vector;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator * (const Vector2D<_Ty>& vector) const noexcept {
		return Copy() *= vector;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator / (const Vector2D<_Ty>& vector) const noexcept {
		return Copy() /= vector;
	}
	template <typename _Ty>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator % (const Vector2D<_Ty>& vector) const noexcept {
		return Copy() %= vector;
	}


	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator += (const _Ty& num) noexcept {
		LeftTop += num;
		RightBottom += num;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator -= (const _Ty& num) noexcept {
		LeftTop -= num;
		RightBottom -= num;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator *= (const _Ty& num) noexcept {
		LeftTop *= num;
		RightBottom *= num;
		return (*this);
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator /= (const _Ty& num) noexcept {
		LeftTop /= num;
		RightBottom /= num;
		return (*this);
	}
	template <typename _Ty>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator %= (const _Ty& num) noexcept {
		LeftTop %= num;
		RightBottom %= num;
		return (*this);
	}

	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator + (const _Ty& num) const noexcept {
		return Copy() += num;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator - (const _Ty& num) const noexcept {
		return Copy() -= num;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator * (const _Ty& num) const noexcept {
		return Copy() *= num;
	}
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator / (const _Ty& num) const noexcept {
		return Copy() /= num;
	}
	template <typename _Ty>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Rect<_Ty> Rect<_Ty>::operator % (const _Ty& num) const noexcept {
		return Copy() %= num;
	}

	template <typename _Ty>
	NT_CONSTEXPR Bool Rect<_Ty>::operator == (const _Ty& num) const noexcept {
		return (LeftTop == num && RightBottom == num);
	}

	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>& Rect<_Ty>::operator = (const Rect<_Ty>& rect) noexcept {
		LeftTop = rect.LeftTop;
		RightBottom = rect.RightBottom;

		return *this;
	}

	template <typename _Ty>
	constexpr Rect<_Ty>& Rect<_Ty>::operator=(Rect<_Ty>&& rect) noexcept {
		if (this == &rect)
			return *this;

		LeftTop = std::move(rect.LeftTop);
		RightBottom = std::move(rect.RightBottom);
		return *this;
	}

	template <typename _Ty>
	NT_CONSTEXPR const _Ty& Rect<_Ty>::operator [] (const uInt& index) const {
		if (index >= 4)
			Raise("Out of range");

		if (index < 2)
			return LeftTop[index];

		return RightBottom[index % 2];
	}
	template <typename _Ty>
	NT_CONSTEXPR _Ty& Rect<_Ty>::operator [] (const uInt& index) {
		const Rect* pConstThis = this;
		return const_cast<_Ty&>((*pConstThis)[index]);
	}

#ifdef _WINDEF_
	template <typename _Ty>
	NT_CONSTEXPR Rect<_Ty>::operator RECT() const noexcept {
		RECT rect;
		rect.left = static_cast<Long>(Left);
		rect.top = static_cast<Long>(Top);
		rect.right = static_cast<Long>(Right);
		rect.bottom = static_cast<Long>(Bottom);
		return rect;
	}
#endif

	template <typename _Ty>
	template <typename _U>
	NT_CONSTEXPR Rect<_Ty>::operator Rect<_U>() const noexcept {
		return {
			Vector2D<_U>(LeftTop),
			Vector2D<_U>(RightBottom)
		};
	}
}