// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Timer.h>
#include <Nt/Core/WinMinimal.h>

#include <windows.h>

namespace Nt {
	Timer::Timer() noexcept :
		m_Start(GetTickCount64())
	{ 
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_Frequency));
	}

	void Timer::Restart() noexcept {
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_Ticks));
		m_Start = GetTickCount64();
	}

	_NODISCARD uLLong Timer::SecondsToTicks(const Double& Seconds) const noexcept {
		return uLLong(Double(m_Frequency) * Seconds);
	}

	_NODISCARD uLLong Timer::GetElapsedTimeMs() const noexcept {
		return GetTickCount64() - m_Start;
	}
	_NODISCARD uLLong Timer::GetTicks() noexcept {
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_Ticks));
		return m_Ticks;
	}
}