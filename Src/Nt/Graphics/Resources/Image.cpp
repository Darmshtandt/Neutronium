// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <filesystem>

#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/ImageLoaders.h>

namespace Nt {
	Image::Image(const String& filePath) {
		Image::LoadFromFile(filePath);
	}
	Image::Image(const Image& otherImage) :
		m_Size(otherImage.m_Size),
		m_BitCount(otherImage.m_BitCount),
		m_FilePath(otherImage.m_FilePath)
	{
		if (otherImage.m_pData != nullptr) {
			const uInt ImageSize = (otherImage.m_Size.x * otherImage.m_Size.y * 4);
			m_pData.reset(reinterpret_cast<Byte*>(new uInt[ImageSize]));
			memcpy(m_pData.get(), otherImage.m_pData.get(), ImageSize);
		}
		else {
			m_pData = nullptr;
		}
	}
	Image::Image(Image&& otherImage) noexcept :
		m_Size(otherImage.m_Size),
		m_BitCount(otherImage.m_BitCount),
		m_FilePath(otherImage.m_FilePath),
		m_pData(std::move(otherImage.m_pData))
	{
	}
	Image::~Image() {
		Image::Release();
	}

	void Image::LoadFromFile(const Nt::String& filePath) {
		m_FilePath = filePath;

		const std::string extension = Nt::String(std::filesystem::path(filePath).extension()).ToUpper().substr(1);

		ImageData data = { };
		if (extension == "TGA")
			data = TGA::LoadFromFile(m_FilePath);
		else
			Raise("Unknown extension");

		std::unique_ptr<Byte[]> uniqueData;
		uniqueData.reset(data.pData);
		SetData(std::move(uniqueData), data.Size, data.BitCount);
	}
	void Image::Release() {
		m_Size = { };
		m_BitCount = 0;
		m_pData.reset();
	}

	void Image::Write(std::ostream& stream) const {
		Serialization::WriteAll(stream, m_FilePath);
	}
	void Image::Read(std::istream& Stream) {
		Serialization::ReadAll(Stream, m_FilePath);
	}

	String Image::GetFilePath() const noexcept {
		return m_FilePath;
	}

	std::type_index Image::GetType() const {
		return typeid(Image);
	}

	const std::unique_ptr<Byte[]>& Image::GetData() const noexcept {
		return m_pData;
	}
	std::unique_ptr<Byte[]>& Image::GetData() noexcept {
		return m_pData;
	}
	uInt2D Image::GetSize() const noexcept {
		return m_Size;
	}
	uInt Image::GetBitCount() const noexcept {
		return m_BitCount;
	}
	uInt Image::GetChannelCount() const noexcept {
		return (m_BitCount / 8);
	}

	void Image::SetData(std::unique_ptr<Byte[]>&& pData, const uInt2D& size, const uInt& bitCount) noexcept {
		m_pData = std::move(pData);
		m_Size = size;
		m_BitCount = bitCount;
	}
}