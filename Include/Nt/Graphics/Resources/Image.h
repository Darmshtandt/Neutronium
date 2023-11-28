#pragma once

namespace Nt {
	class Image : public IResource {
	private:
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
		Image(const Image& OtherImage) noexcept :
			IResource(OtherImage),
			m_Size(OtherImage.m_Size),
			m_BitCount(OtherImage.m_BitCount) {
			const uInt ImageSize = OtherImage.m_Size.x * OtherImage.m_Size.y * 4;
			m_pData = new uInt[ImageSize];
			memcpy(m_pData, OtherImage.m_pData, ImageSize);
		}
		Image(const uInt& ResourrceType) noexcept :
			IResource(ResourrceType) {
		}
		Image(const uInt& ResourrceType, cString FileName) :
			IResource(ResourrceType) {
			LoadFromFile(FileName);
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
			m_Size = uInt2D::Zero;
			m_BitCount = 0;
		}

	private:
		void _LoadTGA() {
			std::ifstream fImage(GetFilePath(), std::ios::in | std::ios::binary);
			if (!fImage.is_open())
				_ThrowError("Failed to open image file");

			TGAHeader Head;
			fImage.read(reinterpret_cast<char*>(&Head), 18);
			fImage.seekg(Head.IDLength, std::ios::_Seekcur);

			m_Size.x = Head.Width;
			m_Size.y = Head.Height;
			m_BitCount = Head.Bits / 8;

			if (m_BitCount == 0 || m_Size.x == 0 || m_Size.y == 0)
				_ThrowError("Image file is corrupted");

			const uInt PixelCount = m_Size.x * m_Size.y;
			if (m_pData)
				delete(m_pData);
			m_pData = new Byte[PixelCount * 4];

			Byte Pixel[4] = { 255, 255, 255, 255 };
			uInt* pPixel = reinterpret_cast<uInt*>(Pixel);
			uInt* pImage = reinterpret_cast<uInt*>(m_pData);

			auto AddPixel = [&]() {
				fImage.read(reinterpret_cast<char*>(&Pixel), m_BitCount);
				std::swap(Pixel[0], Pixel[2]);

				(*pImage) = (*pPixel);
				++pImage;
			};

			constexpr uInt UNCOMPRESSED = 2;
			constexpr uInt COMPRESSED = 10;

			switch (Head.ImageType) {
			case UNCOMPRESSED:
				for (uInt i = 0; i < PixelCount; ++i)
					AddPixel();
				break;
			case COMPRESSED:
			{
				uInt Chunk = 0;
				uInt CurrentPixel = 0;

				do {
					fImage.read(reinterpret_cast<char*>(&Chunk), 1);
					if (Chunk < 128) {
						for (uInt i = 0; i <= Chunk; ++i)
							AddPixel();
					}
					else {
						Chunk -= 128;

						fImage.read(reinterpret_cast<char*>(&Pixel), m_BitCount);
						std::swap(Pixel[0], Pixel[2]);

						for (uInt i = 0; i <= Chunk; ++i) {
							(*pImage) = (*pPixel);
							++pImage;
						}
					}
					CurrentPixel += Chunk + 1;
				} while (CurrentPixel < PixelCount);
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