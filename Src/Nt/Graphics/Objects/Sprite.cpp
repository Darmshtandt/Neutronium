// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <windows.h>
#include <shobjidl.h>
#include <GL/GLEW.h>
#include <GL/GL.h>

#include <fstream>

// ============================================================================
//	Neutronium
// ----------------------------------------------------------------------------
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Math/Matrix3x3.h>
#include <Nt/Core/Math/Matrix4x4.h>

#include <Nt/Core/Colors.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Core/Timer.h>

#include <Nt/Graphics/Geometry/Primitives.h>
#include <Nt/Graphics/Buffer.h>
#include <Nt/Graphics/VertexArray.h>
#include <Nt/Graphics/Objects/IObject.h>
#include <Nt/Graphics/Shader.h>

#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/ResourceManager.h>

#include <Nt/Graphics/System/Menu.h>
#include <Nt/Graphics/System/HandleWindow.h>

#include <Nt/Graphics/Renderer.h>
#include <Nt/Graphics/Objects/Sprite.h>

namespace Nt {
	Sprite::Sprite() :
		m_pMesh(std::make_unique<Mesh>(Primitive::Quad({ 1.f, 1.f }, Colors::White))),
		m_pTexture(nullptr),
		m_TextureIndex(0)
	{
		IObject::SetSize({ 1.f, 1.f });
	}

	Sprite::Sprite(const uInt& textureIndex, const Float2D& size, const Float4D& color) :
		m_pMesh(std::make_unique<Mesh>(Primitive::Quad(size, color)))
	{
		IObject::SetSize(Float3D(size, 0.f));
		IObject::SetColor(color);
		SetTexture(textureIndex);
	}
	Sprite::Sprite(const Texture& newTexture, const Float2D& size, const Float4D& color) :
		m_pMesh(std::make_unique<Mesh>(Primitive::Quad(size, color)))
	{
		IObject::SetSize(Float3D(size, 0.f));
		IObject::SetColor(color);
		SetTexture(newTexture);
	}
	Sprite::Sprite(const Sprite& newSprite) :
		IObject(newSprite),
		m_TextureRect(newSprite.m_TextureRect),
		m_pMesh(new Mesh(*newSprite.m_pMesh)),
		m_TextureIndex(newSprite.m_TextureIndex)
	{
		if (newSprite.m_TextureIndex == -1)
			m_pTexture = new Texture(*newSprite.m_pTexture);
		else
			m_pTexture = newSprite.m_pTexture;
	}
	Sprite::~Sprite() {
		if (m_TextureIndex == -1)
			SAFE_DELETE(&m_pTexture);
	}

	void Sprite::Render(NotNull<Renderer*> pRenderer) const {
		if (!IsVisible())
			return;

		if (m_pTexture != nullptr)
			m_pTexture->Bind();
		else
			pRenderer->UnbindTexture();

		const Float4D Color = pRenderer->GetColor();
		pRenderer->SetColor(GetColor());
		pRenderer->MatrixWorldPush();
		pRenderer->Transform(GetPosition(), GetOrigin(), GetAngle(), GetAngleOrigin());
		pRenderer->Render(m_pMesh.get());
		pRenderer->MatrixWorldPop();
		pRenderer->SetColor(Color);
	}
	void Sprite::Render([[maybe_unused]] NotNull<Renderer*> pRenderer, [[maybe_unused]] const uInt& offset, [[maybe_unused]] const uInt& verticesCount) const
	{
	}

	Sprite& Sprite::operator = (const Sprite& newSprite) {
		if (this == &newSprite)
			return *this;

		IObject::operator=(newSprite);

		m_TextureRect = newSprite.m_TextureRect;
		m_pMesh = std::make_unique<Mesh>(*newSprite.m_pMesh);

		m_TextureIndex = newSprite.m_TextureIndex;
		if (newSprite.m_TextureIndex == uInt(-1))
			m_pTexture = new Texture(*newSprite.m_pTexture);
		else
			m_pTexture = newSprite.m_pTexture;

		return *this;
	}

	Texture* Sprite::GetTexturePtr() const noexcept {
		return m_pTexture;
	}

	const std::unique_ptr<Mesh>& Sprite::GetMeshPtr() const noexcept {
		return m_pMesh;
	}
	FloatRect Sprite::GetTextureRect() const noexcept {
		FloatRect TextureRect = m_TextureRect;
		TextureRect.LeftTop *= Float2D(m_pTexture->GetSize());
		TextureRect.RightBottom *= Float2D(m_pTexture->GetSize());
		return TextureRect;
	}
	uInt Sprite::GetTextureIndex() const noexcept {
		return m_TextureIndex;
	}

	void Sprite::SetTextureRect(const FloatRect& TextureRect) {
		m_TextureRect = TextureRect;
		if (!(m_TextureRect.LeftTop <= 1.f && m_TextureRect.RightBottom <= 1.f)) {
			m_TextureRect.LeftTop /= Float2D(m_pTexture->GetSize());
			m_TextureRect.RightBottom /= Float2D(m_pTexture->GetSize());
		}
		_UpdateTexCoords();
	}
	void Sprite::SetTexture(const Texture& NewTexture) {
		if (m_TextureIndex == uInt(-1))
			SAFE_DELETE(&m_pTexture);

		m_TextureIndex = uInt(-1);
		m_pTexture = new Texture(NewTexture);
	}
	void Sprite::SetTexture(const uInt& TextureIndex) {
		if (m_TextureIndex == uInt(-1))
			SAFE_DELETE(&m_pTexture);

		m_TextureIndex = TextureIndex;
		m_pTexture = ResourceManager::Instance().Get<Texture>(m_TextureIndex);
	}

	void Sprite::SetSize(const Float3D& Size) {
		m_pMesh->SetScale(Size);
		IObject::SetSize(Size);
	}

	void Sprite::_UpdateTexCoords() const {
		if (m_pMesh) {
			Vertices_t Vertices = m_pMesh->GetVertices();
			Vertices[0].TexCoord = Float3D(m_TextureRect.Right, m_TextureRect.Top, 0.f);
			Vertices[1].TexCoord = Float3D(m_TextureRect.Left, m_TextureRect.Top, 0.f);
			Vertices[2].TexCoord = Float3D(m_TextureRect.Left, m_TextureRect.Bottom, 0.f);
			Vertices[3].TexCoord = Float3D(m_TextureRect.Right, m_TextureRect.Bottom, 0.f);
			m_pMesh->UpdateVertices(Vertices);
		}
	}
}