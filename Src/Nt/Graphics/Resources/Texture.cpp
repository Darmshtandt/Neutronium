// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <windows.h>
#include <GL\GLEW.h>
#include <GL\GL.h>

#include <functional>
#include <fstream>
#include <vector>
#include <list>
#include <map>

#include <Nt/Core/Utilities.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Colors.h>

#include <Nt/Graphics/Buffer.h>
#include <Nt/Graphics/VertexArray.h>

#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/ResourceManager.h>


namespace Nt {
	Texture::Texture(const String& FileName) {
		Texture::LoadFromFile(FileName);
	}
	Texture::Texture(const Texture& newTexture) : 
		m_Image(newTexture.m_Image),
		m_Parameters()
	{
		if (newTexture.m_ID != 0) {
			Create();
			SetParameters(newTexture.m_Parameters);
		}
	}
	Texture::Texture(Texture&& otherTexture) noexcept :
		m_Image(std::move(otherTexture.m_Image)),
		m_Parameters(otherTexture.m_Parameters),
		m_ID(otherTexture.m_ID),
		m_ColorComponent(otherTexture.m_ColorComponent)
	{
		m_ID = 0;
		m_ColorComponent = 0;
	}
	Texture::~Texture() {
		Texture::Release();
	}

	void Texture::Read(std::istream& Stream) {
		m_Image.Read(Stream);
	}

	void Texture::FlipVerticaly() {
		if (GetData() != nullptr) {
			const uInt* thisDataPtr = reinterpret_cast<const uInt*>(GetData().get());
			uInt* pFlipData = new uInt[GetSize().x * GetSize().y];

			for (uInt y = 0; y < GetSize().y; ++y) {
				const uInt flipDataOffset = y * GetSize().x;
				const uInt thisDataOffset = (GetSize().y - y - 1) * GetSize().x;
				memcpy(pFlipData + flipDataOffset, thisDataPtr + thisDataOffset, sizeof(uInt) * GetSize().x);
			}

			std::unique_ptr<Byte[]> uniqueData;
			uniqueData.reset(reinterpret_cast<Byte*>(pFlipData));
			m_Image.SetData(std::move(uniqueData), GetSize(), GetChannelCount());

			Delete();
			Create();
		}
	}
	void Texture::Rotate_90_Degrees(const Bool& toRight) {
		if (GetData() != nullptr) {
			const uInt* thisDataPtr = reinterpret_cast<const uInt*>(GetData().get());
			uInt* pFlipData = new uInt[GetSize().x * GetSize().y];

			for (uInt y = 0; y < GetSize().y; ++y) {
				for (uInt x = 0; x < GetSize().x; ++x) {
					if (toRight)
						pFlipData[y * GetSize().x + x] = thisDataPtr[x * GetSize().x + y];
					else
						pFlipData[(y + 1) * GetSize().x - x - 1] = thisDataPtr[x * GetSize().x + y];
				}
			}

			std::unique_ptr<Byte[]> uniqueData;
			uniqueData.reset(reinterpret_cast<Byte*>(pFlipData));
			m_Image.SetData(std::move(uniqueData), GetSize(), GetChannelCount());

			Delete();
			Create();
		}
	}

	void Texture::SetParameters(const Parameters& parameters) {
		m_Parameters = parameters;

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Parameters.MinFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Parameters.MagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, m_Parameters.MinLOD);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, m_Parameters.MaxLOD);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Parameters.S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Parameters.T);
	}

	void Texture::SetMinFiler(const MinFilter& filter) {
		m_Parameters.MinFilter = filter;

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	}
	void Texture::SetMagFiler(const MagFilter& filter) {
		m_Parameters.MagFilter = filter;

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	}
	void Texture::SetMinLOD(const Int& value) {
		m_Parameters.MinLOD = value;

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, value);
	}
	void Texture::SetMaxLOD(const Int& value) {
		m_Parameters.MaxLOD = value;

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, value);
	}
	void Texture::SetWrapS(const Wrap& wrap) {
		m_Parameters.S = wrap;

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	}
	void Texture::SetWrapT(const Wrap& wrap) {
		m_Parameters.T = wrap;

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	}

	void Texture::Bind() const {
		if (m_ID == 0)
			Raise("Texture not created");

		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture::BindUnit(const uInt& unitID) const {
		if (glBindTextureUnit == nullptr)
			Raise("GLEW not initialized");

		if (m_ID == 0)
			Raise("Texture not created");

		glBindTextureUnit(unitID, m_ID);
	}

	std::type_index Texture::GetType() const {
		return typeid(Texture);
	}

	String Texture::GetFilePath() const noexcept {
		return m_Image.GetFilePath();
	}

	uInt2D Texture::GetSize() const noexcept {
		return m_Image.GetSize();
	}

	const std::unique_ptr<Byte[]>& Texture::GetData() const noexcept {
		return m_Image.GetData();
	}
	std::unique_ptr<Byte[]>& Texture::GetData() noexcept {
		return m_Image.GetData();
	}

	uInt Texture::GetChannelCount() const noexcept {
		return m_Image.GetChannelCount();
	}

	uInt Texture::GetID() const noexcept {
		return m_ID;
	}

	Bool Texture::IsCreated() const noexcept {
		return (m_ID != 0);
	}

	void Texture::Create(const uInt& channelsCount, const uInt2D& size, std::unique_ptr<Byte[]>&& pData) {
		m_Image.SetData(std::move(pData), size, channelsCount * 8);

		Create();
	}

	void Texture::Create() {
		if (m_ID != 0)
			glDeleteTextures(1, &m_ID);

		glGenTextures(1, &m_ID);
		if (m_ID == 0)
			_ThrowError("Failed to create texture");

		switch (m_Image.GetChannelCount()) {
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
		if (m_Image.GetData() != nullptr)
			gluBuild2DMipmaps(GL_TEXTURE_2D, m_ColorComponent, m_Image.GetSize().x, m_Image.GetSize().y, m_ColorComponent, GL_UNSIGNED_BYTE, m_Image.GetData().get());
		else
			glTexImage2D(GL_TEXTURE_2D, 0, m_ColorComponent, m_Image.GetSize().x, m_Image.GetSize().y, 0, m_ColorComponent, GL_UNSIGNED_BYTE, nullptr);

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

	void Texture::LoadFromFile(const Nt::String& filePath) {
		m_Image.LoadFromFile(filePath);

		Create();
	}

	void Texture::Release() {
		m_Image.Release();
		Delete();
	}
}