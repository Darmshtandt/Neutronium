#pragma once

namespace Nt::ResourceManager {
	NT_API extern uInt Add(IResource* pResource);
	NT_API extern void Remove(const uInt& Type, const uInt& Index);
	NT_API extern void Clear();

	NT_API extern Texture* GetTexture(const uInt& Index);
	NT_API extern Mesh* GetMesh(const uInt& Index);
}