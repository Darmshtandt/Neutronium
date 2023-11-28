#include <windows.h>
#include <string>
#include <filesystem>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

namespace Nt {
	void _ShowCursor(const Bool& fShow) noexcept {
		SetCursor(fShow ? LoadCursor(nullptr, IDC_ARROW) : nullptr);
	}

	//String GetFileExtension(String Path) {
	//	return std::filesystem::path(Path).extension().string();
	//}

	Error::Error(const String& Message, const String& Caption,
		const String& FileName, const String& Line, const String& FunctionName) noexcept :
		std::exception(Message.c_str()),
		Caption(Caption),
		FileName(FileName),
		Line(Line),
		FunctionName(FunctionName) 
	{ }
	void Error::Show() const noexcept {
		String FullErrorMessage;
		FullErrorMessage += "====================";
		FullErrorMessage += "\nFile name:\n" + FileName;
		FullErrorMessage += "\n\nFunction name:\n" + FunctionName;
		FullErrorMessage += "\n\nLine: " + Line;
		FullErrorMessage += "\n====================";
		FullErrorMessage += "\nMessage:\n";
		FullErrorMessage += what();
		FullErrorMessage += "\n====================";

		Log::Error('\n' + FullErrorMessage);
		ERROR_MSGA(FullErrorMessage, Caption);
	}
}