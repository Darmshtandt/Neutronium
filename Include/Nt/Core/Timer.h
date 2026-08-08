#pragma once

#include <Nt/Core/Utilities.h>

namespace Nt {
	class NT_API Timer {
	public:
		Timer() noexcept;

		void Restart() noexcept;

		_NODISCARD uLLong SecondsToTicks(const Double& Seconds) const noexcept;

		_NODISCARD uLLong GetElapsedTimeMs() const noexcept;
		_NODISCARD uLLong GetTicks() noexcept;

	private:
		uLLong m_Start = 0ull;
		uLLong m_Frequency = 0ull;
		uLLong m_Ticks = 0ull;
	};
}