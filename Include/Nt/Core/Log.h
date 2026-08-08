#pragma once

#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>

#pragma warning(disable : 4251)

namespace Nt {
	class NT_API Log {
	public:
		Log() noexcept;

		NT_NODISCARD static Log& Instance() noexcept;

		void Successful(const String& message) noexcept;
		void Warning(const String& message) noexcept;
		void Error(const String& message) noexcept;
		void Message(const String& message) noexcept;

		void EnableOutput() noexcept;
		void DisableOutput() noexcept;
		void EnableOutputSuccessfully() noexcept;
		void DisableOutputSuccessfully() noexcept;
		void EnableOutputWarnings() noexcept;
		void DisableOutputWarnings() noexcept;
		void EnableOutputErrors() noexcept;
		void DisableOutputErrors() noexcept;
		void EnableOutputMessages() noexcept;
		void DisableOutputMessages() noexcept;

		void SetMessageStream(std::ostream& messageStream) noexcept;
		void SetErrorStream(std::ostream& errorStream) noexcept;
		void SetWarningStream(std::ostream& warningStream) noexcept;

	private:
		std::reference_wrapper<std::ostream> m_MessageStream;
		std::reference_wrapper<std::ostream> m_ErrorStream;
		std::reference_wrapper<std::ostream> m_WarningStream;

		Bool m_EnableOutput = true;
		Bool m_EnableOutputSuccessfulls = true;
		Bool m_EnableOutputWarnings = true;
		Bool m_EnableOutputErrors = true;
		Bool m_EnableOutputMessages = true;
	};
}

#pragma warning(default : 4251)