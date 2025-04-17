#pragma once

#include <stacktrace>

#pragma warning(disable : 4002)
#pragma warning(disable : 4003)
#pragma warning(disable : 4005)

#define RaiseWithCaption(Msg, Caption) \
	throw Nt::Error(Msg, Caption, __FILE__, __LINE__, __FUNCTION__, std::stacktrace::current())

#define RaiseWithoutCaption(Msg) \
	throw Nt::Error(Msg, "Error", __FILE__, __LINE__, __FUNCTION__, std::stacktrace::current())

#define RaiseSelect(_1, _2, NAME, ...) NAME
#define Raise(Msg, Caption) RaiseWithCaption(Msg, Caption)
#define Raise(Msg) RaiseWithoutCaption(Msg)

#define AssertWithCaption(Expression, AssertMessage, AssertCaption) \
	if (!(Expression)) RaiseWithCaption(AssertMessage, AssertCaption)

#define AssertWithoutCaption(Expression, AssertMessage) \
	if (!(Expression)) RaiseWithoutCaption(AssertMessage)

#define AssertSelect(_1, _2, _3, NAME, ...) NAME
#define Assert(Expression, Msg, Caption) AssertWithCaption(Expression, Msg, Caption)
#define Assert(Expression, Msg) AssertWithoutCaption(Expression, Msg)


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