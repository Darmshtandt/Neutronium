#pragma once

#include <fstream>
#include <Nt/Graphics/Resources/Image.h>

namespace Nt {
	struct ImageData final {
		std::string FileName;
		uInt2D Size;
		uInt BitCount = 0;
		Byte* pData = nullptr;
	};

	struct TGA {
#	pragma pack(push, 1)
		struct Header {
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
#	pragma pack(pop)

		static ImageData LoadFromFile(const Nt::String& filePath) {
			ImageData image = { };
			image.FileName = filePath;

			std::ifstream file(image.FileName, std::ios::binary);
			if (!file.is_open())
				Raise("Failed to open image file");

			Header header;
			file.read(reinterpret_cast<char*>(&header), 18);
			file.seekg(header.IDLength, std::ios::_Seekcur);

			image.Size = uInt2D(header.Width, header.Height);
			image.BitCount = header.Bits;

			const uInt channelCount = image.BitCount / 8;

			if (channelCount == 0 || image.Size.x == 0 || image.Size.y == 0)
				Raise("Image file is corrupted");

			const uInt pixelCount = image.Size.x * image.Size.y;
			image.pData = new Byte[pixelCount * 4];

			Byte pixel[4] = { 255, 255, 255, 255 };
			Byte* imagePointer = image.pData;

			auto AddPixel = [&]() {
				file.read(reinterpret_cast<Char*>(&pixel), channelCount);
				std::swap(pixel[0], pixel[2]);

				memcpy(imagePointer, pixel, 4);
				imagePointer += 4;
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
					file.read(reinterpret_cast<Char*>(&chunk), 1);
					if (chunk < 128) {
						for (uInt i = 0; i <= chunk; ++i)
							AddPixel();
					}
					else {
						chunk -= 128;

						file.read(reinterpret_cast<Char*>(&pixel), channelCount);
						std::swap(pixel[0], pixel[2]);

						for (uInt i = 0; i <= chunk; ++i) {
							memcpy(imagePointer, pixel, 4);
							imagePointer += 4;
						}
					}
					currentPixel += chunk + 1;
				} while (currentPixel < pixelCount);
				break;
			}
			default:
				SAFE_DELETE(&image.pData);
				file.close();
				Raise("Image file is corrupted");
			}

			file.close();

			image.BitCount = 32;
			return image;
		}
	};
}