#pragma once

namespace Nt {
	class Image : public IResource {
	public:
#		pragma pack(1)
		struct TGAHeader {
			Byte IDLength;
			Byte ColorMapType;
			Byte ImageType;

			Short ColorMapOrigin;
			Short ColorMapLength;
			Byte  ColorMapEntrySize;

			Short XOrigin;
			Short YOrigin;
			Short Width;
			Short Height;
			Byte Bits;
			Byte ImageDescriptor;
		};
#		pragma pack(4)

	protected:
		Image(const Image& otherImage) noexcept :
			IResource(otherImage),
			m_Size(otherImage.m_Size),
			m_BitCount(otherImage.m_BitCount)
		{
			if (otherImage.m_pData != nullptr) {
				const uInt ImageSize = (otherImage.m_Size.x * otherImage.m_Size.y * 4);
				m_pData = new uInt[ImageSize];
				memcpy(m_pData, otherImage.m_pData, ImageSize);
			}
			else {
				m_pData = nullptr;
			}
		}
		Image(const uInt& resourrceType) noexcept :
			IResource(resourrceType)
		{
		}
		Image(const uInt& resourrceType, cString fileName) :
			IResource(resourrceType)
		{
			LoadFromFile(fileName);
		}
		~Image() override {
			Release();
		}

	public:
		Byte* GetData() const noexcept {
			return (Byte*)m_pData;
		}
		uInt2D GetSize() const noexcept {
			return m_Size;
		}
		uInt GetBitCount() const noexcept {
			return m_BitCount;
		}

	protected:
		uInt2D m_Size;
		uInt m_BitCount;

		void _LoadFromFile() override {
			_LoadTGA();
		}
		virtual void _Release() override {
			m_Size = Zero2D<uInt>;
			m_BitCount = 0;
		}

	private:
		void _LoadTGA() {
			std::ifstream fImage(GetFilePath(), std::ios::in | std::ios::binary);
			if (!fImage.is_open())
				_ThrowError("Failed to open image file");

			TGAHeader header;
			fImage.read(reinterpret_cast<char*>(&header), 18);
			fImage.seekg(header.IDLength, std::ios::_Seekcur);

			m_Size.x = header.Width;
			m_Size.y = header.Height;
			m_BitCount = header.Bits / 8;

			if (m_BitCount == 0 || m_Size.x == 0 || m_Size.y == 0)
				_ThrowError("Image file is corrupted");

			const uInt pixelCount = m_Size.x * m_Size.y;
			if (m_pData)
				delete(m_pData);
			m_pData = new Byte[pixelCount * 4];

			Byte pixel[4] = { 255, 255, 255, 255 };
			uInt* pPixel = reinterpret_cast<uInt*>(pixel);
			uInt* pImage = reinterpret_cast<uInt*>(m_pData);

			auto AddPixel = [&]() {
				fImage.read(reinterpret_cast<char*>(&pixel), m_BitCount);
				std::swap(pixel[0], pixel[2]);

				(*pImage) = (*pPixel);
				++pImage;
			};

			constexpr uInt UNCOMPRESSED = 2;
			constexpr uInt COMPRESSED = 10;

			switch (header.ImageType) {
			case UNCOMPRESSED:
				for (uInt i = 0; i < pixelCount; ++i)
					AddPixel();
				break;
			case COMPRESSED:
			{
				uInt chunk = 0;
				uInt currentPixel = 0;

				do {
					fImage.read(reinterpret_cast<char*>(&chunk), 1);
					if (chunk < 128) {
						for (uInt i = 0; i <= chunk; ++i)
							AddPixel();
					}
					else {
						chunk -= 128;

						fImage.read(reinterpret_cast<char*>(&pixel), m_BitCount);
						std::swap(pixel[0], pixel[2]);

						for (uInt i = 0; i <= chunk; ++i) {
							(*pImage) = (*pPixel);
							++pImage;
						}
					}
					currentPixel += chunk + 1;
				} while (currentPixel < pixelCount);
				break;
			}
			default:
				SAFE_DELETE(&m_pData);
				fImage.close();
				_ThrowError("Image file is corrupted");
			}
			fImage.close();
		}
	};
}