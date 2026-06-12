// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <GL/GLEW.h>
#include <GL/GL.h>

#include <Nt/Graphics/Objects/Model.h>
#include <Nt/Graphics/Renderer.h>

namespace Nt {
	Model::Model(Mesh* pMesh, Texture* pTexture) noexcept :
		m_pMesh(pMesh),
		m_pTexture(pTexture)
	{
	}

	Model::Model(Model&& newModel) noexcept :
		IObject(std::move(static_cast<IObject&&>(newModel))),
		m_pMesh(newModel.m_pMesh),
		m_pTexture(newModel.m_pTexture)
	{
		newModel.m_pMesh.Reset();
		newModel.m_pTexture.Reset();
	}

	void Model::Render(NotNull<Renderer*> pRenderer) const {
		if (!IsVisible() || !m_pMesh.IsValid())
			return;

		Model::Render(pRenderer, 0, m_pMesh.Get()->GetVerticesCount());
	}
	void Model::Render(NotNull<Renderer*> pRenderer, const uInt& offset, const uInt& verticesCount) const {
		if (!IsVisible() || !m_pMesh.IsValid())
			return;

		if (m_pTexture.IsValid())
			pRenderer->BindTexture(m_pTexture.Get());

		const Float4D color = pRenderer->GetColor();
		pRenderer->SetColor(GetColor());
		pRenderer->MatrixWorldPush();
		pRenderer->Transform(GetPosition(), GetOrigin(), GetAngle(), GetAngleOrigin());
		pRenderer->Scale(GetSize());
		pRenderer->Render(m_pMesh.Get(), offset, verticesCount);
		pRenderer->MatrixWorldPop();
		pRenderer->SetColor(color);
	}

	ResourceHandle<Mesh> Model::GetMesh() const noexcept {
		return m_pMesh;
	}
	ResourceHandle<Texture> Model::GetTexture() const noexcept {
		return m_pTexture;
	}

	void Model::SetMeshByPtr(Mesh* pMesh) noexcept {
		m_pMesh = pMesh;
	}

	void Model::SetTextureByPtr(Texture* pTexture) noexcept {
		m_pTexture = pTexture;
	}

	void Model::SetMesh(const uInt& index) noexcept {
		m_pMesh.Set(index);
	}

	void Model::SetTexture(const uInt& index) noexcept {
		m_pTexture.Set(index);
	}
}
