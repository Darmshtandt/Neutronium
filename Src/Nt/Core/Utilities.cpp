#include <windows.h>
#include <GL/GL.h>
#include <string>
#include <filesystem>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#pragma comment(lib, "OpenGL32")

namespace Nt {
	void _ShowCursor(const Bool& fShow) noexcept {
		SetCursor(fShow ? LoadCursor(nullptr, IDC_ARROW) : nullptr);
	}

	Error::Error(const String& Message, const String& Caption,
		const String& FileName, const String& Line, const String& FunctionName) noexcept :
		std::exception(Message.c_str()),
		Caption(Caption),
		FileName(FileName),
		Line(Line),
		FunctionName(FunctionName) 
	{ 
	}

	void Error::Show() const noexcept {
		String FullErrorMessage;
		FullErrorMessage += "====================";
		FullErrorMessage += "\nFile name:\n" + FileName;
		FullErrorMessage += "\n\nFunction name:\n" + FunctionName;
		FullErrorMessage += "\n\nLine: " + Line;
		
		const Int WinaApi_ErorCode = GetLastError();
		if (WinaApi_ErorCode != 0) {
			FullErrorMessage += "\n====================";
			FullErrorMessage += "\nWinApi error: ";

			Char* messageBuffer = nullptr;
			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, 
				WinaApi_ErorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				messageBuffer, 
				0, 
				nullptr);

			if (messageBuffer == nullptr)
				FullErrorMessage += WinaApi_ErorCode;
			else
				FullErrorMessage += messageBuffer;
		}

		const Int OpenGL_ErrorCode = glGetError();
		if (OpenGL_ErrorCode != 0) {
			FullErrorMessage += "\n====================";
			FullErrorMessage += "\nOpenGL error code: ";
			FullErrorMessage += OpenGL_ErrorCode;
		}

		FullErrorMessage += "\n====================";
		FullErrorMessage += "\nMessage:\n";
		FullErrorMessage += what();
		FullErrorMessage += "\n====================";

		Log::Error('\n' + FullErrorMessage);
		ERROR_MSGA(FullErrorMessage, Caption);
	}
}