#pragma once

#include <list>
#include <map>
#include <Nt/Graphics/Resources/IResource.h>

namespace Nt {
	class ResourceManager {
	public:
		using ResourceList = std::vector<std::unique_ptr<IResource>>;
		using ResourcesContainer = std::map<std::type_index, ResourceList>;

	public:
		ResourceManager() = default;

		template <class T, class... Args>
		uInt Add(Args&&... args) {
			return Add(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}

		NT_API uInt Add(std::unique_ptr<IResource>&& pNewResource);
		NT_API void Remove(const std::unique_ptr<IResource>& pResource);
		NT_API void Remove(const std::type_index& type, const uInt& index);
		NT_API void Clear();

		NT_API _NODISCARD Bool IsResourceExists(const std::unique_ptr<IResource>& pResource) const;
		NT_API Bool IsResourceExists(const std::type_index& type, const uInt& index) const;

		template <typename _Ty> requires std::is_base_of_v<IResource, _Ty>
		_NODISCARD _Ty* Get(const uInt& index) {
			if (index == uInt(-1))
				return nullptr;

			const std::type_index type = typeid(_Ty);
			if (!IsResourceExists(type, index))
				return nullptr;

			return dynamic_cast<_Ty*>(m_Resources.at(type)[index].get());
		}

		_NODISCARD
		NT_API static ResourceManager& Instance() noexcept;

		_NODISCARD
		NT_API const ResourcesContainer& GetResources() const noexcept;

	private:
		ResourcesContainer m_Resources;
	};
}