#pragma once

#ifndef NT_DLL
#	define NT_API  __declspec(dllimport)
#else
#	define NT_API __declspec(dllexport)
#endif

#if defined(_MSC_VER)
#	define NT_FORCE_INLINE __forceinline

#	if _MSVC_LANG >= 201103L
#		define NT_CONSTEXPR constexpr
#	else
#		define NT_CONSTEXPR
#	endif
#else
#	if __cplusplus >= 201103L
#		define NT_CONSTEXPR constexpr
#	else
#		define NT_CONSTEXPR
#	endif

#	if defined(__GNUC__) || defined(__clang__)
#		define NT_FORCE_INLINE __attribute__((always_inline)) inline
#	else
#		define NT_FORCE_INLINE inline
#	endif
#endif

#define NT_MAYBE_UNUSED [[maybe_unused]]
#define NT_NODISCARD [[nodiscard]]

#define NT_NODISCARD_CONSTEXPR NT_NODISCARD NT_CONSTEXPR