#pragma once

namespace Nt {
	template <typename _Ty>
	struct Vector3D {
		constexpr Vector3D() noexcept :
			x(0), y(0), z(0) {
		}
		constexpr Vector3D(const _Ty& X, const _Ty& Y, const _Ty& Z) noexcept :
			x(X), y(Y), z(Z) {
		}
		constexpr Vector3D(const _Ty& X, const Vector2D<_Ty>& YZ) noexcept :
			xyz { X, YZ.x, YZ.y } {
		}
		constexpr Vector3D(const Vector2D<_Ty>& XY, const _Ty& Z) noexcept :
			xyz { XY.x, XY.y, Z } {
		}
		constexpr Vector3D(const Vector2D<_Ty>& Vec2D) noexcept :
			xyz { Vec2D.x, Vec2D.y, 0 } {
		}

		constexpr _Ty Length() const noexcept {
			return sqrt(x * x + y * y + z * z);
		}
		constexpr _Ty LengthSquare() const noexcept {
			return x * x + y * y + z * z;
		}
		constexpr _Ty GetDistance(const Vector3D<_Ty>& Vec) const noexcept {
			return ((*this) - Vec).Length();
		}
		constexpr Vector3D<_Ty> GetNormalize() const noexcept {
			return (*this / Length());
		}
		constexpr Vector3D<_Ty> GetCross(const Vector3D<_Ty>& Vec) const noexcept {
			Vector3D<_Ty> Result;
			Result.x = this->y * Vec.z - this->z * Vec.y;
			Result.y = this->z * Vec.x - this->x * Vec.z;
			Result.z = this->x * Vec.y - this->y * Vec.x;
			return Result;
		}
		constexpr _Ty Dot(const Vector3D<_Ty>& Vec) const noexcept {
			return x * Vec.x + y * Vec.y + z * Vec.z;
		}
		constexpr String ToString() const noexcept {
			String str;
			return str << "{ x: " << x << ", y: " << y << ", z: " << z << " };";
		}

// ============================================================================
//		Operators with vector
// ----------------------------------------------------------------------------
		constexpr Vector3D<_Ty> operator + (const Vector3D<_Ty>& Vec) const noexcept {
			return Vector3D(x + Vec.x, y + Vec.y, z + Vec.z);
		}
		constexpr Vector3D<_Ty> operator - (const Vector3D<_Ty>& Vec) const noexcept {
			return Vector3D(x - Vec.x, y - Vec.y, z - Vec.z);
		}
		constexpr Vector3D<_Ty> operator * (const Vector3D<_Ty>& Vec) const noexcept {
			return Vector3D(x * Vec.x, y * Vec.y, z * Vec.z);
		}
		constexpr Vector3D<_Ty> operator / (const Vector3D<_Ty>& Vec) const noexcept {
			return Vector3D(x / Vec.x, y / Vec.y, z / Vec.z);
		}
		constexpr Vector3D<_Ty> operator % (const Vector3D<_Ty>& Vec) const noexcept {
			return Vector3D(x % Vec.x, y % Vec.y, z % Vec.z);
		}

		constexpr Vector3D<_Ty> operator += (const Vector3D<_Ty>& Vec) noexcept {
			this->x += Vec.x;
			this->y += Vec.y;
			this->z += Vec.z;
			return *this;
		}
		constexpr Vector3D<_Ty> operator -= (const Vector3D<_Ty>& Vec) noexcept {
			this->x -= Vec.x;
			this->y -= Vec.y;
			this->z -= Vec.z;
			return *this;
		}
		constexpr Vector3D<_Ty> operator *= (const Vector3D<_Ty>& Vec) noexcept {
			this->x *= Vec.x;
			this->y *= Vec.y;
			this->z *= Vec.z;
			return *this;
		}
		constexpr Vector3D<_Ty> operator /= (const Vector3D<_Ty>& Vec) noexcept {
			this->x /= Vec.x;
			this->y /= Vec.y;
			this->z /= Vec.z;
			return *this;
		}
		constexpr Vector3D<_Ty> operator %= (const Vector3D<_Ty>& Vec) noexcept {
			this->x %= Vec.x;
			this->y %= Vec.y;
			this->z %= Vec.z;
			return *this;
		}

		constexpr Bool operator > (const Vector3D<_Ty>& Vec) const noexcept {
			return (x > Vec.x && y > Vec.y && z > Vec.z);
		}
		constexpr Bool operator < (const Vector3D<_Ty>& Vec) const noexcept {
			return (x < Vec.x && y < Vec.y && z < Vec.z);
		}
		constexpr Bool operator >= (const Vector3D<_Ty>& Vec) const noexcept {
			return (x >= Vec.x && y >= Vec.y && z >= Vec.z);
		}
		constexpr Bool operator <= (const Vector3D<_Ty>& Vec) const noexcept {
			return (x <= Vec.x && y <= Vec.y && z <= Vec.z);
		}
		constexpr Bool operator == (const Vector3D<_Ty>& Vec) const noexcept {
			return (x == Vec.x && y == Vec.y && z == Vec.z);
		}
		constexpr Bool operator != (const Vector3D<_Ty>& Vec) const noexcept {
			return (x != Vec.x || y != Vec.y || z != Vec.z);
		}

// ============================================================================
//		Operators with nums
// ----------------------------------------------------------------------------
		constexpr Vector3D<_Ty> operator + (const _Ty& num) const noexcept {
			return Vector3D(x + num, y + num, z + num);
		}
		constexpr Vector3D<_Ty> operator - (const _Ty& num) const noexcept {
			return Vector3D(x - num, y - num, z - num);
		}
		constexpr Vector3D<_Ty> operator * (const _Ty& num) const noexcept {
			return Vector3D(x * num, y * num, z * num);
		}
		constexpr Vector3D<_Ty> operator / (const _Ty& num) const noexcept {
			if (num == 0)
				return { };
			return Vector3D(x / num, y / num, z / num);
		}
		constexpr Vector3D<_Ty> operator % (const _Ty& num) const noexcept {
			if (num == 0)
				return { };
			return Vector3D(x % num, y % num, z % num);
		}

		friend constexpr Vector3D<_Ty> operator + (const _Ty& num, const Vector3D<_Ty>& Vec) noexcept {
			return Vector3D(num + Vec.x, num + Vec.y, num + Vec.z);
		}
		friend constexpr Vector3D<_Ty> operator - (const _Ty& num, const Vector3D<_Ty>& Vec) noexcept {
			return Vector3D(num - Vec.x, num - Vec.y, num - Vec.z);
		}
		friend constexpr Vector3D<_Ty> operator * (const _Ty& num, const Vector3D<_Ty>& Vec) noexcept {
			return Vector3D(num * Vec.x, num * Vec.y, num * Vec.z);
		}
		friend constexpr Vector3D<_Ty> operator / (const _Ty& num, const Vector3D<_Ty>& Vec) noexcept {
			return Vector3D(num / Vec.x, num / Vec.y, num / Vec.z);
		}
		friend constexpr Vector3D<_Ty> operator % (const _Ty& num, const Vector3D<_Ty>& Vec) noexcept {
			return Vector3D(num % Vec.x, num % Vec.y, num % Vec.z);
		}

		constexpr Vector3D<_Ty> operator += (const _Ty& num) noexcept {
			this->x += num;
			this->y += num;
			this->z += num;
			return *this;
		}
		constexpr Vector3D<_Ty> operator -= (const _Ty& num) noexcept {
			this->x -= num;
			this->y -= num;
			this->z -= num;
			return *this;
		}
		constexpr Vector3D<_Ty> operator *= (const _Ty& num) noexcept {
			this->x *= num;
			this->y *= num;
			this->z *= num;
			return *this;
		}
		constexpr Vector3D<_Ty> operator /= (const _Ty& num) noexcept {
			this->x /= num;
			this->y /= num;
			this->z /= num;
			return *this;
		}
		constexpr Vector3D<_Ty> operator %= (const _Ty& num) noexcept {
			this->x %= num;
			this->y %= num;
			this->z %= num;
			return *this;
		}

		constexpr Bool operator > (const _Ty& num) const noexcept {
			return (x > num && y > num && z > num);
		}
		constexpr Bool operator < (const _Ty& num) const noexcept {
			return (x < num&& y < num&& z < num);
		}
		constexpr Bool operator >= (const _Ty& num) const noexcept {
			return (x >= num && y >= num && z >= num);
		}
		constexpr Bool operator <= (const _Ty& num) const noexcept {
			return (x <= num && y <= num && z <= num);
		}
		constexpr Bool operator == (const _Ty& num) const noexcept {
			return (x == num && y == num && z == num);
		}
		constexpr Bool operator != (const _Ty& num) const noexcept {
			return (x != num || y != num || z != num);
		}

// ============================================================================
//		Others operators
// ----------------------------------------------------------------------------
		constexpr const _Ty& operator [] (const uInt& index) const {
			if (index >= 3)
				Raise("Vector3D: Index out of range");
			return xyz[index];
		}
		constexpr _Ty& operator [] (const uInt& index) {
			if (index >= 3)
				Raise("Vector3D: Index out of range");
			return xyz[index];
		}

		constexpr Vector3D<_Ty> operator + () const noexcept {
			return *this;
		}
		constexpr Vector3D<_Ty> operator - () const noexcept {
			return (*this) * (_Ty)(-1);
		}

		constexpr Vector3D<_Ty> operator = (const Vector3D<_Ty>& Vec) const noexcept {
			if (*this != Vec)
				memcpy(const_cast<void*>(reinterpret_cast<const void*>(this)), &Vec, sizeof(Vec));
			return (*this);
		}

		template <typename _U>
		constexpr operator Vector3D<_U>() const noexcept {
			return Vector3D<_U>(_U(x), _U(y), _U(z));
		}
		constexpr operator Vector2D<_Ty>() const noexcept {
			return Vector2D(x, y);
		}

// ============================================================================
//		Vectors
// ----------------------------------------------------------------------------
		static const Vector3D<_Ty> Zero;
		static const Vector3D<_Ty> Identity;

		static const Vector3D<_Ty> Left;
		static const Vector3D<_Ty> Right;
		static const Vector3D<_Ty> Down;
		static const Vector3D<_Ty> Up;
		static const Vector3D<_Ty> Back;
		static const Vector3D<_Ty> Forward;

		union {
			struct {
				_Ty x, y, z;
			};
			struct {
				Vector2D<_Ty> xy;
				_Ty z;
			};
			struct {
				_Ty x;
				Vector2D<_Ty> yz;
			};
			_Ty xyz[3];

			struct {
				_Ty r, g, b;
			};
			struct {
				Vector2D<_Ty> rg;
				_Ty b;
			};
			struct {
				_Ty r;
				Vector2D<_Ty> gb;
			};
			_Ty rgb[3];
		};
	};

	template <typename _Ty>
	const Vector3D<_Ty> Vector3D<_Ty>::Zero = { 0.f, 0.f, 0.f };

	template <typename _Ty>
	const Vector3D<_Ty> Vector3D<_Ty>::Identity = { 1.f, 1.f, 1.f };

	template <typename _Ty>
	const Vector3D<_Ty> Vector3D<_Ty>::Left = { -1.f, 0.f, 0.f };

	template <typename _Ty>
	const Vector3D<_Ty> Vector3D<_Ty>::Right = { 1.f, 0.f, 0.f };

	template <typename _Ty>
	const Vector3D<_Ty> Vector3D<_Ty>::Down = { 0.f, -1.f, 0.f };

	template <typename _Ty>
	const Vector3D<_Ty> Vector3D<_Ty>::Up = { 0.f, 1.f, 0.f };

	template <typename _Ty>
	const Vector3D<_Ty> Vector3D<_Ty>::Back = { 0.f, 0.f, -1.f };

	template <typename _Ty>
	const Vector3D<_Ty> Vector3D<_Ty>::Forward = { 0.f, 0.f, 1.f };
}