#pragma once

#include <stacktrace>

#pragma warning(disable : 4002)
#pragma warning(disable : 4003)
#pragma warning(disable : 4005)

#ifdef _DEBUG
#	define RaiseWithCaption(msg, caption) \
	throw Nt::Error(msg, caption, __FILE__, __LINE__, __FUNCTION__, std::stacktrace::current())

#	define RaiseWithoutCaption(msg) \
	RaiseWithCaption(msg, "Error")
#else
#	include <Nt/Core/Log.h>

#	define RaiseWithCaption(msg, caption) \
	Nt::Log::Instance().Error(Nt::String('[') + caption + "]: " + msg)

#	define RaiseWithoutCaption(msg) \
	RaiseWithCaption(msg, "Error")
#endif

#define RaiseSelect(_1, _2, NAME, ...) NAME
#define Raise(msg, caption) RaiseWithCaption(msg, caption)
#define Raise(msg) RaiseWithoutCaption(msg)

#define AssertWithCaption(expression, assertMessage, assertCaption) \
	if (!(expression)) RaiseWithCaption(assertMessage, assertCaption)

#define AssertWithoutCaption(expression, assertMessage) \
	if (!(expression)) RaiseWithoutCaption(assertMessage)

#define AssertSelect(_1, _2, _3, NAME, ...) NAME
#define Assert(expression, msg, caption) AssertWithCaption(expression, msg, caption)
#define Assert(expression, msg) AssertWithoutCaption(expression, msg)


namespace Nt {
	inline Bool g_NtStacktraceEnabled = true;

	struct Error : public std::exception {
		NT_API Error(const String& Message,
			const String& Caption,
			const String& FileName,
			const String& Line,
			const String& FunctionName = "", const std::stacktrace& stacktrace = {}) noexcept;

		NT_API void Show() const noexcept;

		String Caption;
		String FileName;
		String Line;
		String FunctionName;
		std::stacktrace Stacktrace;
	};
}