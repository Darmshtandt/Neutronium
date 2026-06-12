#pragma once

#include <Nt/Core/Math/Matrix4x4.h>

namespace Nt {
	NT_API NT_NODISCARD Matrix4x4 ComputeFrustumLH(FloatRect rect, const Float& _near, const Float& _far);
	NT_API NT_NODISCARD Matrix4x4 ComputeFrustumRH(FloatRect rect, const Float& _near, const Float& _far);

	NT_API NT_NODISCARD Matrix4x4 ComputePerspectiveLH(const Float& fov, const Float& aspect, const Float& _near, const Float& _far);
	NT_API NT_NODISCARD Matrix4x4 ComputePerspectiveRH(const Float& fov, const Float& aspect, const Float& _near, const Float& _far);

	NT_API NT_NODISCARD Matrix4x4 ComputeOrthoLH(FloatRect rect, const Float& orthoNear, const Float& orthoFar) noexcept;
	NT_API NT_NODISCARD Matrix4x4 ComputeOrthoRH(FloatRect rect, const Float& orthoNear, const Float& orthoFar) noexcept;

	NT_API NT_NODISCARD Matrix4x4 ComputeOrthoLH2D(const FloatRect& rect) noexcept;
	NT_API NT_NODISCARD Matrix4x4 ComputeOrthoRH2D(const FloatRect& rect) noexcept;


	NT_API NT_NODISCARD Matrix4x4 MatrixRotateLHX(const Float& angle) noexcept;
	NT_API NT_NODISCARD Matrix4x4 MatrixRotateLHY(const Float& angle) noexcept;
	NT_API NT_NODISCARD Matrix4x4 MatrixRotateLHZ(const Float& angle) noexcept;
	NT_API NT_NODISCARD Matrix4x4 MatrixRotateLH(const Float3D& angle) noexcept;

	NT_API NT_NODISCARD Matrix4x4 MatrixRotateRHX(const Float& angle) noexcept;
	NT_API NT_NODISCARD Matrix4x4 MatrixRotateRHY(const Float& angle) noexcept;
	NT_API NT_NODISCARD Matrix4x4 MatrixRotateRHZ(const Float& angle) noexcept;
	NT_API NT_NODISCARD Matrix4x4 MatrixRotateRH(const Float3D& angle) noexcept;
}