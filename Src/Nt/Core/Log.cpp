// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#include <windows.h>
#include <iostream>

#include <Nt/Core/Log.h>

namespace Nt {
	Log::Log() noexcept :
		m_MessageStream(std::cout),
		m_ErrorStream(std::cerr),
		m_WarningStream(std::clog)
	{
	}

	Log& Log::Instance() noexcept {
		static Log i;
		return i;
	}

	void Log::Successful(const String& message) noexcept {
		if (m_EnableOutput && m_EnableOutputSuccessfulls)
			m_MessageStream.get() << "[SUCCESSFUL]: " << message << '\n';
	}
	void Log::Warning(const String& message) noexcept {
		if (m_EnableOutput && m_EnableOutputWarnings) {
			m_WarningStream.get() << "[WARNING]: " << message << '\n';

			const uInt lastErrorCode = GetLastError();
			if (lastErrorCode != 0) {
				m_ErrorStream.get() << "[WinApi error] code: " << lastErrorCode << '\n';
				SetLastError(lastErrorCode);
			}
		}
	}
	void Log::Error(const String& message) noexcept {
		if (m_EnableOutput && m_EnableOutputErrors)
			m_ErrorStream.get() << "[ERROR]: " << message << '\n';
	}
	void Log::Message(const String& message) noexcept {
		if (m_EnableOutput && m_EnableOutputMessages)
			m_MessageStream.get() << "[MESSAGE]: " << message << '\n';
	}

	void Log::EnableOutput() noexcept {
		m_EnableOutput = true;
	}
	void Log::DisableOutput() noexcept {
		m_EnableOutput = false;
	}
	void Log::EnableOutputSuccessfully() noexcept {
		m_EnableOutputSuccessfulls = true;
	}
	void Log::DisableOutputSuccessfully() noexcept {
		m_EnableOutputSuccessfulls = false;
	}
	void Log::EnableOutputWarnings() noexcept {
		m_EnableOutputWarnings = true;
	}
	void Log::DisableOutputWarnings() noexcept {
		m_EnableOutputWarnings = false;
	}
	void Log::EnableOutputErrors() noexcept {
		m_EnableOutputErrors = true;
	}
	void Log::DisableOutputErrors() noexcept {
		m_EnableOutputErrors = false;
	}
	void Log::EnableOutputMessages() noexcept {
		m_EnableOutputMessages = true;
	}
	void Log::DisableOutputMessages() noexcept {
		m_EnableOutputMessages = false;
	}

	void Log::SetMessageStream(std::ostream& messageStream) noexcept {
		m_MessageStream = messageStream;
	}
	void Log::SetErrorStream(std::ostream& errorStream) noexcept {
		m_ErrorStream = errorStream;
	}
	void Log::SetWarningStream(std::ostream& warningStream) noexcept {
		m_WarningStream = warningStream;
	}
}