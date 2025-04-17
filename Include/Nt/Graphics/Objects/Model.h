#pragma once

#include <Nt/Graphics/Objects/IObject.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/ResourceManager.h>

namespace Nt {
	class Renderer;

	class Model : public IObject, public ISerialization {
	public:
		NT_API Model() noexcept;
		NT_API Model(const uInt& meshIndex);
		NT_API Model(const uInt& meshIndex, const uInt& textureIndex);
		NT_API Model(const Mesh& newMesh);
		NT_API Model(const Mesh& newMesh, const Texture& newTexture);
		NT_API Model(const Nt::String& meshFileName);
		NT_API Model(const Nt::String& meshFileName, const Nt::String& textureFileName);
		NT_API Model(const Model& newModel);
		NT_API Model(Model&& newModel);
		NT_API ~Model();

		NT_API void Write(std::ostream& stream) const override;
		NT_API void Read(std::istream& stream) override;
		NT_API constexpr uInt Sizeof() const noexcept override;
		NT_API constexpr uInt ClassType() const noexcept;
		NT_API _NODISCARD static ISerialization* New([[maybe_unused]] const uInt& classType);

		NT_API void LoadTextureFromFile(const Nt::String& fileName);
		NT_API void LoadMeshFromFile(const Nt::String& fileName);

		NT_API void Render(Renderer* pRenderer) const override;
		NT_API void Render(Renderer* pRenderer, const uInt& offset, const uInt& verticesCount) const override;

		NT_API Model& operator = (const Model& newModel);
		NT_API Model& operator = (Model&& model);

		NT_API _NODISCARD uInt GetTextureIndex() const noexcept;
		NT_API _NODISCARD uInt GetMeshIndex() const noexcept;
		NT_API _NODISCARD Texture* GetTexturePtr() const noexcept;
		NT_API _NODISCARD Mesh* GetMeshPtr() const noexcept;

		NT_API void SetTexture(const uInt& textureIndex);
		NT_API void SetTexture(const Texture& newTexture);
		NT_API void SetMesh(const uInt& meshIndex);
		NT_API void SetMesh(const Mesh& newMesh);
		NT_API void SetMesh(const Shape& shape);

	protected:
		Mesh* m_pMesh;
		Texture* m_pTexture;
		uInt m_MeshIndex;
		uInt m_TextureIndex;
	};
}