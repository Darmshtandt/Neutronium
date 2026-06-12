#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	struct Matrix3x3 {
		constexpr Matrix3x3() noexcept {
			MakeIdentity();
		}
		constexpr Matrix3x3(std::initializer_list<Float> list) noexcept {
			for (uInt i = 0; i < (std::min(list.size(), 9u)); ++i)
				Matrix[i] = *(list.begin() + i);
		}
		constexpr Matrix3x3(const Matrix3x3& matrix) noexcept {
			for (uInt i = 0; i < 9; ++i)
				Matrix[i] = matrix.Matrix[i];
		}

		constexpr static Matrix3x3 GetIdentity() noexcept {
			return Matrix3x3 { };
		}
		constexpr void MakeIdentity() noexcept {
			for (uInt i = 0; i < 3; ++i) {
				for (uInt j = 0; j < 3; ++j)
					Matrix2D[i][j] = (i == j) ? 1.f : 0.f;
			}
		}

		constexpr void Translate(const Float3D& vector) noexcept {
			Matrix2D[2][0] += Matrix2D[0][0] * vector.x + Matrix2D[1][0] * vector.y + Matrix2D[2][0] * vector.z;
			Matrix2D[2][1] += Matrix2D[0][1] * vector.x + Matrix2D[1][1] * vector.y + Matrix2D[2][1] * vector.z;
			Matrix2D[2][2] += Matrix2D[0][2] * vector.x + Matrix2D[1][2] * vector.y + Matrix2D[2][2] * vector.z;
		}

		constexpr void Rotate(const Float3D& angle) noexcept {
			if (angle == 0.f)
				return;

			Matrix3x3 Rotate;
			Rotate.MakeIdentity();

			if (angle.x != 0.f) {
				Rotate *= {
					1.f, 0.f, 0.f,
					0.f, cosf(angle.x), -sinf(angle.x),
					0.f, sinf(angle.x), cosf(angle.x),
				};
			}
			if (angle.y != 0.f) {
				Rotate *= {
					cosf(angle.y), 0.f, sinf(angle.y),
					0.f, 1.f, 0.f,
					-sinf(angle.y), 0.f, cosf(angle.y),
				};
			}
			if (angle.z != 0.f) {
				Rotate *= {
					cosf(angle.z), -sinf(angle.z), 0.f,
					sinf(angle.z), cosf(angle.z), 0.f,
					0.f, 0.f, 1.f,
				};
			}
			(*this) *= Rotate;
		}

		constexpr void Transposition() noexcept {
			for (uInt i = 0; i < 3; ++i) {
				for (uInt j = i + 1; j < 3; ++j)
					std::swap(Matrix2D[i][j], Matrix2D[j][i]);
			}
		}

		constexpr Float Det() const noexcept {
			Float Result = _11 * _22 * _33;
			Result += _21 * _32 * _13;
			Result += _31 * _12 * _23;

			Result -= _13 * _22 * _31;
			Result -= _11 * _23 * _32;
			Result -= _12 * _21 * _33;
			return Result;
		}


		constexpr bool operator == (const Matrix3x3& Mat) const noexcept {
			for (uInt i = 0; i < 3; ++i)
				if (this->Columns[i] != Mat.Columns[i])
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
			for (uInt i = 0; i < std::min(List.size(), 9u); ++i)
				Matrix[i] = *(List.begin() + i);
			return *this;
		}

		constexpr Float3D operator * (const Float3D& Vec) const noexcept {
			Float3D Result = { };
			Matrix3x3 This = *this;
			for (uInt i = 0; i < 3; ++i) {
				This.Columns[i] *= Vec;
				Result.Array[i] = This.Columns[i].x + This.Columns[i].y + This.Columns[i].z;
			}
			return Result;
		}

		constexpr Matrix3x3 operator * (const Matrix3x3& Mat) const noexcept {
			Matrix3x3 Result = { };
			for (uInt i = 0; i < 3; ++i) {
				Result.Columns[i] = this->Columns[0] * Mat.Columns[i].x;
				Result.Columns[i] += this->Columns[1] * Mat.Columns[i].y;
				Result.Columns[i] += this->Columns[2] * Mat.Columns[i].z;
			}
			return Result;
		}

		constexpr Matrix3x3 operator * (const Float& Num) const noexcept {
			Matrix3x3 Result = *this;
			for (uInt i = 0; i < 3; ++i)
				Result.Columns[i] *= Num;
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
				Float _11, _12, _13;
				Float _21, _22, _23;
				Float _31, _32, _33;
			};

			Float3D Columns[3];
			Float Matrix2D[3][3];
			Float Matrix[9] = { };
		};
	};
}