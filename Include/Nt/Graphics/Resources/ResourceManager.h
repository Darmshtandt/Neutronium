#pragma once

#include <Nt/Graphics/Resources/IResource.h>
#include <map>

namespace Nt {
	class ResourceManager {
	public:
		using ResourceArray = std::vector<std::unique_ptr<IResource>>;
		using ResourcesContainer = std::map<std::type_index, ResourceArray>;

	public:
		ResourceManager() = default;

		template <class _Ty, class... _Args>
		uInt Add(_Args&&... args) {
			return Add(std::move(std::make_unique<_Ty>(std::forward<_Args>(args)...)));
		}
		template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<IResource, _Ty>>>
		void Remove(const uInt& index) {
			Remove(typeid(_Ty), index);
		}

		NT_API uInt Add(std::unique_ptr<IResource>&& pNewResource);
		NT_API void Remove(const std::unique_ptr<IResource>& pResource);
		NT_API void Remove(const std::type_index& type, const uInt& index);
		NT_API void Clear();

		NT_API void RequireExist(const std::unique_ptr<IResource>& pResource) const;
		NT_API void RequireExist(const std::type_index& type, const uInt& index) const;

		template <typename _Ty> requires std::is_base_of_v<IResource, _Ty>
		NT_NODISCARD _Ty* Get(const uInt& index) {
			if (index == uInt(-1))
				return nullptr;

			const std::type_index type = typeid(_Ty);
			RequireExist(type, index);

			return dynamic_cast<_Ty*>(m_Resources.at(type)[index].get());
		}

		NT_NODISCARD NT_API Bool IsValid(IResource* pResource) const noexcept;

		NT_NODISCARD NT_API static ResourceManager& Instance() noexcept;
		NT_NODISCARD NT_API const ResourcesContainer& GetResources() const noexcept;

	private:
		ResourcesContainer m_Resources;
	};
}