#pragma once

#ifndef NT_DLL
#	define NT_API  __declspec(dllimport)
#else
#	define NT_API __declspec(dllexport)
#endif

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

#include <Nt/Core/NtError.h>

constexpr Float PIf = 3.1415926535897932384636433832795f;
constexpr Double PI = 3.1415926535897932384636433832795;

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

	__inline void __nop() noexcept 
	{
	}

	__inline void* ZeroMemory(void* ptr, const uInt& size) {
		return memset(ptr, 0, size);
	}

	template <typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
	_NODISCARD _CONSTEXPR20 _Ty Abs(const _Ty& value) noexcept {
		return (value < 0) ? -value : value;
	}

	template <class _Ty>
	_Ty RequireNotNull(const _Ty& pointer, const std::string_view& message) {
		if (pointer == nullptr)
			Raise(message);

		return pointer;
	}

	template <class _Ty>
	_Ty* RequireNotNull(const std::unique_ptr<_Ty>& pointer) {
		if (pointer == nullptr)
			Raise(std::string(typeid(_Ty).name()) + " is null");

		return pointer.get();
	}

	template <class _Ty>
	_Ty* RequireNotNull(std::unique_ptr<_Ty>& pointer) {
		if (pointer == nullptr)
			Raise(std::string(typeid(_Ty).name()) + " is null");

		return pointer.get();
	}

	template <class _Ty> requires std::is_pointer_v<_Ty>
	_Ty RequireNotNull(_Ty pointer) {
		if (pointer == nullptr)
			Raise(std::string(typeid(_Ty).name()) + " is null");

		return pointer;
	}
}