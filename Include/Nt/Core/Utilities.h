#pragma once

#include <Nt/Core/NtDLL.h>
#include <typeinfo>
#include <string>
#include <memory>

#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>

#define SAFE_DELETE(p) do { if (*p) { delete(*p); (*p) = nullptr; } } while (false)
#define SAFE_RELEASE(p) do { if (*p) { (*p)->Release(); delete(*p); (*p) = nullptr; } } while (false)

#ifndef DCX_USESTYLE
#	define DCX_USESTYLE 0x00010000
#endif

#ifdef ZeroMemory
#	undef ZeroMemory
#endif

#ifdef PI
#	undef PI
#endif

#ifdef RAD
#	undef RAD
#endif

#ifdef min
#	undef min
#endif

#ifdef max
#	undef max
#endif

#define Requires(...) typename = std::enable_if_t<__VA_ARGS__>

#include <Nt/Core/NtError.h>

constexpr Float PIf = 3.1415927f;
constexpr Double PI = 3.141592653589793;

constexpr Float RADf = (PIf / 180.f);
constexpr Double RAD = (PI / 180.0);

namespace Nt {
	template <template <typename ...> class, template <typename ...> class>
	struct Is_Same_Template : std::false_type 
	{
	};

	template <template <typename ...> class _C>
	struct Is_Same_Template<_C, _C> : std::true_type
	{
	};

	template <template <typename ...> class _Ty, template <typename ...> class _U>
	constexpr Bool Is_Same_Template_v = Is_Same_Template<_Ty, _U>::value;

// ============================================================================
//		Functions
// ----------------------------------------------------------------------------
	NT_API void _ShowCursor(const Bool& fShow) noexcept;

	NT_FORCE_INLINE void __nop() noexcept 
	{
	}

	NT_FORCE_INLINE void* ZeroMemory(void* ptr, const uInt& size) {
		return memset(ptr, 0, size);
	}

	template <typename _Ty, typename _U>
	NT_NODISCARD_CONSTEXPR _U CostexprCast(const _Ty& value) noexcept {
		using UTypeLimit = std::numeric_limits<_U>;

		if NT_CONSTEXPR (value < static_cast<_Ty>(UTypeLimit::min()))
			return UTypeLimit::min();
		else if NT_CONSTEXPR (value > static_cast<_Ty>(UTypeLimit::max()))
			return UTypeLimit::max();
		return static_cast<_U>(value);
	}
}

#include <Nt/Core/NotNull.h>