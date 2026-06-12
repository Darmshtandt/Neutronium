#pragma once

#include <Nt/Core/Math/Matrix4x4.h>

namespace NtEx {
	using namespace Nt;

	struct Quaternion {
		using Type = Float;
		using Type2D = Vector2D<Type>;
		using Type3D = Vector3D<Type>;
		using Type4D = Vector4D<Type>;

		static constexpr Type Epsilon = std::numeric_limits<Type>::epsilon();

		NT_CONSTEXPR Quaternion() noexcept :
			x(0), y(0), z(0), w(0)
		{
		}
		NT_CONSTEXPR Quaternion(Type X, Type Y, Type Z, Type W) noexcept :
			x(X), y(Y), z(Z), w(W)
		{
		}
		NT_CONSTEXPR Quaternion(const Type3D& axis, Type angle) noexcept :
			x(axis.x), y(axis.y), z(axis.z), w(angle)
		{
		}
		explicit Quaternion(const Matrix3x3& matrix) noexcept {
			*this = GetFromMatrix(matrix);
		}
		explicit Quaternion(const Matrix4x4& matrix4x4) noexcept {
			Matrix3x3 matrix3x3;
			for (uInt i = 0; i < 3; ++i)
				matrix3x3.Columns[i] = matrix4x4.Columns[i].xyz;

			*this = GetFromMatrix(matrix3x3);
		}

		NT_NODISCARD_CONSTEXPR static Quaternion Identity() noexcept {
			return Quaternion(0, 0, 0, 1);
		}
		NT_NODISCARD_CONSTEXPR static Quaternion GetFromDirection(const Type3D& dir, const Type3D& up) noexcept {
			if (dir.LengthSquare() < Epsilon)
				return Quaternion();

			const Type3D zAxis = dir;
			Type3D xAxis = up.GetCross(zAxis);
			if (xAxis.LengthSquare() < Epsilon)
				return GetRotationTo({ 0.f, 0.f, 1.f }, zAxis);

			xAxis = xAxis.GetNormalize();
			const Type3D yAxis = zAxis.GetCross(xAxis);
			return GetFromAxes(xAxis, yAxis, zAxis);
		}
		NT_NODISCARD static Quaternion GetFromEuler(const Type3D& euler) noexcept {
			const Type3D a = euler * static_cast<Type>(0.5);
			const Type3D c(std::cos(a.x), std::cos(a.y), std::cos(a.z));
			const Type3D s(std::sin(a.x), std::sin(a.y), std::sin(a.z));

			Quaternion result;
			result.w = c.x * c.y * c.z + s.x * s.y * s.z;
			result.x = s.x * c.y * c.z - c.x * s.y * s.z;
			result.y = c.x * s.y * c.z + s.x * c.y * s.z;
			result.z = c.x * c.y * s.z - s.x * s.y * c.z;
			return result;
		}
		NT_NODISCARD_CONSTEXPR static Quaternion GetFromAxes(const Type3D& xAxis, const Type3D& yAxis, const Type3D& zAxis) noexcept {
			Nt::Matrix3x3 mat;
			mat.Matrix2D[0][0] = xAxis.x;
			mat.Matrix2D[1][0] = xAxis.y;
			mat.Matrix2D[2][0] = xAxis.z;
			mat.Matrix2D[0][1] = yAxis.x;
			mat.Matrix2D[1][1] = yAxis.y;
			mat.Matrix2D[2][1] = yAxis.z;
			mat.Matrix2D[0][2] = zAxis.x;
			mat.Matrix2D[1][2] = zAxis.y;
			mat.Matrix2D[2][2] = zAxis.z;

			return GetFromMatrix(mat);
		}
		NT_NODISCARD_CONSTEXPR static Quaternion GetFromMatrix(const Matrix3x3& mat) noexcept {
			Type scalar;
			Type3D axis;

			const Type trace = mat._11 + mat._22 + mat._33;
			if (trace > Epsilon) {
				const Type s = 2.0f * std::sqrt(trace + 1.f);

				scalar = 0.25f * s;
				axis.x = (mat._32 - mat._23) / s;
				axis.y = (mat._13 - mat._31) / s;
				axis.z = (mat._21 - mat._12) / s;

				return Quaternion(axis, scalar);
			}

			Int i = 0, j = 1, k = 2;
			if (mat._22 > mat._11)
				i = 1, j = 2, k = 0;
			if (mat._33 > mat.Matrix2D[i][i])
				i = 2, j = 0, k = 1;

			const Float s = 2.0f *
				std::sqrt(mat.Matrix2D[i][i] - mat.Matrix2D[j][j] - mat.Matrix2D[k][k] + 1.0f);

			axis[i] = 0.25f * s;
			scalar = (mat.Matrix2D[k][j] - mat.Matrix2D[j][k]) / s;
			axis[j] = (mat.Matrix2D[j][i] + mat.Matrix2D[i][j]) / s;
			axis[k] = (mat.Matrix2D[k][i] + mat.Matrix2D[i][k]) / s;

			return Quaternion(axis, scalar);
		}

		NT_NODISCARD_CONSTEXPR static Quaternion LookRotation(const Type3D& forward, const Type3D& up);

		NT_NODISCARD_CONSTEXPR Quaternion operator * (const Quaternion& quat) const {
			return Quaternion(
				w * quat.x + x * quat.w + y * quat.z - z * quat.y,
				w * quat.y - x * quat.z + y * quat.w + z * quat.x,
				w * quat.z + x * quat.y - y * quat.x + z * quat.w,
				w * quat.w - x * quat.x - y * quat.y - z * quat.z
			);
		}
		NT_NODISCARD_CONSTEXPR Type3D operator * (const Type3D& vector) const {
			const Type3D qVector(x, y, z);
			const Type3D t = qVector.GetCross(vector) * static_cast<Type>(2);
			return vector + t * w + qVector.GetCross(t);
		}

		NT_NODISCARD_CONSTEXPR Quaternion operator + (const Quaternion& quat) const noexcept {
			return { x + quat.x, y + quat.y, z + quat.z, w + quat.w };
		}
		NT_NODISCARD_CONSTEXPR Quaternion operator - (const Quaternion& quat) const noexcept {
			return { x - quat.x, y - quat.y, z - quat.z, w - quat.w };
		}
		NT_NODISCARD_CONSTEXPR Quaternion operator * (Type scalar) const noexcept {
			return { x * scalar, y * scalar, z * scalar, w * scalar };
		}
		NT_NODISCARD_CONSTEXPR Quaternion operator / (Type scalar) const noexcept {
			return { x / scalar, y / scalar, z / scalar, w / scalar };
		}

		NT_NODISCARD_CONSTEXPR Quaternion operator - () const noexcept {
			return { -x, -y, -z, -w };
		}

		NT_NODISCARD_CONSTEXPR Quaternion& operator += (const Quaternion& quat) noexcept {
			return *this = *this + quat;
		}
		NT_NODISCARD_CONSTEXPR Quaternion& operator -= (const Quaternion& quat) noexcept {
			return *this = *this - quat;
		}
		NT_NODISCARD_CONSTEXPR Quaternion& operator *= (Type scalar) noexcept {
			return *this = *this * scalar;
		}
		NT_NODISCARD_CONSTEXPR Quaternion& operator /= (Type scalar) noexcept {
			return *this = *this / scalar;
		}

		NT_NODISCARD_CONSTEXPR Bool operator == (const Quaternion& quat) const noexcept {
			return x == quat.x && y == quat.y && z == quat.z && w == quat.w;
		}

		NT_NODISCARD Type Length() const noexcept {
			return std::sqrt(LengthSquared());
		}
		NT_NODISCARD_CONSTEXPR Type LengthSquared() const  noexcept {
			return x * x + y * y + z * z + w * w;
		}
		NT_NODISCARD Quaternion GetNormalize() const noexcept {
			const Type lengthSq = LengthSquared();
			if (std::abs(lengthSq) <= Epsilon)
				return Quaternion(0.f, 0.f, 0.f, 0.f);
			return *this / std::sqrt(lengthSq);
		}
		NT_NODISCARD_CONSTEXPR Type Dot(const Quaternion& quat) const noexcept {
			return x * quat.x + y * quat.y + z * quat.z + w * quat.w;
		}

		NT_NODISCARD_CONSTEXPR Quaternion GetConjugate() const noexcept {
			return { -x, -y, -z, w };
		}
		NT_NODISCARD Quaternion GetInverse() const noexcept {
			const Type lengthSq = LengthSquared();
			if (std::abs(lengthSq) <= Epsilon)
				return Identity();

			const Type inverseLength = static_cast<Type>(1) / std::sqrt(lengthSq);
			return GetConjugate() * inverseLength;
		}

		static Quaternion Lerp(const Quaternion& a, const Quaternion& b, Type t);
		static Quaternion NLerp(const Quaternion& a, const Quaternion& b, Type t);
		static Quaternion SLerp(const Quaternion& a, const Quaternion& b, Type t);

		Matrix3x3 ToMatrix3x3() const noexcept {
			Matrix3x3 m;

			const Type xx = x * x, yy = y * y, zz = z * z;
			const Type xy = x * y, xz = x * z, yz = y * z;
			const Type wx = w * x, wy = w * y, wz = w * z;

			m._11 = static_cast<Type>(1) - static_cast<Type>(2) * (yy + zz);
			m._12 = static_cast<Type>(2) * (xy + wz);
			m._13 = static_cast<Type>(2) * (xz - wy);

			m._21 = static_cast<Type>(2) * (xy - wz);
			m._22 = static_cast<Type>(1) - static_cast<Type>(2) * (xx + zz);
			m._23 = static_cast<Type>(2) * (yz + wx);

			m._31 = static_cast<Type>(2) * (xz + wy);
			m._32 = static_cast<Type>(2) * (yz - wx);
			m._33 = static_cast<Type>(1) - static_cast<Type>(2) * (xx + yy);

			return m;
		}

		Matrix4x4 ToMatrix4x4() const noexcept {
			Matrix3x3 m3x3 = ToMatrix3x3();

			Matrix4x4 m4x4;
			m4x4.Columns[0] = Float4D(m3x3.Columns[0], 0.f);
			m4x4.Columns[1] = Float4D(m3x3.Columns[1], 0.f);
			m4x4.Columns[2] = Float4D(m3x3.Columns[2], 0.f);
			return m4x4;
		}

		Type Roll() const noexcept {
			const Type2D vec = {
				w * w - x * x - y * y + z * z,
				static_cast<Type>(2) * (y * z + w * x)
			};

			if (vec.Abs() < Epsilon)
				return static_cast<Type>(2) * std::atan2(x, w);
			return std::atan2(vec.y, vec.x);
		}
		Type Pitch() const noexcept {
			return std::asin(std::clamp(
				static_cast<Type>(-2) * (x * z - w * y),
				static_cast<Type>(-1),
				static_cast<Type>(1)));
		}
		Type Yaw() const noexcept {
			const Type2D vec = {
				w * w + x * x - y * y - z * z,
				static_cast<Type>(2) * (x * y + w * z)
			};

			if (vec.Abs() < Epsilon)
				return static_cast<Type>(0);
			return std::atan2(vec.y, vec.x);
		}

		Type3D ToEuler() const {
			return {
				Roll(),
				Pitch(),
				Yaw()
			};
		}

		NT_NODISCARD static Quaternion GetRotationTo(const Type3D& from, const Type3D& to) noexcept {
			const Type3D dirFrom = from.GetNormalize();
			const Type3D dirTo = to.GetNormalize();

			Type d = dirFrom.Dot(dirTo) + 1.f;
			if (std::fabs(d) < Epsilon) {
				Type3D axis = Type3D(1.f, 0.f, 0.f).GetCross(dirFrom);
				if (axis.LengthSquare() < Epsilon)
					axis = Type3D(0.f, 1.f, 0.f).GetCross(dirFrom);
				return Quaternion(axis.GetNormalize(), 0.f);
			}

			d = std::sqrt(d * 2.f);
			const Type3D axis = dirFrom.GetCross(dirTo) / d;
			return Quaternion(axis, d * 0.5f).GetNormalize();
		}

		NT_NODISCARD Type operator [] (const uInt index) const {
			Assert(index < 4, "Out of range");
			return Arr[index];
		}
		NT_NODISCARD Type& operator [] (const uInt index) {
			Assert(index < 4, "Out of range");
			return Arr[index];
		}

		Quaternion EnsureNonZero() const;
		Quaternion EnsureShortestPath(const Quaternion& rhs) const;
		Bool IsNormalized(Type eps = 1e-6f) const;

		static Type AngleBetween(const Quaternion& a, const Quaternion& b);
		static Quaternion LookAt(const Type3D& pos, const Type3D& target, const Type3D& up);
		static Quaternion FromTo(const Type3D& from, const Type3D& to);

		union {
			struct {
				Type x;
				Type y;
				Type z;
				Type w;
			};

			Type Arr[4] = { };
		};
	};
}