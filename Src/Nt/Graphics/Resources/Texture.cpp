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
	Texture::Texture() noexcept :
		Image(IResource::TYPE_TEXTURE)
	{
	}
	Texture::Texture(const Texture& NewTexture) noexcept :
		Image(NewTexture)
	{
		_Create();
	}
	Texture::Texture(const String& FileName) :
		Image(IResource::TYPE_TEXTURE) {
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
			if (m_pData)
				delete(m_pData);
			m_pData = pFlipData;

			glDeleteTextures(1, &m_ID);
			_Create();
		}
	}
	void Texture::Rotate_90_Degrees(const Bool& toRight) {
		if (IsLoaded()) {
			const uInt* ThisDataPtr = reinterpret_cast<const uInt*>(m_pData);
			uInt* pFlipData = new uInt[m_Size.x * m_Size.y];

			for (uInt y = 0; y < m_Size.y; ++y) {
				for (uInt x = 0; x < m_Size.x; ++x) {
					if (toRight)
						pFlipData[y * m_Size.x + x] = ((uInt*)m_pData)[x * m_Size.x + y];
					else
						pFlipData[(y + 1) * m_Size.x - x - 1] = ((uInt*)m_pData)[x * m_Size.x + y];
				}
			}

			if (m_pData)
				delete(m_pData);
			m_pData = pFlipData;

			glDeleteTextures(1, &m_ID);
			_Create();
		}
	}

	void Texture::SetMinFiler(const MinFilter& filer) const noexcept {
		Set();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filer);
	}
	void Texture::SetMagFiler(const MagFilter& filer) const noexcept {
		Set();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filer);
	}
	void Texture::SetMinLOD(const Int& value) const noexcept {
		Set();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, value);
	}
	void Texture::SetMaxLOD(const Int& value) const noexcept {
		Set();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, value);
	}
	void Texture::SetWrapS(const Wrap& wrap) const noexcept {
		Set();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	}
	void Texture::SetWrapT(const Wrap& wrap) const noexcept {
		Set();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	}

	void Texture::Set() const {
		if (m_ID == 0)
			Raise("Texture is not created");
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	uInt Texture::GetID() const noexcept {
		return m_ID;
	}

	void Texture::_Create() {
		glGenTextures(1, &m_ID);
		if (m_ID == 0)
			_ThrowError("Failed to create texture");

		glBindTexture(GL_TEXTURE_2D, m_ID);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_Size.x, m_Size.y, GL_RGBA, GL_UNSIGNED_BYTE, m_pData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateTextureMipmap(m_ID);
	}

	void Texture::_LoadFromFile() {
		Image::_LoadFromFile();
		_Create();
	}

	void Texture::_Release() {
		Image::_Release();
		if (m_ID)
			glDeleteTextures(1, &m_ID);
	}
}