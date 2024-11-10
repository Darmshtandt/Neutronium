#pragma once

namespace Nt {
	template <typename _Ty>
	struct Vector2D {
		constexpr Vector2D() :
			x(0), y(0) 
		{
		}
#ifdef _WINDEF_
		constexpr Vector2D(const POINT& Point) :
			x(Point.x), y(Point.y)
		{ 
		}
		constexpr Vector2D(const SIZE& Size) :
			x(Size.cx), y(Size.cy)
		{ 
		}
#endif
		constexpr Vector2D(const _Ty& X, const _Ty& Y) :
			x(X), y(Y)
		{ 
		}

		constexpr _Ty Length() const noexcept {
			return _Ty(sqrt(Double(x * x + y * y)));
		}
		constexpr _Ty LengthSquare() const noexcept {
			return (x * x + y * y);
		}
		constexpr _Ty Distance(const Vector2D<_Ty>& Vec) const noexcept {
			return ((*this) - Vec).Length();
		}
		constexpr Vector2D<_Ty> GetNormalize() const noexcept {
			return (*this / Length());
		}
		constexpr _Ty GetCross(const Vector2D<_Ty>& Vec) const noexcept {
			return (x * Vec.y) - (Vec.x * y);
		}
		constexpr _Ty Dot(const Vector2D<_Ty>& Vec) const noexcept {
			return x * Vec.x + y * Vec.y;
		}
		constexpr String ToString() const noexcept {
			String str;
			return str << ("{ x: ") << String(x) << ", y: " << String(y) << " };";
		}

		constexpr Vector2D<_Ty> GetClamp(const _Ty& min, const _Ty& max) const noexcept {
			Vector2D<_Ty> vector = (*this);
			for (uInt i = 0; i < 2; ++i) {
				if (vector.xy[i] < min)
					vector.xy[i] = min;
				else if (vector.xy[i] > max)
					vector.xy[i] = max;
			}

			return vector;
		}

// ============================================================================
//		Operators with vector
// ----------------------------------------------------------------------------
		constexpr Vector2D<_Ty> operator + (const Vector2D<_Ty>& Vec) const noexcept {
			return Vector2D(x + Vec.x, y + Vec.y);
		}
		constexpr Vector2D<_Ty> operator - (const Vector2D<_Ty>& Vec) const noexcept {
			return Vector2D(x - Vec.x, y - Vec.y);
		}
		constexpr Vector2D<_Ty> operator * (const Vector2D<_Ty>& Vec) const noexcept {
			return Vector2D(x * Vec.x, y * Vec.y);
		}
		constexpr Vector2D<_Ty> operator / (const Vector2D<_Ty>& Vec) const noexcept {
			return Vector2D(x / Vec.x, y / Vec.y);
		}
		constexpr Vector2D<_Ty> operator % (const Vector2D<_Ty>& Vec) const noexcept {
			return Vector2D(x % Vec.x, y % Vec.y);
		}
	
		constexpr Vector2D<_Ty> operator += (const Vector2D<_Ty>& Vec) noexcept {
			this->x += Vec.x;
			this->y += Vec.y;
			return *this;
		}
		constexpr Vector2D<_Ty> operator -= (const Vector2D<_Ty>& Vec) noexcept {
			this->x -= Vec.x;
			this->y -= Vec.y;
			return *this;
		}
		constexpr Vector2D<_Ty> operator *= (const Vector2D<_Ty>& Vec) noexcept {
			this->x *= Vec.x;
			this->y *= Vec.y;
			return *this;
		}
		constexpr Vector2D<_Ty> operator /= (const Vector2D<_Ty>& Vec) noexcept {
			this->x /= Vec.x;
			this->y /= Vec.y;
			return *this;
		}
		constexpr Vector2D<_Ty> operator %= (const Vector2D<_Ty>& Vec) noexcept {
			this->x %= Vec.x;
			this->y %= Vec.y;
			return *this;
		}

		constexpr Bool operator > (const Vector2D<_Ty>& Vec) const noexcept {
			return (x > Vec.x && y > Vec.y);
		}
		constexpr Bool operator < (const Vector2D<_Ty>& Vec) const noexcept {
			return (x < Vec.x && y < Vec.y);
		}
		constexpr Bool operator >= (const Vector2D<_Ty>& Vec) const noexcept {
			return (x >= Vec.x && y >= Vec.y);
		}
		constexpr Bool operator <= (const Vector2D<_Ty>& Vec) const noexcept {
			return (x <= Vec.x && y <= Vec.y);
		}
		constexpr Bool operator == (const Vector2D<_Ty>& Vec) const noexcept {
			return (x == Vec.x && y == Vec.y);
		}
		constexpr Bool operator != (const Vector2D<_Ty>& Vec) const noexcept {
			return (x != Vec.x || y != Vec.y);
		}
	
// ============================================================================
//		Operators with nums
// ----------------------------------------------------------------------------
		constexpr Vector2D<_Ty> operator + (const _Ty& num) const noexcept {
			return Vector2D(x + num, y + num);
		}
		constexpr Vector2D<_Ty> operator - (const _Ty& num) const noexcept {
			return Vector2D(x - num, y - num);
		}
		constexpr Vector2D<_Ty> operator * (const _Ty& num) const noexcept {
			return Vector2D(x * num, y * num);
		}
		constexpr Vector2D<_Ty> operator / (const _Ty& num) const noexcept {
			return Vector2D(x / num, y / num);
		}
		constexpr Vector2D<_Ty> operator % (const _Ty& num) const noexcept {
			return Vector2D(x % num, y % num);
		}
	
		friend constexpr Vector2D<_Ty> operator + (const _Ty& num, const Vector2D<_Ty>& Vec) noexcept {
			return Vector2D(num + Vec.x, num + Vec.y);
		}
		friend constexpr Vector2D<_Ty> operator - (const _Ty& num, const Vector2D<_Ty>& Vec) noexcept {
			return Vector2D(num - Vec.x, num - Vec.y);
		}
		friend constexpr Vector2D<_Ty> operator * (const _Ty& num, const Vector2D<_Ty>& Vec) noexcept {
			return Vector2D(num * Vec.x, num * Vec.y);
		}
		friend constexpr Vector2D<_Ty> operator / (const _Ty& num, const Vector2D<_Ty>& Vec) noexcept {
			return Vector2D(num / Vec.x, num / Vec.y);
		}
		friend constexpr Vector2D<_Ty> operator % (const _Ty& num, const Vector2D<_Ty>& Vec) noexcept {
			return Vector2D(num % Vec.x, num % Vec.y);
		}
	
		constexpr Vector2D<_Ty> operator += (const _Ty& num) noexcept {
			this->x += num;
			this->y += num;
			return *this;
		}
		constexpr Vector2D<_Ty> operator -= (const _Ty& num) noexcept {
			this->x -= num;
			this->y -= num;
			return *this;
		}
		constexpr Vector2D<_Ty> operator *= (const _Ty& num) noexcept {
			this->x *= num;
			this->y *= num;
			return *this;
		}
		constexpr Vector2D<_Ty> operator /= (const _Ty& num) noexcept {
			this->x /= num;
			this->y /= num;
			return *this;
		}
		constexpr Vector2D<_Ty> operator %= (const _Ty& num) noexcept {
			this->x %= num;
			this->y %= num;
			return *this;
		}

		constexpr Bool operator > (const _Ty& num) const noexcept {
			return (x > num && y > num);
		}
		constexpr Bool operator < (const _Ty& num) const noexcept {
			return (x < num && y < num);
		}
		constexpr Bool operator >= (const _Ty& num) const noexcept {
			return (x >= num && y >= num);
		}
		constexpr Bool operator <= (const _Ty& num) const noexcept {
			return (x <= num && y <= num);
		}
		constexpr Bool operator == (const _Ty& num) const noexcept {
			return (x == num && y == num);
		}
		constexpr Bool operator != (const _Ty& num) const noexcept {
			return (x != num || y != num);
		}
	
// ============================================================================
//		Others operators
// ----------------------------------------------------------------------------
		constexpr const _Ty& operator [] (const uInt& index) const {
			if (index >= 2)
				Raise("Vector2D: Index out of range");
			return xy[index];
		}
		constexpr _Ty& operator [] (const uInt& index) {
			if (index >= 2)
				Raise("Vector2D: Index out of range");
			return xy[index];
		}

		constexpr Vector2D<_Ty> operator + () const noexcept {
			return (*this);
		}
		constexpr Vector2D<_Ty> operator - () const noexcept {
			return (*this) * (-1);
		}

		template <typename _U>
		constexpr operator Vector2D<_U>() const noexcept {
			return Vector2D<_U>(_U(x), _U(y));
		}

#ifdef _WINDEF_
		constexpr operator POINT() const noexcept {
			return { Long(x), Long(y)};
		}
		constexpr operator SIZE() const noexcept {
			return { Long(x), Long(y)};
		}
#endif

		union {
			struct {
				_Ty x, y;
			};
			_Ty xy[2];

			struct {
				_Ty r, g;
			};
			_Ty rg[2];
		};
	};
	
	template <typename _Ty>
	constexpr Vector2D<_Ty> Zero2D = { 0, 0 };
	
	template <typename _Ty>
	constexpr Vector2D<_Ty> Identity2D = { 1, 1 };
	
	template <typename _Ty>
	constexpr Vector2D<_Ty> Left2D = { _Ty(-1), 0};
	
	template <typename _Ty>
	constexpr Vector2D<_Ty> Right2D = { 1, 0 };
	
	template <typename _Ty>
	constexpr Vector2D<_Ty> Down2D = { 0, _Ty(-1) };
	
	template <typename _Ty>
	constexpr Vector2D<_Ty> Up2D = { 0, 1 };
}