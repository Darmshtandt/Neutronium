#pragma once

#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Math/Matrix3x3.h>

namespace Nt {
	struct Matrix4x4 {
		constexpr Matrix4x4() noexcept {
			MakeIdentity();
		}
		constexpr Matrix4x4(std::initializer_list<Float> list) noexcept {
			for (uInt i = 0; i < std::min(list.size(), 16u); ++i)
				Matrix[i] = *(list.begin() + i);
		}
		constexpr Matrix4x4(const Matrix4x4& matrix) noexcept {
			for (uInt i = 0; i < 16; ++i)
				Matrix[i] = matrix.Matrix[i];
		}

		static constexpr Matrix4x4 Fill(const Float& value) noexcept {
			Matrix4x4 result;
			for (uInt i = 0; i < 4 * 4; ++i)
				result.Matrix[i] = value;
			return result;
		}

		static constexpr Matrix4x4 GetIdentity() noexcept {
			return Matrix4x4 { };
		}
		static constexpr Matrix4x4 GetTranslate(const Float3D& vec) noexcept {
			Matrix4x4 Mat;
			Mat.MakeIdentity();
			Mat.Translate(vec);
			return Mat;
		}
		static constexpr Matrix4x4 GetScale(const Float3D& vec) noexcept {
			return Matrix4x4 {
				vec.x, 0.f, 0.f, 0.f,
				0.f,vec.y, 0.f, 0.f,
				0.f, 0.f, vec.z, 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static constexpr Matrix4x4 GetRotateX(const Float& angle) noexcept {
			return Matrix4x4 {
				1.f, 0.f, 0.f, 0.f,
				0.f, cosf(angle), -sinf(angle), 0.f,
				0.f, sinf(angle), cosf(angle), 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static constexpr Matrix4x4 GetRotateY(const Float& angle) noexcept {
			return Matrix4x4 {
				cosf(angle), 0.f, sinf(angle), 0.f,
				0.f, 1.f, 0.f, 0.f,
				-sinf(angle), 0.f, cosf(angle), 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static constexpr Matrix4x4 GetRotateZ(const Float& angle) noexcept {
			return  Matrix4x4 {
				cosf(angle), -sinf(angle), 0.f, 0.f,
				sinf(angle), cosf(angle), 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static constexpr Matrix4x4 GetRotate(const Float3D& angle) noexcept {
			return GetRotateX(angle.x) * GetRotateY(angle.y) * GetRotateZ(angle.z);
		}

		constexpr Matrix4x4& MakeIdentity() noexcept {
			for (uInt i = 0; i < 4; ++i)
				for (uInt j = 0; j < 4; ++j)
					Matrix2D[i][j] = Float(i == j);
			return *this;
		}

		constexpr Matrix4x4& Translate(const Float3D& vector) noexcept {
			if (vector.LengthSquare() == 0.f)
				return *this;

			Columns[3] = Columns[0] * vector.x + Columns[1] * vector.y + Columns[2] * vector.z + Columns[3] * 1.f;
			return *this;
		}
		constexpr Matrix4x4& Scale(const Float3D& Vec) noexcept {
			(*this) *= GetScale(Vec);
			return *this;
		}

		constexpr Matrix4x4& RotateX(const Float& angle) noexcept {
			if (angle != 0.f)
				(*this) *= GetRotateX(angle);
			return *this;
		}
		constexpr Matrix4x4& RotateY(const Float& angle) noexcept {
			if (angle != 0.f)
				(*this) *= GetRotateY(angle);
			return *this;
		}
		constexpr Matrix4x4& RotateZ(const Float& angle) noexcept {
			if (angle != 0.f)
				(*this) *= GetRotateZ(angle);
			return *this;
		}
		constexpr Matrix4x4& Rotate(const Float3D& angle) noexcept {
			if (angle.LengthSquare() != 0.f)
				(*this) *= GetRotate(angle);
			return *this;
		}

		constexpr Matrix4x4 Transposition() const noexcept {
			Matrix4x4 Result;
			for (uInt i = 0; i < 4; ++i)
				for (uInt j = 0; j < 4; ++j)
					Result[i][j] = Matrix2D[j][i];
			return Result;
		}
		Float3D GetEulerAngles() const noexcept {
			Float3D angle;
			angle.x = atan2(Columns[2][1], Columns[2][2]);
			angle.y = atan2(-Columns[2][0], sqrt(Columns[2][1] * Columns[2][1] + Columns[2][2] * Columns[2][2]));
			angle.z = atan2(Columns[1][0], Columns[0][0]);
			return angle;
		}

		constexpr Float Det() const noexcept {
			Float result = 0.f;
			result += _11 * Minor(0, 0).Det();
			result -= _12 * Minor(1, 0).Det();
			result += _13 * Minor(2, 0).Det();
			result -= _14 * Minor(3, 0).Det();
			return result;
		}

		// Indexing starts at 0
		constexpr Matrix3x3 Minor(const Float& x, const Float& y) const noexcept {
			Matrix3x3 Result;
			uInt k = 0;
			for (uInt i = 0; i < 4; ++i) {
				if (i == y)
					continue;

				for (uInt j = 0; j < 4; ++j) {
					if (j == x)
						continue;

					Result.Matrix[k] = Matrix2D[i][j];
					++k;
				}
			}
			return Result;
		}
		Float AlgebraicComplement(const Float& x, const Float& y) const noexcept {
			const Float result = Minor(x, y).Det();

			if (static_cast<Int>(x + y) % 2 == 0)
				return result;
			return -result;
		}
		Float AlgebraicComplement(const Float2D& v) const noexcept {
			const Float result = Minor(v.x, v.y).Det();

			if (static_cast<Int>(v.x + v.y) % 2 == 0)
				return result;
			return -result;
		}

		constexpr Matrix4x4 GetInverse() const noexcept {
			Matrix4x4 MatA = { };
			for (uInt i = 0; i < 4; ++i) {
				for (uInt j = 0; j < 4; ++j)
					MatA[i][j] = AlgebraicComplement(uInt2D(i, j));
			}

			Float MatADet = Matrix2D[0][0] * MatA[0][0];
			MatADet += Matrix2D[0][1] * MatA[1][0];
			MatADet += Matrix2D[0][2] * MatA[2][0];
			MatADet += Matrix2D[0][3] * MatA[3][0];

			if (MatADet == 0.f)
				return MatA * 0.f;
			return MatA * (1.f / MatADet);
		}

		constexpr bool operator == (const Float& value) const noexcept {
			for (uInt i = 0; i < 4; ++i)
				if (this->Columns[i] != value)
					return false;
			return true;
		}
		constexpr bool operator == (const Matrix4x4& Mat) const noexcept {
			for (uInt i = 0; i < 4; ++i)
				if (this->Columns[i] != Mat.Columns[i])
					return false;
			return true;
		}
		constexpr bool operator != (const Matrix4x4& Mat) const noexcept {
			return !((*this) == Mat);
		}

		constexpr Matrix4x4 operator = (const Matrix4x4& Mat) noexcept {
			for (uInt i = 0; i < 4; ++i)
				for (uInt j = 0; j < 4; ++j)
					Matrix2D[i][j] = Mat.Matrix2D[i][j];
			return *this;
		}
		constexpr Matrix4x4 operator = (std::initializer_list<Float> List) noexcept {
			for (uInt i = 0; i < std::min(List.size(), 16u); ++i)
				Matrix[i] = *(List.begin() + i);
			return *this;
		}

		constexpr Float4D operator * (const Float4D& v) const noexcept {
			Float4D Result = { };
			Result.x = v.x * Columns[0].x + v.y * Columns[1].x + v.z * Columns[2].x + v.w * Columns[3].x;
			Result.y = v.x * Columns[0].y + v.y * Columns[1].y + v.z * Columns[2].y + v.w * Columns[3].y;
			Result.z = v.x * Columns[0].z + v.y * Columns[1].z + v.z * Columns[2].z + v.w * Columns[3].z;
			Result.w = v.x * Columns[0].w + v.y * Columns[1].w + v.z * Columns[2].w + v.w * Columns[3].w;
			return Result;
		}

		constexpr Matrix4x4 operator * (const Matrix4x4& Mat) const noexcept {
			const Float4D& A0 = Columns[0];
			const Float4D& A1 = Columns[1];
			const Float4D& A2 = Columns[2];
			const Float4D& A3 = Columns[3];

			const Float4D& B0 = Mat.Columns[0];
			const Float4D& B1 = Mat.Columns[1];
			const Float4D& B2 = Mat.Columns[2];
			const Float4D& B3 = Mat.Columns[3];

			Matrix4x4 Result = { };
			Result.Columns[0] = A0 * B0[0] + A1 * B0[1] + A2 * B0[2] + A3 * B0[3];
			Result.Columns[1] = A0 * B1[0] + A1 * B1[1] + A2 * B1[2] + A3 * B1[3];
			Result.Columns[2] = A0 * B2[0] + A1 * B2[1] + A2 * B2[2] + A3 * B2[3];
			Result.Columns[3] = A0 * B3[0] + A1 * B3[1] + A2 * B3[2] + A3 * B3[3];
			return Result;
		}
		constexpr Matrix4x4 operator + (const Matrix4x4& Mat) const noexcept {
			Matrix4x4 Result = { };
			for (uInt i = 0; i < 4; ++i)
				Result.Columns[i] = this->Columns[i] + Mat.Columns[i];

			return Result;
		}
		constexpr Matrix4x4 operator - (const Matrix4x4& Mat) const noexcept {
			Matrix4x4 Result = { };
			for (uInt i = 0; i < 4; ++i)
				Result.Columns[i] = this->Columns[i] - Mat.Columns[i];

			return Result;
		}

		constexpr Matrix4x4 operator * (const Float& Num) const noexcept {
			Matrix4x4 Result = *this;
			for (uInt i = 0; i < 4; ++i)
				Result.Columns[i] *= Num;
			return Result;
		}
		constexpr Matrix4x4 operator + (const Float& Num) const noexcept {
			Matrix4x4 Result = *this;
			for (uInt i = 0; i < 4; ++i)
				Result.Columns[i] += Num;
			return Result;
		}
		constexpr Matrix4x4 operator - (const Float& Num) const noexcept {
			Matrix4x4 Result = *this;
			for (uInt i = 0; i < 4; ++i)
				Result.Columns[i] -= Num;
			return Result;
		}


		constexpr Matrix4x4 operator *= (const Matrix4x4& Mat) noexcept {
			(*this) = (*this) * Mat;
			return (*this);
		}
		constexpr Matrix4x4 operator *= (const Float& Num) noexcept {
			(*this) = (*this) * Num;
			return (*this);
		}

		constexpr Matrix4x4 operator += (const Matrix4x4& Mat) noexcept {
			(*this) = (*this) + Mat;
			return (*this);
		}
		constexpr Matrix4x4 operator += (const Float& Num) noexcept {
			(*this) = (*this) + Num;
			return (*this);
		}

		constexpr Matrix4x4 operator -= (const Matrix4x4& Mat) noexcept {
			(*this) = (*this) - Mat;
			return (*this);
		}
		constexpr Matrix4x4 operator -= (const Float& Num) noexcept {
			(*this) = (*this) - Num;
			return (*this);
		}


		constexpr const Float4D& operator [] (const uInt& Index) const noexcept {
			return Columns[Index];
		}
		constexpr Float4D& operator [] (const uInt& Index) noexcept {
			return Columns[Index];
		}

		operator Float4D() const noexcept {
			return (*this) * Float4D().Fill(1.f);
		}

		union {
			struct {
				Float _11, _12, _13, _14;
				Float _21, _22, _23, _24;
				Float _31, _32, _33, _34;
				Float _41, _42, _43, _44;
			};

			Float4D Columns[4];
			Float Matrix2D[4][4];
			Float Matrix[16] = { };
		};
	};
}