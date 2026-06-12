// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <GL/GLEW.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/ResourceHandle.h>

namespace Nt {
	Texture::Texture(const String& FileName) {
		Texture::LoadFromFile(FileName);
	}
	Texture::Texture(const Texture& newTexture) : 
		m_Image(newTexture.m_Image)
	{
		SetParameters(newTexture.m_Parameters);
		Create();
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

	void Texture::FlipVertically() {
		if (!m_Image.IsValid())
			return;

		uInt* pOldData = reinterpret_cast<uInt*>(GetData());
		uInt* pFlippedData = new uInt[GetSize().x * GetSize().y];

		for (uInt y = 0; y < GetSize().y; ++y) {
			const uInt flipDataOffset = y * GetSize().x;
			const uInt thisDataOffset = (GetSize().y - y - 1) * GetSize().x;
			memcpy(pFlippedData + flipDataOffset, pOldData + thisDataOffset, sizeof(uInt) * GetSize().x);
		}

		m_Image.SetData(
			reinterpret_cast<Byte*>(pFlippedData),
			m_Image.GetSize(),
			m_Image.GetChannelCount());

		Delete();
		Create();
	}
	void Texture::Rotate_90_Degrees(const Bool& toRight) {
		if (!m_Image.IsValid())
			return;

		uInt* pOldData = reinterpret_cast<uInt*>(GetData());
		uInt* pFlippedData = new uInt[GetSize().x * GetSize().y];

		for (uInt y = 0; y < GetSize().y; ++y) {
			for (uInt x = 0; x < GetSize().x; ++x) {
				if (toRight)
					pFlippedData[y * GetSize().x + x] = pOldData[x * GetSize().x + y];
				else
					pFlippedData[(y + 1) * GetSize().x - x - 1] = pOldData[x * GetSize().x + y];
			}
		}

		m_Image.SetData(
			reinterpret_cast<Byte*>(pFlippedData),
			m_Image.GetSize(),
			m_Image.GetChannelCount());

		Delete();
		Create();
	}

	void Texture::SetParameters(const Parameters& parameters) {
		m_Parameters = parameters;
		if (m_ID == 0)
			return;

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Parameters.MinFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Parameters.MagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, m_Parameters.MinLOD);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, m_Parameters.MaxLOD);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Parameters.S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Parameters.T);
	}

	void Texture::SetMinFiler(const MinFilter& filter) {
		m_Parameters.MinFilter = filter;
		if (m_ID == 0)
			return;

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	}
	void Texture::SetMagFiler(const MagFilter& filter) {
		m_Parameters.MagFilter = filter;
		if (m_ID != 0) {
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		}
	}
	void Texture::SetMinLOD(const Int& value) {
		m_Parameters.MinLOD = value;
		if (m_ID != 0) {
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, value);
		}
	}
	void Texture::SetMaxLOD(const Int& value) {
		m_Parameters.MaxLOD = value;
		if (m_ID != 0) {
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, value);
		}
	}
	void Texture::SetWrapS(const Wrap& wrap) {
		m_Parameters.S = wrap;
		if (m_ID != 0) {
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		}
	}
	void Texture::SetWrapT(const Wrap& wrap) {
		m_Parameters.T = wrap;
		if (m_ID != 0) {
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		}
	}

	void Texture::Bind() const {
		Assert(m_ID != 0, "Texture not created");
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture::BindUnit(const uInt& unitID) const {
		Assert(glBindTextureUnit != nullptr, "GLEW not initialized");
		Assert(m_ID != 0, "Texture not created");
		glBindTextureUnit(unitID, m_ID);
	}

	std::type_index Texture::GetType() const {
		return typeid(Texture);
	}

	MinFilter Texture::GetMinFiler() const noexcept {
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, reinterpret_cast<Int*>(&m_Parameters.MinFilter));
		return m_Parameters.MinFilter;
	}
	MagFilter Texture::GetMagFiler() const noexcept {
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, reinterpret_cast<Int*>(&m_Parameters.MagFilter));
		return m_Parameters.MagFilter;
	}
	Int Texture::GetMinLOD() const noexcept {
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, reinterpret_cast<Int*>(&m_Parameters.MinLOD));
		return m_Parameters.MinLOD;
	}
	Int Texture::GetMaxLOD() const noexcept {
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, reinterpret_cast<Int*>(&m_Parameters.MaxLOD));
		return m_Parameters.MaxLOD;
	}
	Wrap Texture::GetWrapS() const noexcept {
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, reinterpret_cast<Int*>(&m_Parameters.S));
		return m_Parameters.S;
	}
	Wrap Texture::GetWrapT() const noexcept {
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, reinterpret_cast<Int*>(&m_Parameters.T));
		return m_Parameters.T;
	}

	String Texture::GetFilePath() const noexcept {
		return m_Image.GetFilePath();
	}
	uInt2D Texture::GetSize() const noexcept {
		return m_Image.GetSize();
	}
	const Byte* Texture::GetData() const noexcept {
		return m_Image.GetData();
	}
	Byte* Texture::GetData() noexcept {
		return m_Image.GetData();
	}
	uInt Texture::GetChannelCount() const noexcept {
		return m_Image.GetChannelCount();
	}
	uInt Texture::GetID() const noexcept {
		return m_ID;
	}
	Bool Texture::IsCreated() const noexcept {
		return m_ID != 0;
	}

	void Texture::Create(const uInt& channelsCount, const uInt2D& size, Byte* pData) {
		m_Image.SetData(pData, size, channelsCount * 8);
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

		const uInt width = m_Image.GetSize().x;
		const uInt height = m_Image.GetSize().y;
		const Int format = m_ColorComponent;

		if (m_Image.IsValid()) {
			gluBuild2DMipmaps(
				GL_TEXTURE_2D, format, width, height, format,
				GL_UNSIGNED_BYTE, m_Image.GetData());
		}
		else {
			glTexImage2D(
				GL_TEXTURE_2D, 0, format, width, height, 0, format,
				GL_UNSIGNED_BYTE, nullptr);
		}

		SetParameters(m_Parameters);
		GenerateMipmap();
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

	template class NT_API ResourceHandle<Texture>;
}