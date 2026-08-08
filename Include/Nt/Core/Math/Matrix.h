#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	template <typename _Ty, uInt columns, uInt rows>
	struct MatrixBase {
		using VectorND = Vector<_Ty, columns>;
		using ValueType = _Ty;

		_CONSTEXPR23 static uInt ColumnsCount = columns;
		_CONSTEXPR23 static uInt RowsCount = rows;


		_CONSTEXPR23 MatrixBase() noexcept {
			Fill(_Ty(0));
		}
		_CONSTEXPR23 MatrixBase(std::initializer_list<_Ty> list) noexcept {
			constexpr uInt size = (ColumnsCount * RowsCount);

			uInt i = 0;
			for (const _Ty& num : list) {
				Array[i] = num;

				++i;
				if (i >= size)
					break;
			}
		}

		_CONSTEXPR23 MatrixBase& Fill(const _Ty& value) noexcept {
			for (uInt i = 0; i < ColumnsCount * RowsCount; ++i)
				Array[i] = value;

			return (*this);
		}

		_NODISCARD _CONSTEXPR23 Bool operator == (const MatrixBase& matrix) const noexcept {
			for (uInt i = 0; i < ColumnsCount; ++i) {
				for (uInt j = 0; j < RowsCount; ++j) {
					if (Array[i][j] != matrix.Array[i][j])
						return false;
				}
			}

			return true;
		}

		_NODISCARD _CONSTEXPR23 VectorND operator * (const VectorND& vector) const noexcept {
			VectorND result = { };
			for (uInt i = 0; i < rows; ++i) {
				const VectorND vectorProduction = this->Rows[i] * vector;

				for (uInt j = 0; j < columns; ++j)
					result.Array[i] += vectorProduction[j];
			}

			return result;
		}


		_CONSTEXPR23 const VectorND& operator [] (const uInt& Index) const noexcept {
			return RowsCount[Index];
		}
		_CONSTEXPR23 VectorND& operator [] (const uInt& Index) noexcept {
			return RowsCount[Index];
		}

		_CONSTEXPR23 MatrixBase& operator = (const MatrixBase& matrix) noexcept {
			Array = matrix.Array;
			return (*this);
		}

		union {
			std::array<Vector<_Ty, ColumnsCount>, RowsCount> Rows;
			std::array<std::array<_Ty, ColumnsCount>, RowsCount> Array2D;
			std::array<_Ty, ColumnsCount * RowsCount> Array;
		};
	};

	template <typename _Ty, uInt size>
	struct MatrixSquare : public MatrixBase<_Ty, size, size> {
		using VectorND = Vector<_Ty, columns>;
		using ValueType = _Ty;

		_CONSTEXPR23 MatrixSquare() noexcept
		{
		}
		_CONSTEXPR23 MatrixSquare(std::initializer_list<_Ty> list) noexcept :
			MatrixBase<_Ty, size, size>(list)
		{
		}

		_NODISCARD _CONSTEXPR23 MatrixSquare& Copy() const noexcept {
			return MatrixSquare(*this);
		}

		static _NODISCARD _CONSTEXPR23 MatrixSquare GetIdentity() noexcept {
			return MatrixSquare().MakeIdentity();
		}
		static _NODISCARD _CONSTEXPR23 MatrixSquare GetTranslate(const VectorND& vector) noexcept {
			return GetIdentity().Translate(vector);
		}
		static _NODISCARD _CONSTEXPR23 MatrixSquare GetScale(const VectorND& vector) noexcept {
			return GetIdentity().Scale(vector);
		}
		static _NODISCARD _CONSTEXPR23 Matrix4x4 GetRotateX(const Float& Angle) noexcept {
			return Matrix4x4{
				1.f, 0.f, 0.f, 0.f,
				0.f, cosf(Angle * RADf), -sinf(Angle * RADf), 0.f,
				0.f, sinf(Angle * RADf), cosf(Angle * RADf), 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static _NODISCARD _CONSTEXPR23 Matrix4x4 GetRotateY(const Float& Angle) noexcept {
			return Matrix4x4{
				cosf(Angle * RADf), 0.f, sinf(Angle * RADf), 0.f,
				0.f, 1.f, 0.f, 0.f,
				-sinf(Angle * RADf), 0.f, cosf(Angle * RADf), 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static _NODISCARD _CONSTEXPR23 Matrix4x4 GetRotateZ(const Float& Angle) noexcept {
			return  Matrix4x4{
				cosf(Angle * RADf), -sinf(Angle * RADf), 0.f, 0.f,
				sinf(Angle * RADf), cosf(Angle * RADf), 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}
		static _NODISCARD _CONSTEXPR23 Matrix4x4 GetRotate(const Float3D& Angle) noexcept {
			return GetRotateX(Angle.x) * GetRotateY(Angle.y) * GetRotateZ(Angle.z);
		}

		_CONSTEXPR23 MatrixSquare& MakeIdentity() noexcept {
			for (uInt i = 0; i < size; ++i) {
				for (uInt j = 0; j < size; ++j)
					Array2D[i][j] = (i == j) ? _Ty(1) : _Ty(0);
			}

			return (*this);
		}

		_CONSTEXPR23 MatrixSquare& Translate(const VectorND& vector) noexcept {
			for (uInt i = 0; i < size; ++i) {
				for (uInt j = 0; j < size - 1; ++j)
					Array2D[size - 1][i] += Array2D[j][i] * vector.Array[j];
			}

			return (*this);
		}
		_CONSTEXPR23 MatrixSquare& Scale(const Float3D& Vec) noexcept {
			(*this) *= GetScale(Vec);
			return (*this);
		}

		_CONSTEXPR23 MatrixSquare& RotateX(const Float& angle) noexcept {
			(*this) *= RotateMatrixX(angle);
			return (*this);
		}
		_CONSTEXPR23 MatrixSquare& RotateY(const Float& angle) noexcept {
			(*this) *= RotateMatrixY(angle);
			return (*this);
		}
		_CONSTEXPR23 MatrixSquare& RotateZ(const Float& angle) noexcept {
			(*this) *= RotateMatrixZ(angle);
			return (*this);
		}
		_CONSTEXPR23 MatrixSquare& Rotate(const Float3D& angle) noexcept {
			(*this) *= GetRotate(angle);
			return (*this);
		}

		_CONSTEXPR23 MatrixSquare Transposition() const noexcept {
			Matrix Result;
			for (uInt i = 0; i < 4; ++i)
				for (uInt j = i; j < 4; ++j)
					Result[i][j] = Matrix2D[j][i];
			return Result;
		}
		_NODISCARD _CONSTEXPR23 Float3D GetEulerangles() const noexcept {
			Float3D angle;
			angle.x = atan2(Rows[2][1], Rows[2][2]);
			angle.y = atan2(-Rows[2][0], sqrt(Rows[2][1] * Rows[2][1] + Rows[2][2] * Rows[2][2]));
			angle.z = atan2(Rows[1][0], Rows[0][0]);
			return angle;
		}

		_CONSTEXPR23 Float Det() const noexcept {
			Float result = 0.f;
			result += _11 * Minor(0, 0).Det();
			result -= _21 * Minor(1, 0).Det();
			result += _31 * Minor(2, 0).Det();
			result -= _41 * Minor(3, 0).Det();
			return result;
		}

		// Indexing starts at 0
		_CONSTEXPR23 Matrix3x3 Minor(const Float& x, const Float& y) const noexcept {
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
			return powf((-1.f), x + y) * Minor(x, y).Det();
		}
		Float AlgebraicComplement(const Float2D& XY) const noexcept {
			return powf((-1.f), XY.x + XY.y) * Minor(XY.x, XY.y).Det();
		}

		_CONSTEXPR23 Matrix GetInverse() const noexcept {
			Matrix MatA = { };
			for (uInt i = 0; i < 4; ++i)
				for (uInt j = 0; j < 4; ++j)
					MatA[j][i] = AlgebraicComplement(uInt2D(i, j));

			Float MatADet = Matrix2D[0][0] * MatA[0][0];
			MatADet += Matrix2D[0][1] * MatA[0][1];
			MatADet += Matrix2D[0][2] * MatA[0][2];
			MatADet += Matrix2D[0][3] * MatA[0][3];

			return MatA * (1.f / MatADet);
		}

		_CONSTEXPR23 Matrix operator * (const Matrix& matrix) const noexcept {
			Matrix result = { };

			for (uInt i = 0; i < RowsCount; ++i) {
				for (uInt j = 0; j < ColumnsCount; ++j) {
					result.Array[i][j] +=
				}

				result.Rows[i] = this->RowsCount[0] * matrix.Rows[i].x;
				result.Rows[i] += this->RowsCount[1] * matrix.Rows[i].y;
				result.Rows[i] += this->RowsCount[2] * matrix.Rows[i].z;
				result.Rows[i] += this->RowsCount[3] * matrix.Rows[i].w;
			}
			return result;
		}

		_CONSTEXPR23 Matrix operator * (const Float& Num) const noexcept {
			Matrix Result = *this;
			for (uInt i = 0; i < 4; ++i)
				Result.Rows[i] *= Num;
			return Result;
		}

		_CONSTEXPR23 Matrix operator *= (const Matrix& Mat) noexcept {
			(*this) = (*this) * Mat;
			return (*this);
		}
		_CONSTEXPR23 Matrix operator *= (const Float& Num) noexcept {
			(*this) = (*this) * Num;
			return (*this);
		}
	};

	template <typename _Ty>
	using Matrix2x2 = Matrix<_Ty, 2, 2>;

	template <typename _Ty>
	using Matrix3x3 = Matrix<_Ty, 3, 3>;

	template <typename _Ty>
	using Matrix4x4 = Matrix<_Ty, 4, 4>;

	using FloatMatrix2x2 = Matrix<Float, 2, 2>;
	using FloatMatrix3x3 = Matrix<Float, 3, 3>;
	using FloatMatrix4x4 = Matrix<Float, 4, 4>;


	template <typename _Ty>
	_CONSTEXPR23 Matrix3x3<_Ty> RotateMatrixX(const Float& angle) noexcept {
		return Matrix3x3<_Ty> {
			1.f, 0.f, 0.f,
			0.f, cosf(angle * RADf), -sinf(angle * RADf),
			0.f, sinf(angle * RADf), cosf(angle * RADf)
		};
	}

	template <typename _Ty>
	_CONSTEXPR23 Matrix3x3<_Ty> RotateMatrixY(const Float& angle) noexcept {
		return Matrix3x3<_Ty> {
			cosf(angle * RADf), 0.f, sinf(angle * RADf),
			0.f, 1.f, 0.f,
			-sinf(angle * RADf), 0.f, cosf(angle * RADf)
		};
	}

	template <typename _Ty>
	_CONSTEXPR23 Matrix3x3<_Ty> RotateMatrixZ(const Float& angle) noexcept {
		return  Matrix3x3<_Ty> {
			cosf(angle * RADf), -sinf(angle * RADf), 0.f,
			sinf(angle * RADf), cosf(angle * RADf), 0.f,
			0.f, 0.f, 1.f
		};
	}

	template <typename _Ty>
	_CONSTEXPR23 Matrix3x3<_Ty> RotateMatrix(const Float3D& angle) noexcept {
		return RotateMatrixX(angle.x) * RotateMatrixY(angle.y) * RotateMatrixZ(angle.z);
	}
}