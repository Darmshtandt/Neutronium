#pragma once

namespace Nt {
	struct Matrix4x4 {
		constexpr Matrix4x4() noexcept {
			MakeIdentity();
		}
		constexpr Matrix4x4(std::initializer_list<Float> List) noexcept {
			for (uInt i = 0; i < min(List.size(), 16); ++i)
				Matrix[i] = *(List.begin() + i);
		}

		static constexpr Matrix4x4 GetIdentity() noexcept {
			return Matrix4x4 { };
		}
		static constexpr Matrix4x4 GetTranslate(const Float3D& Vec) noexcept {
			Matrix4x4 Mat;
			Mat.MakeIdentity();
			Mat.Translate(Vec);
			return Mat;
		}
		static constexpr Matrix4x4 GetScale(const Float3D& Vec) noexcept {
			return Matrix4x4 {
				Vec.x, 0.f, 0.f, 0.f,
				0.f,Vec.y, 0.f, 0.f,
				0.f, 0.f, Vec.z, 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static constexpr Matrix4x4 GetRotateX(const Float& Angle) noexcept {
			return Matrix4x4 {
				1.f, 0.f, 0.f, 0.f,
				0.f, cosf(Angle * RADf), -sinf(Angle * RADf), 0.f,
				0.f, sinf(Angle * RADf), cosf(Angle * RADf), 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static constexpr Matrix4x4 GetRotateY(const Float& Angle) noexcept {
			return Matrix4x4 {
				cosf(Angle * RADf), 0.f, sinf(Angle * RADf), 0.f,
				0.f, 1.f, 0.f, 0.f,
				-sinf(Angle * RADf), 0.f, cosf(Angle * RADf), 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static constexpr Matrix4x4 GetRotateZ(const Float& Angle) noexcept {
			return  Matrix4x4 {
				cosf(Angle * RADf), -sinf(Angle * RADf), 0.f, 0.f,
				sinf(Angle * RADf), cosf(Angle * RADf), 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static constexpr Matrix4x4 GetRotate(const Float3D& Angle) noexcept {
			return GetRotateX(Angle.x) * GetRotateY(Angle.y) * GetRotateZ(Angle.z);
		}

		constexpr void MakeIdentity() noexcept {
			for (uInt i = 0; i < 4; ++i)
				for (uInt j = 0; j < 4; ++j)
					Matrix2D[i][j] = Float(i == j);
		}

		constexpr void Translate(const Float3D& Vec) noexcept {
			Matrix2D[3][0] += Matrix2D[0][0] * Vec.x + Matrix2D[1][0] * Vec.y + Matrix2D[2][0] * Vec.z;
			Matrix2D[3][1] += Matrix2D[0][1] * Vec.x + Matrix2D[1][1] * Vec.y + Matrix2D[2][1] * Vec.z;
			Matrix2D[3][2] += Matrix2D[0][2] * Vec.x + Matrix2D[1][2] * Vec.y + Matrix2D[2][2] * Vec.z;
		}
		constexpr void Scale(const Float3D& Vec) noexcept {
			(*this) *= GetScale(Vec);
		}

		constexpr void RotateX(const Float& Angle) noexcept {
			(*this) *= GetRotateX(Angle);
		}
		constexpr void RotateY(const Float& Angle) noexcept {
			(*this) *= GetRotateY(Angle);
		}
		constexpr void RotateZ(const Float& Angle) noexcept {
			(*this) *= GetRotateZ(Angle);
		}
		constexpr void Rotate(const Float3D& Angle) noexcept {
			(*this) *= GetRotate(Angle);
		}

		constexpr Matrix4x4 Transposition() const noexcept {
			Matrix4x4 Result;
			for (uInt i = 0; i < 4; ++i)
				for (uInt j = i; j < 4; ++j)
					Result[i][j] = Matrix2D[j][i];
			return Result;
		}
		Float3D GetEulerAngles() const noexcept {
			Float3D angle;
			angle.x = atan2(Rows[2][1], Rows[2][2]);
			angle.y = atan2(-Rows[2][0], sqrt(Rows[2][1] * Rows[2][1] + Rows[2][2] * Rows[2][2]));
			angle.z = atan2(Rows[1][0], Rows[0][0]);
			return angle;
		}

		// Indexing starts at 0
		constexpr Matrix3x3 Minor(const Float& x, const Float& y) const noexcept {
			Matrix3x3 Result;
			uInt k = 0;
			for (uInt i = 0; i < 4; ++i) {
				for (uInt j = 0; j < 4; ++j) {
					if (j == y)
						continue;
					else if (i == x)
						break;

					Result.Matrix[k] = Matrix2D[i][j];
					++k;
				}
			}
			return Result;
		}
		Float AlgebraicComplement(const Float& x, const Float& y) const noexcept {
			return powf((-1.f), x + y) * Minor(x, y).Det();
		}
		Float AlgebraicComplement(const Float2D& XY) const noexcept {
			return powf((-1.f), XY.x + XY.y) * Minor(XY.x, XY.y).Det();
		}

		constexpr Matrix4x4 GetInverse() const noexcept {
			Matrix4x4 MatA = { };
			for (uInt i = 0; i < 4; ++i)
				for (uInt j = 0; j < 4; ++j)
					MatA[j][i] = AlgebraicComplement(uInt2D(i, j));

			Float MatADet = Matrix2D[0][0] * MatA[0][0];
			MatADet += Matrix2D[0][1] * MatA[0][1];
			MatADet += Matrix2D[0][2] * MatA[0][2];
			MatADet += Matrix2D[0][3] * MatA[0][3];

			return MatA * (1.f / MatADet);
		}

		constexpr bool operator == (const Matrix4x4& Mat) const noexcept {
			for (uInt i = 0; i < 4; ++i)
				if (this->Rows[i] != Mat.Rows[i])
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
			for (uInt i = 0; i < min(List.size(), 16); ++i)
				Matrix[i] = *(List.begin() + i);
			return *this;
		}

		constexpr Float4D operator * (const Float4D& Vec) const noexcept {
			Float4D Result = Float4D::Zero;
			Matrix4x4 This = *this;
			for (uInt i = 0; i < 4; ++i) {
				This.Rows[i] *= Vec;
				Result.xyzw[i] = This.Rows[i].x + This.Rows[i].y + This.Rows[i].z + This.Rows[i].w;
			}
			return Result;
		}

		constexpr Matrix4x4 operator * (const Matrix4x4& Mat) const noexcept {
			Matrix4x4 Result = { };
			for (uInt i = 0; i < 4; ++i) {
				Result.Rows[i] = this->Rows[0] * Mat.Rows[i].x;
				Result.Rows[i] += this->Rows[1] * Mat.Rows[i].y;
				Result.Rows[i] += this->Rows[2] * Mat.Rows[i].z;
				Result.Rows[i] += this->Rows[3] * Mat.Rows[i].w;
			}
			return Result;
		}

		constexpr Matrix4x4 operator * (const Float& Num) const noexcept {
			Matrix4x4 Result = *this;
			for (uInt i = 0; i < 4; ++i)
				Result.Rows[i] *= Num;
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

		constexpr const Float4D& operator [] (const uInt& Index) const noexcept {
			return Rows[Index];
		}
		constexpr Float4D& operator [] (const uInt& Index) noexcept {
			return Rows[Index];
		}

		operator Float4D() const noexcept {
			return (*this) * Float4D::Identity;
		}

		union {
			struct {
				Float _11, _21, _31, _41;
				Float _12, _22, _32, _42;
				Float _13, _23, _33, _43;
				Float _14, _24, _34, _44;
			};
			Float4D Rows[4];
			Float Matrix2D[4][4];
			Float Matrix[16];
		};
	};
}