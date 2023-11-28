#include <windows.h>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/Timer.h>

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

	uLLong Timer::SecondsToTicks(const Double& Seconds) const noexcept {
		return uLLong(Double(m_Frequency) * Seconds);
	}

	uLLong Timer::GetElapsedTimeMs() const noexcept {
		return GetTickCount64() - m_Start;
	}
	uLLong Timer::GetTicks() noexcept {
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_Ticks));
		return m_Ticks;
	}
}