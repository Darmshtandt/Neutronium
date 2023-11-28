#pragma once

namespace Nt {
	struct Matrix3x3 {
		constexpr Matrix3x3() noexcept {
			MakeIdentity();
		}
		constexpr Matrix3x3(std::initializer_list<Float> List) noexcept {
			for (uInt i = 0; i < (min(List.size(), 9)); ++i)
				Matrix[i] = *(List.begin() + i);
		}

		constexpr static Matrix3x3 GetIdentity() noexcept {
			return Matrix3x3 { };
		}
		constexpr void MakeIdentity() noexcept {
			for (uInt i = 0; i < 3; ++i)
				for (uInt j = 0; j < 3; ++j)
					Matrix2D[i][j] = Float(i == j);
		}


		constexpr void Translate(const Float3D& Vec) noexcept {
			Matrix2D[3][0] += Matrix2D[0][0] * Vec.x + Matrix2D[1][0] * Vec.y + Matrix2D[2][0] * Vec.z;
			Matrix2D[3][1] += Matrix2D[0][1] * Vec.x + Matrix2D[1][1] * Vec.y + Matrix2D[2][1] * Vec.z;
			Matrix2D[3][2] += Matrix2D[0][2] * Vec.x + Matrix2D[1][2] * Vec.y + Matrix2D[2][2] * Vec.z;
		}

		constexpr void Rotate(const Float3D& Angle) noexcept {
			if (Angle == 0.f)
				return;

			Matrix3x3 Rotate;
			Rotate.MakeIdentity();
			if (Angle.x != 0.f) {
				Rotate *= {
					1.f, 0.f, 0.f,
					0.f, cosf(Angle.x), -sinf(Angle.x),
					0.f, sinf(Angle.x), cosf(Angle.x),
				};
			}
			if (Angle.y != 0.f) {
				Rotate *= {
					cosf(Angle.y), 0.f, sinf(Angle.y),
					0.f, 1.f, 0.f,
					-sinf(Angle.y), 0.f, cosf(Angle.y),
				};
			}
			if (Angle.z != 0.f) {
				Rotate *= {
					cosf(Angle.z), -sinf(Angle.z), 0.f,
					sinf(Angle.z), cosf(Angle.z), 0.f,
					0.f, 0.f, 1.f,
				};
			}
			(*this) *= Rotate;
		}

		constexpr void Transposition() noexcept {
			for (uInt i = 0; i < 3; ++i)
				for (uInt j = i; j < 3; ++j) {
					if (i == j)
						continue;
					std::swap(Matrix2D[i][j], Matrix2D[j][i]);
				}
		}

		constexpr Float Det() const noexcept {
			Float Result = Matrix2D[0][0] * Matrix2D[1][1] * Matrix2D[2][2];
			Result += Matrix2D[1][0] * Matrix2D[2][1] * Matrix2D[0][2];
			Result += Matrix2D[2][0] * Matrix2D[0][1] * Matrix2D[1][2];

			Result -= Matrix2D[0][2] * Matrix2D[1][1] * Matrix2D[2][0];
			Result -= Matrix2D[0][0] * Matrix2D[1][2] * Matrix2D[2][1];
			Result -= Matrix2D[0][1] * Matrix2D[1][0] * Matrix2D[2][2];
			return Result;
		}


		constexpr bool operator == (const Matrix3x3& Mat) const noexcept {
			for (uInt i = 0; i < 3; ++i)
				if (this->Rows[i] != Mat.Rows[i])
					return false;
			return true;
		}
		constexpr bool operator != (const Matrix3x3& Mat) const noexcept {
			return !((*this) == Mat);
		}

		constexpr Matrix3x3 operator = (const Matrix3x3& Mat) noexcept {
			for (uInt i = 0; i < 3; ++i)
				for (uInt j = 0; j < 3; ++j)
					Matrix2D[i][j] = Mat.Matrix2D[i][j];
			return *this;
		}
		constexpr Matrix3x3 operator = (std::initializer_list<Float> List) noexcept {
			for (uInt i = 0; i < min(List.size(), 9); ++i)
				Matrix[i] = *(List.begin() + i);
			return *this;
		}

		constexpr Float3D operator * (const Float3D& Vec) const noexcept {
			Float3D Result = { };
			Matrix3x3 This = *this;
			for (uInt i = 0; i < 3; ++i) {
				This.Rows[i] *= Vec;
				Result.xyz[i] = This.Rows[i].x + This.Rows[i].y + This.Rows[i].z;
			}
			return Result;
		}

		constexpr Matrix3x3 operator * (const Matrix3x3& Mat) const noexcept {
			Matrix3x3 Result = { };
			for (uInt i = 0; i < 3; ++i) {
				Result.Rows[i] = this->Rows[0] * Mat.Rows[i].x;
				Result.Rows[i] += this->Rows[1] * Mat.Rows[i].y;
				Result.Rows[i] += this->Rows[2] * Mat.Rows[i].z;
			}
			return Result;
		}

		constexpr Matrix3x3 operator * (const Float& Num) const noexcept {
			Matrix3x3 Result = *this;
			for (uInt i = 0; i < 3; ++i)
				Result.Rows[i] *= Num;
			return Result;
		}

		constexpr Matrix3x3 operator *= (const Matrix3x3& Mat) noexcept {
			(*this) = (*this) * Mat;
			return (*this);
		}
		constexpr Matrix3x3 operator *= (const Float& Num) noexcept {
			(*this) = (*this) * Num;
			return (*this);
		}

		constexpr const Float* operator [] (const uInt& Index) const noexcept {
			return Matrix2D[Index];
		}
		constexpr Float* operator [] (const uInt& Index) noexcept {
			return Matrix2D[Index];
		}

		union {
			struct {
				Float _11, _21, _31;
				Float _12, _22, _32;
				Float _13, _23, _33;
			};
			Float3D Rows[3];
			Float Matrix2D[3][3];
			Float Matrix[9];
		};
	};
}