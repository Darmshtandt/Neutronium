#pragma once

#include <windows.h>
#include <string>
#include <filesystem>

#ifndef min
#	define min(a, b) ((a < b) ? a : b)
#endif

#ifndef max
#	define mac(a, b) ((a > b) ? a : b)
#endif

#ifndef ZeroMemory
#	define ZeroMemory(Dest, Size) memset(Dest, 0, Size)
#endif

#define RaiseWithCaption(Msg, Caption) \
	if (Nt::g_NtExcepts) \
		throw Nt::Error(Msg, Caption, __FILE__, __LINE__, __FUNCTION__)
#define RaiseWithoutCaption(Msg) \
	if (Nt::g_NtExcepts) \
		throw Nt::Error(Msg, "Error", __FILE__, __LINE__, __FUNCTION__)
#define RaiseSelect(_1, _2, NAME, ...) NAME
#define Raise(...) RaiseSelect(__VA_ARGS__, RaiseWithCaption, RaiseWithoutCaption)(__VA_ARGS__)
#define Raise(Msg, Caption) RaiseWithCaption(Msg, Caption)
#define Raise(Msg) RaiseWithoutCaption(Msg)

#define AssertWithCaption(Expression, AssertMessage, AssertCaption) \
	if (!Expression) \
		RaiseWithCaption(AssertMessage, AssertCaption)
#define AssertWithoutCaption(Expression, AssertMessage) \
	if (!Expression) \
		RaiseWithoutCaption(AssertMessage)
#define AssertSelect(_1, _2, _3, NAME, ...) NAME
#define Assert(...) AssertSelect(__VA_ARGS__, AssertWithCaption, AssertWithoutCaption)(__VA_ARGS__)


namespace Nt {
	inline Bool g_NtExcepts = true;

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
//		Funcions
// ----------------------------------------------------------------------------
	NT_API void _ShowCursor(const Bool& fShow) noexcept;
	__inline void __nop() noexcept 
	{
	}

	//NT_API String GetFileExtension(String Path);

	struct Error : public std::exception {
		NT_API Error(
			const String& Message,
			const String& Caption,
			const String& FileName,
			const String& Line,
			const String& FunctionName = "") noexcept;

		NT_API void Show() const noexcept;

		String Caption;
		String FileName;
		String Line;
		String FunctionName;
	};

	using RunTimeError = std::runtime_error;
}