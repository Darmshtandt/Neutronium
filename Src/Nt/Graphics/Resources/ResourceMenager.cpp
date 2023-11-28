#include <windows.h>
#include <GL\GLEW.h>
#include <GL\GL.h>

#include <functional>
#include <fstream>
#include <vector>
#include <list>
#include <map>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Vectors.h>
#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Colors.h>

#include <Nt/Graphics/Geometry.h>
#include <Nt/Graphics/VertexArray.h>

#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/ResourceMenager.h>


namespace Nt {
	ISerialization* IResource::New(const uInt& Type) {
		switch (Type) {
		case TYPE_TEXTURE:
			return new Texture;
		case TYPE_TERRAIN:
			return nullptr;
		case TYPE_MESH:
			return new Mesh;
		case TYPE_SOUND:
			return nullptr;
		}
		return nullptr;
	}
	
	namespace ResourceManager {
		std::map<uInt, std::list<IResource*>> g_Resources = { };

		void IsResourceExists(const uInt& Type, const uInt& Index) {
			if (!g_Resources.contains(Type)) {
				String ErrorMsg = "Attempting to reference an not contained type\nType: ";
				ErrorMsg += Type;
				ErrorMsg += "\nIndex: ";
				ErrorMsg += Index;
				Raise(ErrorMsg);
			}
			if (Index >= g_Resources[Type].size()) {
				String ErrorMsg = "Out of range\nType: ";
				ErrorMsg += Type;
				ErrorMsg += "\nIndex: ";
				ErrorMsg += Index;
				Raise(ErrorMsg);
			}
		}
	
		template <typename _Ty, const uInt Type> requires std::is_base_of_v<IResource, _Ty>
		_Ty* GetResource(const uInt& Index) {
			if (Index == uInt(-1))
				return nullptr;

			IsResourceExists(Type, Index);

			auto it = g_Resources[Type].begin();
			std::advance(it, Index);
			return dynamic_cast<_Ty*>(*it);
		}

		template <typename _Ty, const uInt Type>
		_Ty* GetResource(const uInt& Index) {
			Log::Message(std::string("Resource type: ") + typeid(_Ty).name());
			Raise("Unknown resource type");
		}

		uInt Add(IResource* pResource) {
			if (!pResource)
				Raise("Resource is nullptr");
			if (pResource->TYPE == IResource::TYPE_NULL)
				Raise("Resource error type");
			g_Resources[pResource->TYPE].push_back(pResource);
			return (g_Resources[pResource->TYPE].size() - 1);
		}
		void Remove(const uInt& Type, const uInt& Index) {
			IsResourceExists(Type, Index);

			auto it = g_Resources[Type].begin();
			std::advance(it, Index);

			(*it)->Release();
			delete(*it);
			g_Resources[Type].erase(it);

			if (g_Resources[Type].size() == 0)
				g_Resources.erase(Type);
		}
		void Clear() {
			for (std::pair<uInt, std::list<IResource*>> Resources : g_Resources) {
				for (IResource* pResource : Resources.second) {
					pResource->Release();
					delete(pResource);
				}
				Resources.second.clear();
			}
			g_Resources.clear();
		}

		Texture* GetTexture(const uInt& Index) {
			return GetResource<Texture, IResource::TYPE_TEXTURE>(Index);
		}
		Mesh* GetMesh(const uInt& Index) {
			return GetResource<Mesh, IResource::TYPE_MESH>(Index);
		}
	}
}