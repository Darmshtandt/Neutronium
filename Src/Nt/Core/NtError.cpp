// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <filesystem>
#include <string>

#include <Nt/Core/MessageWindow.h>
#include <Nt/Core/Log.h>
#include <Nt/Core/WinMinimal.h>

#include <windows.h>
#include <GL/GL.h>

#pragma comment(lib, "OpenGL32")

namespace Nt {
	Nt::Error::Error(const String& Message, const String& Caption,
		const String& FileName, const String& Line, const String& FunctionName, 
		const std::stacktrace& stacktrace) noexcept :

		std::exception(Message.c_str()),
		Caption(Caption),
		FileName(FileName),
		Line(Line),
		FunctionName(FunctionName),
		Stacktrace(stacktrace)
	{
	}

	void Error::Show() const noexcept {
		String fullErrorMessage;
		fullErrorMessage += "====================";
		fullErrorMessage += "\nFile name:\n" + FileName;
		fullErrorMessage += "\n\nFunction name:\n" + FunctionName;
		fullErrorMessage += "\n\nLine: " + Line;

		const Int winaApi_ErorCode = GetLastError();
		if (winaApi_ErorCode != 0) {
			fullErrorMessage += "\n====================";
			fullErrorMessage += "\nWinApi error: ";

			Char* messageBuffer = nullptr;
			const DWord messageSize = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				winaApi_ErorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&messageBuffer),
				0,
				nullptr);

			if (messageSize == 0 || messageBuffer == nullptr)
				fullErrorMessage += winaApi_ErorCode;
			else
				fullErrorMessage += messageBuffer;

			if (messageBuffer != nullptr)
				LocalFree(messageBuffer);
		}

		const Int openGL_ErrorCode = glGetError();
		if (openGL_ErrorCode != 0) {
			fullErrorMessage += "\n====================";
			fullErrorMessage += "\nOpenGL error code: ";
			fullErrorMessage += openGL_ErrorCode;
		}

		fullErrorMessage += "\n====================";
		fullErrorMessage += "\nMessage:\n";
		fullErrorMessage += what();
		fullErrorMessage += "\n====================";

		if (g_NtStacktraceEnabled) {
			std::ostringstream stream;
			stream << Stacktrace;

			fullErrorMessage += "\nStacktrace:\n";
			fullErrorMessage += stream.str();
			fullErrorMessage += "\n====================";
		}

		Log::Error('\n' + fullErrorMessage);
		MessageWindow(fullErrorMessage, Caption).Show(MessageIcon::ERROR);
	}
}