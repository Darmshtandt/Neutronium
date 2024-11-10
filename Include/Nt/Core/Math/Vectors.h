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
	__inline Vector2D<_Ty> ceil(const Vector2D<_Ty>& vec) noexcept {
		return Vector2D(std::ceil(vec.x), std::ceil(vec.y));
	}

	template <typename _Ty>
	__inline Vector2D<_Ty> ceil(const Vector3D<_Ty>& vec) noexcept {
		return Vector3D(std::ceil(vec.x), std::ceil(vec.y), std::ceil(vec.z));
	}

	template <typename _Ty>
	__inline Vector2D<_Ty> ceil(const Vector4D<_Ty>& vec) noexcept {
		return Vector4D(std::ceil(vec.x), std::ceil(vec.y), std::ceil(vec.z), std::ceil(vec.w));
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


	template <typename _Ty>
	__inline Vector2D<_Ty> Min(const Vector2D<_Ty>& leftVector, const Vector2D<_Ty>& rightVector) noexcept {
		return Vector2D(min(leftVector.x, rightVector.x), min(leftVector.y, rightVector.y));
	}
	template <typename _Ty>
	__inline Vector3D<_Ty> Min(const Vector3D<_Ty>& leftVector, const Vector3D<_Ty>& rightVector) noexcept {
		return Vector3D(min(leftVector.x, rightVector.x), min(leftVector.y, rightVector.y), min(leftVector.z, rightVector.z));
	}
	template <typename _Ty>
	__inline Vector4D<_Ty> Min(const Vector4D<_Ty>& leftVector, const Vector4D<_Ty>& rightVector) noexcept {
		return Vector4D(min(leftVector.x, rightVector.x), min(leftVector.y, rightVector.y), min(leftVector.z, rightVector.z), min(leftVector.w, rightVector.w));
	}


	template <typename _Ty>
	__inline Vector2D<_Ty> Max(const Vector2D<_Ty>& leftVector, const Vector2D<_Ty>& rightVector) noexcept {
		return Vector2D(max(leftVector.x, rightVector.x), max(leftVector.y, rightVector.y));
	}
	template <typename _Ty>
	__inline Vector3D<_Ty> Max(const Vector3D<_Ty>& leftVector, const Vector3D<_Ty>& rightVector) noexcept {
		return Vector3D(max(leftVector.x, rightVector.x), max(leftVector.y, rightVector.y), max(leftVector.z, rightVector.z));
	}
	template <typename _Ty>
	__inline Vector4D<_Ty> Max(const Vector4D<_Ty>& leftVector, const Vector4D<_Ty>& rightVector) noexcept {
		return Vector4D(max(leftVector.x, rightVector.x), max(leftVector.y, rightVector.y), max(leftVector.z, rightVector.z), max(leftVector.w, rightVector.w));
	}
}