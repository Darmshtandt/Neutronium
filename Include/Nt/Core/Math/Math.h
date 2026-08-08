#pragma once

#include <algorithm>
#include <Nt/Core/Utilities.h>

#ifdef min
#	undef min
#endif

#ifdef max
#	undef max
#endif

namespace Nt {
	template <typename _Ty>
	NT_NODISCARD_CONSTEXPR
	_Ty Permute(const _Ty& x) noexcept {
		if NT_CONSTEXPR(std::is_integral_v<_Ty>)
			return (((x * 34.0) + 1) * x) % 289.0;
		return fmod(((x * 34.0) + 1) * x, 289.0);
	}

	template <typename _Ty>
	NT_NODISCARD_CONSTEXPR
	_Ty Fade(const _Ty& t) noexcept {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	template <typename _Ty>
	NT_NODISCARD_CONSTEXPR
	_Ty Lerp(const _Ty& a, const _Ty& b, const _Ty& t) noexcept {
		return a + t * (b - a);
	}

	template <typename _Ty>
	NT_NODISCARD_CONSTEXPR
	_Ty Fract(const _Ty& x) noexcept {
		return x - _Ty(Int(x));
	}

	template <typename _Ty>
	NT_NODISCARD_CONSTEXPR
	_Ty Clamp(const _Ty& value, const _Ty& minValue, const _Ty& maxValue) {
		return std::min(std::max(value, minValue), maxValue);
	}

	template <typename _Ty, Requires(std::is_arithmetic_v<_Ty>)>
	NT_NODISCARD_CONSTEXPR 
	_Ty Abs(const _Ty& value) noexcept {
		return (value < 0) ? -value : value;
	}

	template <typename _Ty, Requires(std::is_arithmetic_v<_Ty>)>
	NT_NODISCARD_CONSTEXPR 
	_Ty Trunc(const _Ty& value) noexcept {
		return static_cast<_Ty>(static_cast<LLong>(value));
	}
}