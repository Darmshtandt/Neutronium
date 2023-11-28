#include <iostream>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Log.h>

namespace Nt {
	void Log::Successfull(const String& Message) noexcept {
		if (m_EnableOutput && m_EnableOutputSuccessfulls)
			std::cout << "[SUCCESSFULL]: " << Message << std::endl;
	}
	void Log::Warning(const String& Message) noexcept {
		if (m_EnableOutput && m_EnableOutputWarnings)
			std::cerr << "[WARNING]: " << Message << std::endl;
	}
	void Log::Error(const String& Message) noexcept {
		if (m_EnableOutput && m_EnableOutputErrors)
			std::cerr << "[ERROR]: " << Message << std::endl;
	}
	void Log::Message(const String& Message) noexcept {
		if (m_EnableOutput && m_EnableOutputMessages)
			std::cout << "[MESSAGE]: " << Message << std::endl;
	}


	bool Log::m_EnableOutput = true;
	bool Log::m_EnableOutputSuccessfulls = true;
	bool Log::m_EnableOutputWarnings = true;
	bool Log::m_EnableOutputErrors = true;
	bool Log::m_EnableOutputMessages = true;
}