#pragma once

#include <algorithm>
#include <Nt/Core/NtTypes.h>

#ifdef min
#	undef min
#endif

#ifdef max
#	undef max
#endif

namespace Nt {
	template <typename _Ty>
	_NODISCARD _CONSTEXPR20
	_Ty Permute(const _Ty& x) noexcept {
		if constexpr (std::is_integral_v<_Ty>)
			return (((x * 34.0) + 1) * x) % 289.0;
		return fmod(((x * 34.0) + 1) * x, 289.0);
	}

	template <typename _Ty>
	_NODISCARD _CONSTEXPR20
	_Ty Fade(const _Ty& t) noexcept {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	template <typename _Ty>
	_NODISCARD _CONSTEXPR20
	_Ty Lerp(const _Ty& a, const _Ty& b, const _Ty& t) noexcept {
		return a + t * (b - a);
	}

	template <typename _Ty>
	_NODISCARD _CONSTEXPR20
	_Ty Fract(const _Ty& x) noexcept {
		return x - _Ty(Int(x));
	}

	template <typename _Ty>
	_NODISCARD _CONSTEXPR23
	_Ty Clamp(const _Ty& value, const _Ty& minValue, const _Ty& maxValue) {
		return std::min(std::max(value, minValue), maxValue);
	}
}