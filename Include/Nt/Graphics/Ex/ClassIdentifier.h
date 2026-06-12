#pragma once

#include <Nt/Core/Utilities.h>

namespace NtEx {
	using namespace Nt;

	using ClassID = uInt;

	class Identifier {
	protected:
		explicit Identifier(const ClassID& id) noexcept;

	public:
		Identifier() = delete;
		Identifier(const Identifier&) noexcept = default;
		Identifier(Identifier&&) noexcept = default;
		virtual ~Identifier() noexcept = default;

		Identifier& operator = (const Identifier&) noexcept = default;
		Identifier& operator = (Identifier&&) noexcept = default;

		NT_NODISCARD NT_CONSTEXPR ClassID GetID() const noexcept;

	private:
		ClassID m_ID;
	};

	template <class>
	struct Class {
		template <class _U>
		NT_NODISCARD static NT_CONSTEXPR Bool Is() noexcept;
		NT_NODISCARD static NT_CONSTEXPR Bool Is(const ClassID& otherID) noexcept;
		NT_NODISCARD static NT_CONSTEXPR Bool Is(const Identifier& other) noexcept;
		NT_NODISCARD static NT_CONSTEXPR ClassID ID() noexcept;
		NT_NODISCARD static NT_CONSTEXPR std::string_view Name() noexcept;
	};
}

#include <Nt/Graphics/Ex/ClassIdentifier.inl>