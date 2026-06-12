#pragma once

#include <Nt/Graphics/Ex/ClassIdentifier.h>

namespace NtEx {
	using namespace Nt;

	class Object;

	class BaseComponent : public Identifier {
	protected:
		BaseComponent(NotNull<Object*> pOwner, ClassID id) noexcept :
			Identifier(id),
			m_pOwner(pOwner)
		{
		}

	public:
		BaseComponent() = delete;
		virtual ~BaseComponent() noexcept = default;

		NT_NODISCARD Object* OwnerPtr() noexcept {
			return m_pOwner;
		}

	private:
		Object* m_pOwner;
	};
}