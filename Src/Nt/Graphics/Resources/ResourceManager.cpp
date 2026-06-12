// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Log.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Graphics/Resources/ResourceManager.h>

namespace Nt {
	namespace Details {
		uInt g_Generation = 0;
	}

	void ResourceManager::RequireExist(const std::unique_ptr<IResource>& pResource) const {
		RequireNotNull(pResource);

		const std::type_index& type = pResource->GetType();

		if (!m_Resources.contains(type)) {
			String errorMsg = "Attempting to reference an not contained type\nType: ";
			errorMsg += type.name();

			Raise(errorMsg);
		}

		const ResourceArray& resourseList = m_Resources.at(type);
		const ResourceArray::const_iterator iterator =
			std::find(resourseList.cbegin(), resourseList.cend(), pResource);

		if (iterator == resourseList.cend())
			Raise(std::string("Not found.\nType: ") + type.name());
	}

	void ResourceManager::RequireExist(const std::type_index& type, const uInt& index) const {
		if (!m_Resources.contains(type)) {
			String errorMsg = "Attempting to reference an not contained type\nType: ";
			errorMsg += type.name();
			errorMsg += "\nIndex: ";
			errorMsg += index;

			Raise(errorMsg);
		}

		if (index >= m_Resources.at(type).size()) {
			String errorMsg = "Out of range\nType: ";
			errorMsg += type.name();
			errorMsg += "\nIndex: ";
			errorMsg += index;

			Raise(errorMsg);
		}
	}

	Bool ResourceManager::IsValid(IResource* pResource) const noexcept {
		if (pResource == nullptr)
			return false;

		const ResourceArray& resourceList = m_Resources.at(pResource->GetType());
		if (pResource->m_Index >= resourceList.size())
			return false;

		IResource* pExistingResource = resourceList[pResource->m_Index].get();
		return pResource->m_Generation == pExistingResource->m_Generation;
	}

	ResourceManager& ResourceManager::Instance() noexcept {
		static ResourceManager manager;
		return manager;
	}

	const ResourceManager::ResourcesContainer& ResourceManager::GetResources() const noexcept {
		return m_Resources;
	}

	uInt ResourceManager::Add(std::unique_ptr<IResource>&& pNewResource) {
		RequireNotNull(pNewResource.get());

		ResourceArray& resourceList = m_Resources[pNewResource->GetType()];

		const ResourceArray::const_iterator resourceIterator =
			std::find_if(resourceList.begin(), resourceList.end(), [&](const std::unique_ptr<IResource>& pResource) {
				if (pResource == pNewResource)
					return true;
				if (pResource->GetFilePath().empty())
					return false;
				return (pResource->GetFilePath() == pNewResource->GetFilePath());
				});

		if (resourceIterator != resourceList.end()) {
			Log::Instance().Warning("The resource has already been added");
			return (*resourceIterator)->m_Index;
		}

		const uInt index = resourceList.size();
		pNewResource->m_Generation = ++Details::g_Generation;
		pNewResource->m_Index = index;
		resourceList.emplace_back(std::move(pNewResource));

		return index;
	}
	void ResourceManager::Remove(const std::unique_ptr<IResource>& pResource) {
		const std::type_index type = pResource->GetType();
		RequireExist(pResource);

		const ResourceArray::const_iterator iterator =
			std::find(m_Resources[type].cbegin(), m_Resources[type].cend(), pResource);

		m_Resources[type].erase(iterator);

		if (m_Resources[type].size() == 0)
			m_Resources.erase(type);
	}
	void ResourceManager::Remove(const std::type_index& type, const uInt& index) {
		RequireExist(type, index);

		ResourceArray::const_iterator iterator = m_Resources[type].begin();
		std::advance(iterator, index);

		m_Resources[type].erase(iterator);

		if (m_Resources[type].size() == 0)
			m_Resources.erase(type);
	}
	void ResourceManager::Clear() {
		m_Resources.clear();
	}
}
