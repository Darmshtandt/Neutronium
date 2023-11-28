#pragma once

namespace Nt::GDI {
	class Bitmap {
	public:
		Bitmap() noexcept : 
			m_hBitmap(nullptr),
			m_pData(nullptr)
		{ }
		Bitmap(HBITMAP hBitmap) noexcept :
			m_hBitmap(hBitmap),
			m_pData(nullptr)
		{ }
		~Bitmap() {
			Delete();
		}

		void Create(const uInt2D& size, const Byte* pData) {
			if (m_hBitmap) {
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

			m_hBitmap = CreateBitmap(m_Size.x, m_Size.y, 1, m_BitCount, m_pData);
			if (m_hBitmap == nullptr)
				Raise("Failed to create bitmap.");
		}

		void Render(const HandleWindow& window, const IntRect& scaleRect) {
			RenderHBitmap(window.GetDC(), m_hBitmap, scaleRect);
		}
		void Render(HDC hdc, const IntRect& scaleRect = { }) {
			RenderHBitmap(hdc, m_hBitmap, scaleRect);
		}

		void Delete() {
			if ((m_hBitmap && (!m_pData)) || ((!m_hBitmap) && m_pData))
				Log::Error("Bitmap is corrupted");

			if (m_hBitmap) {
				DeleteObject(m_hBitmap);
				m_hBitmap = nullptr;
			}

			SAFE_DELETE(&m_pData);
		}

		void SetHBitmap(HBITMAP hBitmap) {
			m_hBitmap = hBitmap;
		}

		HBITMAP GetHandle() const noexcept {
			return m_hBitmap;
		}
		uInt2D GetSize() const noexcept {
			return m_Size;
		}
		Bool IsCreated() const noexcept {
			return (m_hBitmap != nullptr);
		}

	private:
		HBITMAP m_hBitmap;
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