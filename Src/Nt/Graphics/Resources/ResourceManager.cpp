// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <windows.h>
#include <GL\GLEW.h>
#include <GL\GL.h>

#include <functional>
#include <fstream>
#include <vector>
#include <list>
#include <map>

#include <Nt/Core/Utilities.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Colors.h>

#include <Nt/Graphics/Buffer.h>
#include <Nt/Graphics/VertexArray.h>

#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/ResourceManager.h>

namespace Nt {
	Bool ResourceManager::IsResourceExists(const std::unique_ptr<IResource>& pResource) const {
		RequireNotNull(pResource.get());

		const std::type_index& type = pResource->GetType();

		if (!m_Resources.contains(type)) {
			String errorMsg = "Attempting to reference an not contained type\nType: ";
			errorMsg += type.name();

			Raise(errorMsg);
		}

		const ResourceList& resourseList = m_Resources.at(type);
		const ResourceList::const_iterator iterator =
			std::find(resourseList.cbegin(), resourseList.cend(), pResource);

		if (iterator == resourseList.cend())
			Raise(std::string("Not found.\nType: ") + type.name());

		return true;
	}

	Bool ResourceManager::IsResourceExists(const std::type_index& type, const uInt& index) const {
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

		return true;
	}

	_NODISCARD
	ResourceManager& ResourceManager::Instance() noexcept {
		static ResourceManager manager;
		return manager;
	}

	_NODISCARD
	const ResourceManager::ResourcesContainer& ResourceManager::GetResources() const noexcept {
		return m_Resources;
	}

	uInt ResourceManager::Add(std::unique_ptr<IResource>&& pNewResource) {
		RequireNotNull(pNewResource.get());

		ResourceList& resourceList = m_Resources[pNewResource->GetType()];

		const ResourceList::const_iterator resourceIterator =
			std::find_if(resourceList.begin(), resourceList.end(), [&](const std::unique_ptr<IResource>& pResource) {
				return (pResource == pNewResource) || (pResource->GetFilePath() == pNewResource->GetFilePath());
				});

		if (resourceIterator != resourceList.end()) {
			Log::Warning("The resource has already been added");
			return uInt(-1);
		}

		resourceList.push_back(std::move(pNewResource));

		return (resourceList.size() - 1);
	}
	void ResourceManager::Remove(const std::unique_ptr<IResource>& pResource) {
		const std::type_index type = pResource->GetType();

		if (!IsResourceExists(pResource)) {
			String errorMsgStr = "Not found.";
			errorMsgStr += "\n\nFile path: ";
			errorMsgStr += pResource->GetFilePath();
			errorMsgStr += "\nResource type: ";
			errorMsgStr += type.name();

			Raise(errorMsgStr);
		}

		const ResourceList::const_iterator iterator =
			std::find(m_Resources[type].cbegin(), m_Resources[type].cend(), pResource);

		m_Resources[type].erase(iterator);

		if (m_Resources[type].size() == 0)
			m_Resources.erase(type);
	}
	void ResourceManager::Remove(const std::type_index& type, const uInt& index) {
		IsResourceExists(type, index);

		ResourceList::const_iterator iterator = m_Resources[type].begin();
		std::advance(iterator, index);

		m_Resources[type].erase(iterator);

		if (m_Resources[type].size() == 0)
			m_Resources.erase(type);
	}
	void ResourceManager::Clear() {
		m_Resources.clear();
	}
}