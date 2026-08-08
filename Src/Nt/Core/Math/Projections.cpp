#include <Nt/Core/Math/Projections.h>

namespace Nt {
	Matrix4x4 ComputeFrustumLH(FloatRect rect, const Float& _near, const Float& _far) {
		Matrix4x4 projection;
		projection._11 = (2.f * _near) / (rect.Right - rect.Left);
		projection._22 = (2.f * _near) / (rect.Top - rect.Bottom);
		projection._33 = (_far + _near) / (_far - _near);

		projection._31 = (rect.Right + rect.Left) / (rect.Right - rect.Left);
		projection._32 = (rect.Top + rect.Bottom) / (rect.Top - rect.Bottom);
		projection._43 = -(2 * _far * _near) / (_far - _near);
		projection._34 = 1.f;
		return projection;
	}

	Matrix4x4 ComputeFrustumRH(FloatRect rect, const Float& _near, const Float& _far) {
		Matrix4x4 projection;
		projection._11 = (2.f * _near) / (rect.Right - rect.Left);
		projection._22 = (2.f * _near) / (rect.Top - rect.Bottom);
		projection._33 = -(_far + _near) / (_far - _near);

		projection._31 = (rect.Right + rect.Left) / (rect.Right - rect.Left);
		projection._32 = (rect.Top + rect.Bottom) / (rect.Top - rect.Bottom);
		projection._43 = -(2 * _far * _near) / (_far - _near);
		projection._34 = -1.f;
		return projection;
	}

	Matrix4x4 ComputePerspectiveLH(const Float& fov, const Float& aspect, const Float& _near, const Float& _far) {
		const Float tanFOV = std::tanf(fov / 2.f * RADf);

		Matrix4x4 projection;
		projection._11 = 1.f / (tanFOV * aspect);
		projection._22 = (1.f / tanFOV);
		projection._33 = (_far + _near) / (_far - _near);
		projection._43 = -(2.f * _far * _near) / (_far - _near);
		projection._34 = 1.f;
		projection._44 = 0.f;

		return projection;
	}
	Matrix4x4 ComputePerspectiveRH(const Float& fov, const Float& aspect, const Float& _near, const Float& _far) {
		const Float tanFOV = std::tanf(fov / 2.f * RADf);

		Matrix4x4 projection;
		projection._11 = 1.f / (tanFOV * aspect);
		projection._22 = (1.f / tanFOV);
		projection._33 = -(_far + _near) / (_far - _near);
		projection._43 = -(2.f * _far * _near) / (_far - _near);
		projection._34 = -1.f;
		projection._44 = 0.f;

		return projection;
	}

	Matrix4x4 ComputeOrthoLH(FloatRect rect, const Float& orthoNear, const Float& orthoFar) noexcept {
		Matrix4x4 projection;
		projection._11 = 2.f / (rect.Right - rect.Left);
		projection._22 = 2.f / (rect.Top - rect.Bottom);
		projection._33 = 2.f / (orthoFar - orthoNear);
		projection._44 = 1.f;

		projection._41 = -(rect.Right + rect.Left) / (rect.Right - rect.Left);
		projection._42 = -(rect.Top + rect.Bottom) / (rect.Top - rect.Bottom);
		projection._43 = -(orthoFar + orthoNear) / (orthoFar - orthoNear);
		return projection;
	}
	Matrix4x4 ComputeOrthoRH(FloatRect rect, const Float& orthoNear, const Float& orthoFar) noexcept {
		Matrix4x4 projection;
		projection._11 = 2.f / (rect.Right - rect.Left);
		projection._22 = 2.f / (rect.Top - rect.Bottom);
		projection._33 = -2.f / (orthoFar - orthoNear);
		projection._44 = 1.f;

		projection._41 = -(rect.Right + rect.Left) / (rect.Right - rect.Left);
		projection._42 = -(rect.Top + rect.Bottom) / (rect.Top - rect.Bottom);
		projection._43 = -(orthoFar + orthoNear) / (orthoFar - orthoNear);
		return projection;
	}

	Matrix4x4 ComputeOrthoLH2D(const FloatRect& rect) noexcept {
		return ComputeOrthoLH(rect, -1.f, 1.f);
	}
	Matrix4x4 ComputeOrthoRH2D(const FloatRect& rect) noexcept {
		return ComputeOrthoRH(rect, -1.f, 1.f);
	}


	Matrix4x4 MatrixRotateLHX(const Float& angle) noexcept {
		return {
			1.f, 0.f, 0.f, 0.f,
			0.f, cosf(angle), -sinf(angle), 0.f,
			0.f, sinf(angle), cosf(angle), 0.f,
			0.f, 0.f, 0.f, 1.f,
		};
	};
	Matrix4x4 MatrixRotateLHY(const Float& angle) noexcept {
		return {
			cosf(angle), 0.f, sinf(angle), 0.f,
			0.f, 1.f, 0.f, 0.f,
			-sinf(angle), 0.f, cosf(angle), 0.f,
			0.f, 0.f, 0.f, 1.f,
		};
	};
	Matrix4x4 MatrixRotateLHZ(const Float& angle) noexcept {
		return {
			cosf(angle), -sinf(angle), 0.f, 0.f,
			sinf(angle), cosf(angle), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f,
		};
	};
	Matrix4x4 MatrixRotateLH(const Float3D& angle) noexcept {
		return MatrixRotateLHX(angle.x) * MatrixRotateLHY(angle.y) * MatrixRotateLHZ(angle.z);
	};

	Matrix4x4 MatrixRotateRHX(const Float& angle) noexcept {
		return {
			1.f, 0.f, 0.f, 0.f,
			0.f, cosf(angle), sinf(angle), 0.f,
			0.f, -sinf(angle), cosf(angle), 0.f,
			0.f, 0.f, 0.f, 1.f,
		};
	};
	Matrix4x4 MatrixRotateRHY(const Float& angle) noexcept {
		return {
			cosf(angle), 0.f, -sinf(angle), 0.f,
			0.f, 1.f, 0.f, 0.f,
			sinf(angle), 0.f, cosf(angle), 0.f,
			0.f, 0.f, 0.f, 1.f,
		};
	};
	Matrix4x4 MatrixRotateRHZ(const Float& angle) noexcept {
		return {
			cosf(angle), sinf(angle), 0.f, 0.f,
			-sinf(angle), cosf(angle), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f,
		};
	};
	Matrix4x4 MatrixRotateRH(const Float3D& angle) noexcept {
		return MatrixRotateRHX(angle.x) * MatrixRotateRHY(angle.y) * MatrixRotateRHZ(angle.z);
	};
}