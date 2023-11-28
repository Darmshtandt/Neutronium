#pragma once

namespace Nt {
	class Timer {
	public:
		NT_API Timer() noexcept;

		NT_API void Restart() noexcept;

		NT_API uLLong SecondsToTicks(const Double& Seconds) const noexcept;

		NT_API uLLong GetElapsedTimeMs() const noexcept;
		NT_API uLLong GetTicks() noexcept;

	private:
		uLLong m_Start;
		uLLong m_Frequency;
		uLLong m_Ticks;
	};
}