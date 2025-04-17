// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>

namespace Nt {
	ISerialization* IResource::New(const uInt& type) {
		if (type == typeid(Image).hash_code())
			return new Image;
		else if (type == typeid(Texture).hash_code())
			return new Texture;
		else if (type == typeid(Mesh).hash_code())
			return new Mesh;

		return nullptr;
	}
}