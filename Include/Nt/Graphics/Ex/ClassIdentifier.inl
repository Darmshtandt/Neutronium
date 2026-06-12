#pragma once

namespace NtEx {
	using namespace Nt;

	__inline Identifier::Identifier(const ClassID& id) noexcept :
		m_ID(id)
	{
	}

	NT_CONSTEXPR ClassID Identifier::GetID() const noexcept {
		return m_ID;
	}

	template <class _Ty>
	template <class _U>
	NT_CONSTEXPR Bool Class<_Ty>::Is() noexcept {
		return ID() == Class<_U>::ID();
	}

	template <class _Ty>
	NT_CONSTEXPR Bool Class<_Ty>::Is(const ClassID& otherID) noexcept {
		return ID() == otherID;
	}

	template <class _Ty>
	NT_CONSTEXPR Bool Class<_Ty>::Is(const Identifier& other) noexcept {
		return ID() == other.GetID();
	}

	template <class _Ty>
	NT_CONSTEXPR ClassID Class<_Ty>::ID() noexcept {
		uLLong hash = 1469598103934665603ull;
		for (const Char& symbol : __FUNCSIG__) {
			hash ^= static_cast<uInt>(symbol);
			hash *= 1099511628211ull;
		}
		return static_cast<ClassID>(hash);
	}

	template <class _Ty>
	NT_CONSTEXPR std::string_view Class<_Ty>::Name() noexcept {
		return __FUNCSIG__;
	}
}