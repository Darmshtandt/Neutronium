#pragma once

#include <Nt/Graphics/Resources/ResourceManager.h>
#include <Nt/Core/Utilities.h>

namespace Nt {
	template <class _Ty, Requires(std::is_base_of_v<IResource, _Ty>)>
	class NT_API ResourceHandle final {
	public:
		ResourceHandle(_Ty* pResource) noexcept :
			m_pResource(pResource)
		{
		}

		ResourceHandle() noexcept = default;
		ResourceHandle(const ResourceHandle&) noexcept = default;
		ResourceHandle(ResourceHandle&&) noexcept = default;
		~ResourceHandle() noexcept = default;

		ResourceHandle& operator = (const ResourceHandle&) noexcept = default;
		ResourceHandle& operator = (ResourceHandle&&) noexcept = default;

		void Reset() {
			m_pResource = nullptr;
		}

		NT_NODISCARD _Ty* Get() noexcept {
			return m_pResource;
		}
		NT_NODISCARD const _Ty* Get() const noexcept {
			return m_pResource;
		}

		const _Ty* SetByPointer(_Ty* pResource) noexcept {
			m_pResource = pResource;
			return m_pResource;
		}
		const _Ty* Set(const uInt& index) noexcept {
			m_pResource = ResourceManager::Instance().Get<_Ty>(index);
			return m_pResource;
		}

		NT_NODISCARD Bool IsValid() const noexcept {
			if (m_pResource != nullptr)
				return true;
			return ResourceManager::Instance().IsValid(m_pResource);
		}

	private:
		_Ty* m_pResource = nullptr;
	};
}