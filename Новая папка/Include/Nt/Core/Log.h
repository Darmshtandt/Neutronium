#pragma once

namespace Nt {
	class Log {
	public:
		NT_API static void Successfull(const String& Message) noexcept;
		NT_API static void Warning(const String& Message) noexcept;
		NT_API static void Error(const String& Message) noexcept;
		NT_API static void Message(const String& Message) noexcept;


		static void EnableOutput() noexcept {
			m_EnableOutput = true;
		}
		static void DisableOutput() noexcept {
			m_EnableOutput = false;
		}

		static void EnableOutputSuccessfulls() noexcept {
			m_EnableOutputSuccessfulls = true;
		}
		static void DisableOutputSuccessfulls() noexcept {
			m_EnableOutputSuccessfulls = false;
		}

		static void EnableOutputWarnings() noexcept {
			m_EnableOutputWarnings = true;
		}
		static void DisableOutputWarnings() noexcept {
			m_EnableOutputWarnings = false;
		}

		static void EnableOutputErrors() noexcept {
			m_EnableOutputErrors = true;
		}
		static void DisableOutputErrors() noexcept {
			m_EnableOutputErrors = false;
		}

		static void EnableOutputMessages() noexcept {
			m_EnableOutputMessages = true;
		}
		static void DisableOutputMessages() noexcept {
			m_EnableOutputMessages = false;
		}

	private:
		static bool m_EnableOutput;
		static bool m_EnableOutputSuccessfulls;
		static bool m_EnableOutputWarnings;
		static bool m_EnableOutputErrors;
		static bool m_EnableOutputMessages;
	};
}