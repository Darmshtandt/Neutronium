#pragma once

namespace Nt {
	template <typename _Ty>
	struct Vector4D {
		constexpr Vector4D() :
			xy(), zw() {
		}
		constexpr Vector4D(const _Ty& X, const _Ty& Y, const _Ty& Z, const _Ty& W) :
			x(X), y(Y), z(Z), w(W) {
		}
		constexpr Vector4D(const Vector2D<_Ty>& XY, const _Ty& Z, const _Ty& W) :
			xy(XY), zw(Z, W) {
		}
		constexpr Vector4D(const _Ty& X, const Vector2D<_Ty>& YZ, const _Ty& W) :
			x(X), yz(YZ), w(W) {
		}
		constexpr Vector4D(const _Ty& X, const _Ty& Y, const Vector2D<_Ty>& ZW) :
			xy(X, Y), zw(ZW) {
		}
		constexpr Vector4D(const Vector2D<_Ty>& XY, const Vector2D<_Ty>& ZW) :
			xy(XY), zw(ZW) {
		}
		constexpr Vector4D(const Vector3D<_Ty>& XYZ, const _Ty& W) :
			xyzw { XYZ.x, XYZ.y, XYZ.z, W } {
		}
		constexpr Vector4D(const _Ty& X, const Vector3D<_Ty>& YZW) :
			xyzw { X, YZW.x, YZW.y, YZW.z } {
		}
		constexpr Vector4D(const Vector2D<_Ty>& Vec2D) :
			xy(Vec2D), zw(0, 1) {
		}
		constexpr Vector4D(const Vector3D<_Ty>& Vec3D) :
			xyzw { Vec3D.x, Vec3D.y, Vec3D.z, 1 } {
		}

		constexpr _Ty Length() const noexcept {
			return sqrt(x * x + y * y + z * z);
		}
		constexpr _Ty LengthSquare() const noexcept {
			return x * x + y * y + z * z;
		}
		constexpr _Ty GetDistance(const Vector4D<_Ty>& Vec) const noexcept {
			return ((*this) - Vec).Length();
		}
		constexpr Vector4D<_Ty> GetNormalize() const noexcept {
			return (*this / Length());
		}
		constexpr _Ty Dot(const Vector4D<_Ty>& Vec) const noexcept {
			return x * Vec.x + y * Vec.y + z * Vec.z + w * Vec.w;
		}
		constexpr String ToString() const noexcept {
			String str;
			return str << "{ x: " << x << ", y: " << y << ", z: " << z << ", w: " << w << " };";
		}

// ============================================================================
//		Operators with vector
// ----------------------------------------------------------------------------
		constexpr Vector4D<_Ty> operator + (const Vector4D<_Ty>& Vec) const noexcept {
			return Vector4D(x + Vec.x, y + Vec.y, z + Vec.z, w + Vec.w);
		}
		constexpr Vector4D<_Ty> operator - (const Vector4D<_Ty>& Vec) const noexcept {
			return Vector4D(x - Vec.x, y - Vec.y, z - Vec.z, w - Vec.w);
		}
		constexpr Vector4D<_Ty> operator * (const Vector4D<_Ty>& Vec) const noexcept {
			return Vector4D(x * Vec.x, y * Vec.y, z * Vec.z, w * Vec.w);
		}
		constexpr Vector4D<_Ty> operator / (const Vector4D<_Ty>& Vec) const noexcept {
			return Vector4D(x / Vec.x, y / Vec.y, z / Vec.z, w / Vec.w);
		}
		constexpr Vector4D<_Ty> operator % (const Vector4D<_Ty>& Vec) const noexcept {
			return Vector4D(x % Vec.x, y % Vec.y, z % Vec.z, w % Vec.w);
		}

		friend constexpr Vector4D<_Ty> operator + (const _Ty& num, const Vector4D<_Ty>& Vec) noexcept {
			return Vector4D(num + Vec.x, num + Vec.y, num + Vec.z, num + Vec.w);
		}
		friend constexpr Vector4D<_Ty> operator - (const _Ty& num, const Vector4D<_Ty>& Vec) noexcept {
			return Vector4D(num - Vec.x, num - Vec.y, num - Vec.z, num - Vec.w);
		}
		friend constexpr Vector4D<_Ty> operator * (const _Ty& num, const Vector4D<_Ty>& Vec) noexcept {
			return Vector4D(num * Vec.x, num * Vec.y, num * Vec.z, num * Vec.w);
		}
		friend constexpr Vector4D<_Ty> operator / (const _Ty& num, const Vector4D<_Ty>& Vec) noexcept {
			return Vector4D(num / Vec.x, num / Vec.y, num / Vec.z, num / Vec.w);
		}
		friend constexpr Vector4D<_Ty> operator % (const _Ty& num, const Vector4D<_Ty>& Vec) noexcept {
			return Vector4D(num % Vec.x, num % Vec.y, num % Vec.z, num % Vec.w);
		}

		constexpr Vector4D<_Ty> operator += (Vector4D<_Ty> Vec) noexcept {
			this->x += Vec.x;
			this->y += Vec.y;
			this->z += Vec.z;
			this->w += Vec.w;
			return *this;
		}
		constexpr Vector4D<_Ty> operator -= (Vector4D<_Ty> Vec) noexcept {
			this->x -= Vec.x;
			this->y -= Vec.y;
			this->z -= Vec.z;
			this->w -= Vec.w;
			return *this;
		}
		constexpr Vector4D<_Ty> operator *= (Vector4D<_Ty> Vec) noexcept {
			this->x *= Vec.x;
			this->y *= Vec.y;
			this->z *= Vec.z;
			this->w *= Vec.w;
			return *this;
		}
		constexpr Vector4D<_Ty> operator /= (Vector4D<_Ty> Vec) noexcept {
			this->x /= Vec.x;
			this->y /= Vec.y;
			this->z /= Vec.z;
			this->w /= Vec.w;
			return *this;
		}
		constexpr Vector4D<_Ty> operator %= (Vector4D<_Ty> Vec) noexcept {
			this->x %= Vec.x;
			this->y %= Vec.y;
			this->z %= Vec.z;
			this->w %= Vec.w;
			return *this;
		}

		constexpr bool operator > (const Vector4D<_Ty>& Vec) const noexcept {
			return (x > Vec.x && y > Vec.y && z > Vec.z && w > Vec.w);
		}
		constexpr bool operator < (const Vector4D<_Ty>& Vec) const noexcept {
			return (x < Vec.x&& y < Vec.y&& z < Vec.z&& w < Vec.w);
		}
		constexpr bool operator >= (const Vector4D<_Ty>& Vec) const noexcept {
			return (x >= Vec.x && y >= Vec.y && z >= Vec.z && w >= Vec.w);
		}
		constexpr bool operator <= (const Vector4D<_Ty>& Vec) const noexcept {
			return (x <= Vec.x && y <= Vec.y && z <= Vec.z && w <= Vec.w);
		}
		constexpr bool operator == (const Vector4D<_Ty>& Vec) const noexcept {
			return (x == Vec.x && y == Vec.y && z == Vec.z && w == Vec.w);
		}
		constexpr bool operator != (const Vector4D<_Ty>& Vec) const noexcept {
			return (x != Vec.x || y != Vec.y || z != Vec.z || w != Vec.w);
		}

// ============================================================================
//		Operators with nums
// ----------------------------------------------------------------------------
		constexpr Vector4D<_Ty> operator + (const _Ty& num) const noexcept {
			return Vector4D(x + num, y + num, z + num, w + num);
		}
		constexpr Vector4D<_Ty> operator - (const _Ty& num) const noexcept {
			return Vector4D(x - num, y - num, z - num, w - num);
		}
		constexpr Vector4D<_Ty> operator * (const _Ty& num) const noexcept {
			return Vector4D(x * num, y * num, z * num, w * num);
		}
		constexpr Vector4D<_Ty> operator / (const _Ty& num) const noexcept {
			return Vector4D(x / num, y / num, z / num, w / num);
		}
		constexpr Vector4D<_Ty> operator % (const _Ty& num) const noexcept {
			return Vector4D(x % num, y % num, z % num, w % num);
		}

		constexpr Vector4D<_Ty> operator += (const _Ty& num) noexcept {
			this->x += num;
			this->y += num;
			this->z += num;
			this->w += num;
			return *this;
		}
		constexpr Vector4D<_Ty> operator -= (const _Ty& num) noexcept {
			this->x -= num;
			this->y -= num;
			this->z -= num;
			this->w -= num;
			return *this;
		}
		constexpr Vector4D<_Ty> operator *= (const _Ty& num) noexcept {
			this->x *= num;
			this->y *= num;
			this->z *= num;
			this->w *= num;
			return *this;
		}
		constexpr Vector4D<_Ty> operator /= (const _Ty& num) noexcept {
			this->x /= num;
			this->y /= num;
			this->z /= num;
			this->w /= num;
			return *this;
		}
		constexpr Vector4D<_Ty> operator %= (const _Ty& num) noexcept {
			this->x %= num;
			this->y %= num;
			this->z %= num;
			this->w %= num;
			return *this;
		}

		constexpr bool operator > (const _Ty& num) const noexcept {
			return (x > num && y > num && z > num && w > num);
		}
		constexpr bool operator < (const _Ty& num) const noexcept {
			return (x < num&& y < num&& z < num&& w < num);
		}
		constexpr bool operator >= (const _Ty& num) const noexcept {
			return (x >= num && y >= num && z >= num && w >= num);
		}
		constexpr bool operator <= (const _Ty& num) const noexcept {
			return (x <= num && y <= num && z <= num && w <= num);
		}
		constexpr bool operator == (const _Ty& num) const noexcept {
			return (x == num && y == num && z == num && w == num);
		}
		constexpr bool operator != (const _Ty& num) const noexcept {
			return (x != num || y != num || z != num || w != num);
		}

// ============================================================================
//		Others operators
// ----------------------------------------------------------------------------
		constexpr const _Ty& operator [] (const uInt& index) const {
			if (index >= 4)
				Raise("Vector4D: Index out of range");
			return xyzw[index];
		}
		constexpr _Ty& operator [] (const uInt& index) {
			if (index >= 4)
				Raise("Vector4D: Index out of range");
			return xyzw[index];
		}

		constexpr Vector4D<_Ty> operator + () const noexcept {
			return *this;
		}
		constexpr Vector4D<_Ty> operator - () const noexcept {
			return (*this) * (_Ty)(-1);
		}

		constexpr Vector4D<_Ty> operator = (const Vector4D<_Ty>& Vec) noexcept {
			if (*this != Vec)
				memcpy(const_cast<void*>(reinterpret_cast<const void*>(this)), &Vec, sizeof(Vec));
			return (*this);
		}
		constexpr Vector4D<_Ty> operator = (const Vector3D<_Ty>& Vec3D) noexcept {
			memcpy((void*)this, &Vec3D, sizeof(Vec3D));
			this->w = 1;
			return (*this);
		}

		template <typename _U>
		constexpr operator Vector4D<_U>() const noexcept {
			return Vector4D<_U>(_U(x), _U(y), _U(z), _U(w));
		}
		constexpr operator Vector3D<_Ty>() const noexcept {
			return Vector3D(x, y, z);
		}
		constexpr operator Vector2D<_Ty>() const noexcept {
			return Vector2D(x, y);
		}

// ============================================================================
//		Vectors
// ----------------------------------------------------------------------------
		static const Vector4D<_Ty> Zero;
		static const Vector4D<_Ty> Identity;

		static const Vector4D<_Ty> Left;
		static const Vector4D<_Ty> Right;
		static const Vector4D<_Ty> Down;
		static const Vector4D<_Ty> Up;
		static const Vector4D<_Ty> Back;
		static const Vector4D<_Ty> Forward;

		union {
			struct {
				_Ty x, y, z, w;
			};
			struct {
				Vector2D<_Ty> xy;
				Vector2D<_Ty> zw;
			};
			struct {
				_Ty x;
				Vector2D<_Ty> yz;
				_Ty w;
			};
			struct {
				Vector3D<_Ty> xyz;
				_Ty w;
			};
			struct {
				_Ty x;
				Vector3D<_Ty> yzw;
			};
			_Ty xyzw[4];

			struct {
				_Ty r, g, b, a;
			};
			struct {
				Vector2D<_Ty> rg;
				Vector2D<_Ty> ba;
			};
			struct {
				_Ty r;
				Vector2D<_Ty> gb;
				_Ty a;
			};
			struct {
				Vector3D<_Ty> rgb;
				_Ty a;
			};
			struct {
				_Ty r;
				Vector3D<_Ty> gba;
			};
			_Ty rgba[4];
		};
	};

	template <typename _Ty>
	const Vector4D<_Ty> Vector4D<_Ty>::Zero = Vector3D<_Ty>::Zero;

	template <typename _Ty>
	const Vector4D<_Ty> Vector4D<_Ty>::Identity = Vector3D<_Ty>::Identity;

	template <typename _Ty>
	const Vector4D<_Ty> Vector4D<_Ty>::Left = Vector3D<_Ty>::Left;

	template <typename _Ty>
	const Vector4D<_Ty> Vector4D<_Ty>::Right = Vector3D<_Ty>::Right;

	template <typename _Ty>
	const Vector4D<_Ty> Vector4D<_Ty>::Down = Vector3D<_Ty>::Down;

	template <typename _Ty>
	const Vector4D<_Ty> Vector4D<_Ty>::Up = Vector3D<_Ty>::Up;

	template <typename _Ty>
	const Vector4D<_Ty> Vector4D<_Ty>::Back = Vector3D<_Ty>::Back;

	template <typename _Ty>
	const Vector4D<_Ty> Vector4D<_Ty>::Forward = Vector3D<_Ty>::Forward;
}