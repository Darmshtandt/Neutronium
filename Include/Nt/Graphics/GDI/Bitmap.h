#pragma once

#include <fstream>

namespace Nt::GDI {
	class Bitmap {
	public:
		Bitmap() noexcept : 
			m_Handle(nullptr),
			m_pData(nullptr),
			m_BitCount(0)
		{ 
		}
		Bitmap(const Bitmap& bitmap) noexcept :
			m_Handle(CopyHandleBitmap(bitmap.m_Handle)),
			m_Size(bitmap.m_Size),
			m_BitCount(bitmap.m_BitCount),
			m_pData(_CopyData(bitmap))
		{
		}
		Bitmap(Bitmap&& bitmap) noexcept :
			m_Handle(std::move(bitmap.m_Handle)),
			m_pData(std::move(bitmap.m_pData)),
			m_Size(bitmap.m_Size),
			m_BitCount(bitmap.m_BitCount)
		{
		}
		Bitmap(const HBITMAP& hBitmap) noexcept :
			m_Handle(hBitmap),
			m_BitCount(0),
			m_pData(nullptr)
		{ 
			if (m_Handle != nullptr) {
				BITMAP bitmap;
				GetObject(m_Handle, sizeof(bitmap), &bitmap);

				m_pData = reinterpret_cast<Byte*>(bitmap.bmBits);
				m_Size = uInt2D(bitmap.bmWidth, bitmap.bmHeight);
				m_BitCount = bitmap.bmBitsPixel;
			}
		}
		~Bitmap() {
			Delete();
		}

		void Create(const uInt2D& size, const Byte* pData) {
			if (m_Handle != nullptr) {
				Log::Warning("Bitmap already created.");
				return;
			}

			RequireNotNull(pData);
			if (m_pData != nullptr)
				Raise("Bitmap is corrupted.");

			m_Size = size;
			m_BitCount = 32;

			const uInt imageSize = (m_Size.x * m_Size.y);
			m_pData = new Byte[imageSize * 4];

			for (uInt i = 0; i < imageSize; ++i)
				reinterpret_cast<uInt*>(m_pData)[i] = reinterpret_cast<const uInt*>(pData)[imageSize - i - 1];

			_BGRtoRGB(m_pData);

			m_Handle = CreateBitmap(m_Size.x, m_Size.y, 1, m_BitCount, m_pData);
			RequireNotNull(m_Handle, "Failed to create bitmap.");
		}
		void LoadFromFile(const String& filePath) {
			std::ifstream file(filePath, std::ios::binary);
			if (!file.is_open())
				Raise("Failed to open file");

			Short code = 0;
			file.read((Char*)&code, sizeof(code));

			if (code != 0x4D42)
				Raise("Incorrect code");

			BITMAPCOREHEADER coreHeader;
			file.read((Char*)&coreHeader, sizeof(coreHeader));

			BITMAPINFOHEADER infoHeader;
			file.read((Char*)&infoHeader, sizeof(infoHeader));

			m_Size = Long2D(infoHeader.biWidth, infoHeader.biHeight);
			m_BitCount = uInt(infoHeader.biBitCount);

			const uInt imageByteCount = (m_Size.x * m_Size.y * (m_BitCount / 8));
			m_pData = new Byte[imageByteCount];

			file.read((Char*)m_pData, imageByteCount);
			file.close();

			m_Handle = CreateBitmap(m_Size.x, m_Size.y, infoHeader.biPlanes, m_BitCount, m_pData);
			RequireNotNull(m_Handle, "Failed to create bitmap.");
		}

		void Render(const HandleWindow& window, const IntRect& scaleRect) {
			RenderHBitmap(window.GetDC(), m_Handle, scaleRect);
		}
		void Render(HDC hdc, const IntRect& scaleRect = { }) {
			RenderHBitmap(hdc, m_Handle, scaleRect);
		}

		void Delete() {
			SAFE_DELETE(&m_pData);

			if (m_Handle != nullptr) {
				DeleteObject(m_Handle);
				m_Handle = nullptr;
			}
		}

		Bitmap& operator = (const Bitmap& bitmap) {
			if (this == &bitmap)
				return *this;

			Delete();

			m_Handle = bitmap.m_Handle;
			m_Size = bitmap.m_Size;
			m_BitCount = bitmap.m_BitCount;
			m_pData = _CopyData(bitmap);

			return *this;
		}
		Bitmap& operator = (Bitmap&& bitmap) noexcept {
			if (this == &bitmap)
				return *this;

			Delete();

			m_Handle = std::move(bitmap.m_Handle);
			m_pData = std::move(bitmap.m_pData);
			m_Size = bitmap.m_Size;
			m_BitCount = bitmap.m_BitCount;

			return *this;
		}

		void SetHBitmap(HBITMAP hBitmap) {
			m_Handle = hBitmap;
		}

		_NODISCARD
		HBITMAP GetHandle() const noexcept {
			return m_Handle;
		}

		_NODISCARD
		Byte* GetData() const noexcept {
			return m_pData;
		}

		_NODISCARD
		uInt2D GetSize() const noexcept {
			return m_Size;
		}

		_NODISCARD
		uInt GetBitCount() const noexcept {
			return m_BitCount;
		}

		_NODISCARD
		Bool IsCreated() const noexcept {
			return m_Handle;
		}

	private:
		HBITMAP m_Handle;
		Byte* m_pData;
		uInt2D m_Size;
		uInt m_BitCount;

	private:
		Byte* _CopyData(const Bitmap& bitmap) {
			const uInt dataSize = (bitmap.m_Size.x * bitmap.m_Size.y) * (bitmap.m_BitCount / 8);
			if (bitmap.m_pData != nullptr && dataSize > 0) {

				Byte* pCopiedData = new Byte[dataSize];
				memcpy(pCopiedData, bitmap.m_pData, dataSize);
				return pCopiedData;
			}

			return nullptr;
		}

		void _BGRtoRGB(Byte* pData) {
			RequireNotNull(pData);

			for (uInt i = 0; i < m_Size.x * m_Size.y; ++i)
				std::swap(pData[i * (m_BitCount / 8) + 0], pData[i * (m_BitCount / 8) + 2]);
		}
	};
}