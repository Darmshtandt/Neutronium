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
#include <Nt/Graphics/Resources/ResourceManager.h>


namespace Nt {
	Texture::Texture() noexcept :
		Image(IResource::TYPE_TEXTURE)
	{
	}
	Texture::Texture(const Texture& newTexture) noexcept :
		Image(newTexture)
	{
		if (newTexture.m_ID != 0) {
			Create(4, m_Size, m_pData);
			GenerateMipmap();

			if (m_pData == nullptr) {
				newTexture.Bind();
				m_pData = new Byte[m_Size.x * m_Size.y * 4];
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pData);
				this->Bind();
			}
		}
	}
	Texture::Texture(const String& FileName) :
		Image(IResource::TYPE_TEXTURE) 
	{
		LoadFromFile(FileName);
	}
	Texture::~Texture() {
		Release();
	}

	void Texture::FlipVerticaly() {
		if (IsLoaded()) {
			const uInt* ThisDataPtr = reinterpret_cast<const uInt*>(m_pData);
			uInt* pFlipData = new uInt[m_Size.x * m_Size.y];

			for (uInt y = 0; y < m_Size.y; ++y) {
				const uInt FlipDataOffset = y * m_Size.x;
				const uInt ThisDataOffset = (m_Size.y - y - 1) * m_Size.x;
				memcpy(pFlipData + FlipDataOffset, ThisDataPtr + ThisDataOffset, sizeof(uInt) * m_Size.x);
			}

			if (m_pData != nullptr)
				delete(m_pData);
			m_pData = pFlipData;

			Delete();
			Create(4, m_Size, m_pData);
			GenerateMipmap();
		}
	}
	void Texture::Rotate_90_Degrees(const Bool& toRight) {
		if (IsLoaded()) {
			uInt* pFlipData = new uInt[m_Size.x * m_Size.y];

			for (uInt y = 0; y < m_Size.y; ++y) {
				for (uInt x = 0; x < m_Size.x; ++x) {
					if (toRight)
						pFlipData[y * m_Size.x + x] = ((uInt*)m_pData)[x * m_Size.x + y];
					else
						pFlipData[(y + 1) * m_Size.x - x - 1] = ((uInt*)m_pData)[x * m_Size.x + y];
				}
			}

			if (m_pData != nullptr)
				delete(m_pData);
			m_pData = pFlipData;

			Delete();
			Create(4, m_Size, m_pData);
			GenerateMipmap();
		}
	}

	void Texture::SetMinFiler(const MinFilter& filer) const noexcept {
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filer);
	}
	void Texture::SetMagFiler(const MagFilter& filer) const noexcept {
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filer);
	}
	void Texture::SetMinLOD(const Int& value) const noexcept {
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, value);
	}
	void Texture::SetMaxLOD(const Int& value) const noexcept {
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, value);
	}
	void Texture::SetWrapS(const Wrap& wrap) const noexcept {
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	}
	void Texture::SetWrapT(const Wrap& wrap) const noexcept {
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	}

	void Texture::Bind() const {
		if (m_ID == 0)
			Raise("Texture is not created");
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	uInt Texture::GetID() const noexcept {
		return m_ID;
	}

	Bool Texture::IsCreated() const noexcept {
		return (m_ID != 0);
	}

	void Texture::Create(const uInt& channelsCount, const uInt2D& size, const void* pData) {
		if (m_Size != size)
			m_Size = size;

		if (m_ID != 0)
			glDeleteTextures(1, &m_ID);

		glGenTextures(1, &m_ID);
		if (m_ID == 0)
			_ThrowError("Failed to create texture");

		switch (channelsCount) {
		case 1:
			m_ColorComponent = GL_R;
			break;
		case 2:
			m_ColorComponent = GL_RG;
			break;
		case 3:
			m_ColorComponent = GL_RGB;
			break;
		case 4:
			m_ColorComponent = GL_RGBA;
			break;
		default:
			Raise("Incorrect number of channels");
		}

		glBindTexture(GL_TEXTURE_2D, m_ID);
		if (pData != nullptr)
			gluBuild2DMipmaps(GL_TEXTURE_2D, m_ColorComponent, size.x, size.y, m_ColorComponent, GL_UNSIGNED_BYTE, pData);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, m_ColorComponent, size.x, size.y, 0, m_ColorComponent, GL_UNSIGNED_BYTE, nullptr);

		SetMinFiler(MIN_NEAREST);
		SetMagFiler(MAG_NEAREST);
		SetWrapS(WRAP_REPEAT);
		SetWrapT(WRAP_REPEAT);
	}

	void Texture::Delete() {
		if (m_ID != 0) {
			glDeleteTextures(1, &m_ID);
			m_ID = 0;
		}
	}

	void Texture::GenerateMipmap() const {
		if (m_ID != 0)
			glGenerateTextureMipmap(m_ID);
	}

	void Texture::_LoadFromFile() {
		Image::_LoadFromFile();

		Create(4, m_Size, m_pData);
		GenerateMipmap();
	}

	void Texture::_Release() {
		Image::_Release();
		Delete();
	}
}