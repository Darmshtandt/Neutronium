#pragma once

#include <cmath>
#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>

#include <Nt/Core/Math/Vector2D.h>
#include <Nt/Core/Math/Vector3D.h>
#include <Nt/Core/Math/Vector4D.h>

namespace Nt {
	template <typename _Ty>
	__inline Vector2D<_Ty> round(const Vector2D<_Ty>& vec) noexcept {
		return Vector2D(std::round(vec.x), std::round(vec.y));
	}

	template <typename _Ty>
	__inline Vector2D<_Ty> round(const Vector3D<_Ty>& vec) noexcept {
		return Vector3D(std::round(vec.x), std::round(vec.y), std::round(vec.z));
	}

	template <typename _Ty>
	__inline Vector2D<_Ty> round(const Vector4D<_Ty>& vec) noexcept {
		return Vector4D(std::round(vec.x), std::round(vec.y), std::round(vec.z), std::round(vec.w));
	}


	template <typename _Ty>
	__inline Vector2D<_Ty> abs(const Vector2D<_Ty>& vec) noexcept {
		return Vector2D(std::abs(vec.x), std::abs(vec.y));
	}
	template <typename _Ty>
	__inline Vector3D<_Ty> abs(const Vector3D<_Ty>& vec) noexcept {
		return Vector3D(std::abs(vec.x), std::abs(vec.y), std::abs(vec.z));
	}
	template <typename _Ty>
	__inline Vector4D<_Ty> abs(const Vector4D<_Ty>& vec) noexcept {
		return Vector4D(std::abs(vec.x), std::abs(vec.y), std::abs(vec.z), std::abs(vec.w));
	}
}