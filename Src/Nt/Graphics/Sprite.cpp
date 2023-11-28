#include <windows.h>
#include <GL/GLEW.h>
#include <GL/GL.h>

#include <fstream>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Vectors.h>
#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Math/Matrix3x3.h>
#include <Nt/Core/Math/Matrix4x4.h>

#include <Nt/Core/Colors.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Core/Timer.h>

#include <Nt/Graphics/Geometry.h>
#include <Nt/Graphics/VertexArray.h>
#include <Nt/Graphics/IObject.h>
#include <Nt/Graphics/Shader.h>

#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/ResourceMenager.h>

#include <Nt/Graphics/Renderer.h>
#include <Nt/Graphics/Sprite.h>

namespace Nt {
	Sprite::Sprite() :
		m_pMesh(std::make_unique<Mesh>(Geometry::Quad({ 1.f, 1.f }, Colors::White))),
		m_pTexture(nullptr),
		m_TextureIndex(0)
	{ 
	}

	Sprite::Sprite(const uInt& TextureIndex, const Float2D& Size, const Float4D& Color) :
		m_pMesh(std::make_unique<Mesh>(Geometry::Quad(Size, Color)))
	{
		IObject::SetSize(Size);
		IObject::SetColor(Color);
		SetTexture(TextureIndex);
	}
	Sprite::Sprite(const Texture& NewTexture, const Float2D& Size, const Float4D& Color) :
		m_pMesh(std::make_unique<Mesh>(Geometry::Quad(Size, Color)))
	{
		IObject::SetSize(Size);
		IObject::SetColor(Color);
		SetTexture(NewTexture);
	}
	Sprite::Sprite(const Sprite& NewSprite) :
		IObject(NewSprite),
		m_TextureRect(NewSprite.m_TextureRect),
		m_pMesh(new Mesh(*NewSprite.m_pMesh)),
		m_TextureIndex(NewSprite.m_TextureIndex)
	{
		if (NewSprite.m_TextureIndex == -1)
			m_pTexture = new Texture(*NewSprite.m_pTexture);
		else
			m_pTexture = NewSprite.m_pTexture;
	}
	Sprite::~Sprite() {
		if (m_TextureIndex == -1)
			SAFE_DELETE(&m_pTexture);
	}

	void Sprite::Render(Renderer* pRenderer) const {
		if (!pRenderer)
			Raise("pRenderer is nullptr");

		if (!IsVisible())
			return;

		_CheckTexture();
		m_pTexture->Set();

		const Float4D Color = pRenderer->GetColor();
		pRenderer->SetColor(GetColor());
		pRenderer->MatrixWorldPush();
		pRenderer->Translate(GetPosition());
		pRenderer->Rotate(GetAngleOrigin());
		pRenderer->Translate(GetOrigin());
		pRenderer->Rotate(GetAngle());
		pRenderer->Render(m_pMesh.get());
		pRenderer->MatrixWorldPop();
		pRenderer->SetColor(Color);
	}


	const std::unique_ptr<Mesh>& Sprite::GetMeshPtr() const noexcept {
		return m_pMesh;
	}
	FloatRect Sprite::GetTextureRect() const noexcept {
		FloatRect TextureRect = m_TextureRect;
		TextureRect.LeftTop *= m_pTexture->GetSize();
		TextureRect.RightBottom *= m_pTexture->GetSize();
		return TextureRect;
	}
	uInt Sprite::GetTextureIndex() const noexcept {
		return m_TextureIndex;
	}

	void Sprite::SetTextureRect(const FloatRect& TextureRect) {
		_CheckTexture();
		m_TextureRect = TextureRect;
		if (!(m_TextureRect.LeftTop <= 1.f && m_TextureRect.RightBottom <= 1.f)) {
			m_TextureRect.LeftTop /= m_pTexture->GetSize();
			m_TextureRect.RightBottom /= m_pTexture->GetSize();
		}
		_UpdateTexCoords();
	}
	void Sprite::SetTexture(const Texture& NewTexture) {
		if (m_TextureIndex == -1)
			SAFE_DELETE(&m_pTexture);

		m_TextureIndex = -1;
		m_pTexture = new Texture(NewTexture);
	}
	void Sprite::SetTexture(const uInt& TextureIndex) {
		if (m_TextureIndex == -1)
			SAFE_DELETE(&m_pTexture);
		m_TextureIndex = TextureIndex;
		m_pTexture = ResourceManager::GetTexture(m_TextureIndex);
	}

	void Sprite::SetSize(const Float3D& Size) {
		IObject::SetSize(Size);
		m_pMesh->SetScale(Size);
	}

	void Sprite::_CheckTexture() const {
		if (!m_pTexture)
			Raise("No texture selected");
		m_pTexture->CheckIfItsLoaded();
	}
	void Sprite::_UpdateTexCoords() const {
		if (m_pMesh) {
			Vertices_t Vertices = m_pMesh->GetVertices();
			Vertices[0].TexCoord = Float2D(m_TextureRect.Right, m_TextureRect.Bottom);
			Vertices[1].TexCoord = Float2D(m_TextureRect.Right, m_TextureRect.Top);
			Vertices[2].TexCoord = Float2D(m_TextureRect.Left, m_TextureRect.Top);
			Vertices[3].TexCoord = Float2D(m_TextureRect.Left, m_TextureRect.Bottom);
			m_pMesh->SetVertices(Vertices);
		}
	}
}