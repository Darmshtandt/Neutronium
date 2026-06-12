// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <filesystem>

#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/ImageLoaders.h>

namespace Nt {
	Image::Image(const String& filePath) {
		Image::LoadFromFile(filePath);
	}
	Image::Image(const Image& other) :
		m_Size(other.m_Size),
		m_BitCount(other.m_BitCount),
		m_FilePath(other.m_FilePath)
	{
		if (!other.IsValid())
			return;

		const uInt bufferSize = 
			other.m_Size.x * other.m_Size.y * other.GetChannelCount();

		m_pData.reset(new Byte[bufferSize]);
		memcpy(m_pData.get(), other.m_pData.get(), bufferSize);
	}
	Image::Image(Image&& otherImage) noexcept :
		m_Size(std::move(otherImage.m_Size)),
		m_BitCount(otherImage.m_BitCount),
		m_FilePath(std::move(otherImage.m_FilePath)),
		m_pData(std::move(otherImage.m_pData))
	{
	}
	Image::~Image() {
		Image::Release();
	}

	void Image::LoadFromFile(const String& filePath) {
		m_FilePath = filePath;

		const std::string extension = String(
			std::filesystem::path(filePath).extension())
				.ToUpper().substr(1);

		ImageData data = { };
		if (extension == "TGA")
			data = TGA::LoadFromFile(m_FilePath);
		else
			Raise("Unknown extension");

		SetData(data.pData, data.Size, data.BitCount);
	}
	void Image::Release() {
		m_Size = { };
		m_BitCount = 0;
		m_pData.reset();
	}

	String Image::GetFilePath() const noexcept {
		return m_FilePath;
	}

	std::type_index Image::GetType() const {
		return typeid(Image);
	}

	const Byte* Image::GetData() const noexcept {
		return m_pData.get();
	}
	Byte* Image::GetData() noexcept {
		return m_pData.get();
	}
	uInt2D Image::GetSize() const noexcept {
		return m_Size;
	}
	uInt Image::GetBitCount() const noexcept {
		return m_BitCount;
	}
	uInt Image::GetChannelCount() const noexcept {
		return m_BitCount / 8;
	}

	Bool Image::IsValid() const noexcept {
		return m_pData != nullptr;
	}

	void Image::SetData(Byte* pData, const uInt2D& size, const uInt& bitCount) noexcept {
		m_pData.reset(pData);
		m_Size = size;
		m_BitCount = bitCount;
	}
}