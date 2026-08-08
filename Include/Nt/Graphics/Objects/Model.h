#pragma once

#include <Nt/Graphics/Objects/IObject.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/ResourceHandle.h>

namespace Nt {
	class Renderer;

	class NT_API Model : public IObject {
	public:
		explicit Model(Mesh* pMesh, Texture* pTexture = nullptr) noexcept;

		Model() noexcept = default;
		Model(const Model& newModel) noexcept = default;
		Model(Model&& newModel) noexcept;
		~Model() noexcept override = default;

		void Render(NotNull<Renderer*> pRenderer) const override;
		void Render(NotNull<Renderer*> pRenderer, const uInt& offset, const uInt& verticesCount) const override;

		Model& operator = (const Model& newModel) noexcept = default;
		Model& operator = (Model&& model) noexcept = default;

		NT_NODISCARD ResourceHandle<Mesh> GetMesh() const noexcept;
		NT_NODISCARD ResourceHandle<Texture> GetTexture() const noexcept;

		void SetMeshByPtr(Mesh* pMesh) noexcept;
		void SetTextureByPtr(Texture* pTexture) noexcept;
		void SetMesh(const uInt& index) noexcept;
		void SetTexture(const uInt& index) noexcept;

	protected:
		ResourceHandle<Mesh> m_pMesh = nullptr;
		ResourceHandle<Texture> m_pTexture = nullptr;
	};
}