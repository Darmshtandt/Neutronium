#pragma once

#include <Nt/Graphics/Ex/Quaternion.h>
#include <Nt/Core/Math/Projections.h>

namespace NtEx {
	using namespace Nt;

	template <typename _Ty, uInt fromDimension, uInt toDimension>
	NT_NODISCARD Vector<_Ty, toDimension> ConvertVector(const Vector<_Ty, fromDimension>& vector) {
		Vector<_Ty, toDimension> result;

		const uInt minDimension = std::min(fromDimension, toDimension);
		for (uInt i = 0; i < minDimension; ++i)
			result.Array[i] = vector.Array[i];

		for (uInt i = minDimension; i < toDimension; ++i)
			result.Array[i] = _Ty(0);

		if (toDimension == 4)
			result.Array[3] = _Ty(1);

		return result;
	}

	NT_NODISCARD_CONSTEXPR Matrix4x4& TranslateWorld(Matrix4x4& matrix, const Float3D& vector) noexcept {
		if (vector.LengthSquare() != 0.f)
			matrix.Columns[3].xyz += vector;
		return matrix;
	}

	template <typename _Ty, uInt Dimension>
	struct Matrix {
		using VectorT = Vector<_Ty, Dimension>;

		NT_NODISCARD static
		Matrix4x4 LocalToWorldLH(const VectorT& position, const Quaternion& rotation, const VectorT& size) noexcept;
		NT_NODISCARD static
		Matrix4x4 LocalToWorldRH(const VectorT& position, const Quaternion& rotation, const VectorT& size) noexcept;

		NT_NODISCARD static
		Matrix4x4 WorldToLocalLH(const VectorT& position, const Quaternion& rotation, const VectorT& size) noexcept;
		NT_NODISCARD static
		Matrix4x4 WorldToLocalRH(const VectorT& position, const Quaternion& rotation, const VectorT& size) noexcept;


		NT_NODISCARD __inline static Float3D Right(const Matrix4x4& mat) noexcept {
			return mat.Columns[0].xyz;
		}
		NT_NODISCARD __inline static Float3D Up(const Matrix4x4& mat) noexcept {
			return mat.Columns[1].xyz;
		}
		NT_NODISCARD __inline static Float3D Forward(const Matrix4x4& mat) noexcept {
			return mat.Columns[2].xyz;
		}
		NT_NODISCARD __inline static Float3D Translation(const Matrix4x4& mat) noexcept {
			return mat.Columns[3].xyz;
		}

		NT_NODISCARD __inline static Float3D ExtractRotation(const Matrix4x4& mat) noexcept {
			const Float3D right = mat.Columns[0].xyz.GetNormalize();
			const Float3D up = mat.Columns[1].xyz.GetNormalize();
			const Float3D forward = mat.Columns[2].xyz.GetNormalize();

			return {
				atan2(up.z, forward.z),
				-asin(right.z),
				atan2(right.y, right.x)
			};
		}
	};


	template <typename _Ty, uInt Dimension>
	Matrix4x4 Matrix<_Ty, Dimension>::LocalToWorldLH(const VectorT& position, const Quaternion& rotation, const VectorT& size) noexcept {
		const Vector<_Ty, 3> correctPosition =
			ConvertVector<_Ty, Dimension, 3>(position);
		const Vector<_Ty, 3> correctSize =
			ConvertVector<_Ty, Dimension, 3>(size);

		const Matrix4x4 matScale = Matrix4x4::GetScale(correctSize);
		const Matrix4x4 matRotation = rotation.ToMatrix4x4();
		const Matrix4x4 matTranslate = Matrix4x4::GetTranslate(correctPosition);

		return matTranslate * matRotation * matScale;
	}
	template <typename _Ty, uInt Dimension>
	Matrix4x4 Matrix<_Ty, Dimension>::WorldToLocalLH(const VectorT& position, const Quaternion& rotation, const VectorT& size) noexcept {
		const Vector<_Ty, 3> correctPosition =
			ConvertVector<_Ty, Dimension, 3>(position);
		const Vector<_Ty, 3> correctSize =
			ConvertVector<_Ty, Dimension, 3>(size);

		const Matrix4x4 invScale = Matrix4x4::GetScale(_Ty(1) / correctSize);
		const Matrix4x4 invRotation = rotation.GetInverse().ToMatrix4x4();
		const Matrix4x4 invTranslate = Matrix4x4::GetTranslate(-correctPosition);

		return invScale * invRotation * invTranslate;
	}
}