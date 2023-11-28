#pragma once

namespace Nt::GDI {
	class Bitmap {
	public:
		Bitmap() noexcept : 
			m_Handle(nullptr),
			m_pData(nullptr)
		{ 
		}
		Bitmap(HBITMAP hBitmap) noexcept :
			m_Handle(hBitmap),
			m_pData(nullptr)
		{ 
			if (m_Handle) {
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
			if (m_Handle) {
				Log::Warning("Bitmap already created.");
				return;
			}
			else if (m_pData) {
				Raise("Bitmap is corrupted.");
				return;
			}
			else if (pData == nullptr) {
				Raise("pData is nullptr.");
				return;
			}

			m_Size = size;
			m_BitCount = 32;

			const uInt imageSize = (m_Size.x * m_Size.y);
			m_pData = new Byte[imageSize * 4];
			for (uInt i = 0; i < imageSize; ++i)
				reinterpret_cast<uInt*>(m_pData)[i] = 
					reinterpret_cast<const uInt*>(pData)[imageSize - i - 1];
			BGRtoRGB(m_pData);

			m_Handle = CreateBitmap(m_Size.x, m_Size.y, 1, m_BitCount, m_pData);
			if (m_Handle == nullptr)
				Raise("Failed to create bitmap.");
		}

		void Render(const HandleWindow& window, const IntRect& scaleRect) {
			RenderHBitmap(window.GetDC(), m_Handle, scaleRect);
		}
		void Render(HDC hdc, const IntRect& scaleRect = { }) {
			RenderHBitmap(hdc, m_Handle, scaleRect);
		}

		void Delete() {
			if ((m_Handle && (!m_pData)) || ((!m_Handle) && m_pData))
				Log::Error("Bitmap is corrupted");

			SAFE_DELETE(&m_pData);
			if (m_Handle) {
				DeleteObject(m_Handle);
				m_Handle = nullptr;
			}
		}

		void SetHBitmap(HBITMAP hBitmap) {
			m_Handle = hBitmap;
		}

		HBITMAP GetHandle() const noexcept {
			return m_Handle;
		}
		uInt2D GetSize() const noexcept {
			return m_Size;
		}
		Bool IsCreated() const noexcept {
			return m_Handle;
		}

	private:
		HBITMAP m_Handle;
		Byte* m_pData;
		uInt2D m_Size;
		uInt m_BitCount;

	private:
		void BGRtoRGB(Byte* pData) {
			Byte* imagePtr = m_pData;
			for (uInt i = 0; i < m_Size.x * m_Size.y; ++i)
				std::swap(*(imagePtr + i * 4 + 0), *(imagePtr + i * 4 + 2));
		}
	};
}