#pragma once

#include <Nt/Graphics/System/GDI/GDI.h>
#include <Nt/Graphics/System/HandleWindow.h>

#include <fstream>

namespace Nt::GDI {
	class Bitmap {
	public:
		Bitmap() noexcept = default;
		NT_API Bitmap(const Bitmap& bitmap) noexcept;
		NT_API Bitmap(Bitmap&& bitmap) noexcept;
		NT_API Bitmap(const HBITMAP& hBitmap) noexcept;
		NT_API ~Bitmap();

		NT_API void Create(const uInt2D& size, const Byte* pData);
		NT_API void LoadFromFile(const String& filePath);

		NT_API void Render(const HandleWindow& window, const IntRect& scaleRect);

#ifdef _WINDEF_
		NT_API void Render(HDC hdc, const IntRect& scaleRect = { });
#endif

		NT_API void Delete();

		NT_API Bitmap& operator = (const Bitmap& bitmap);
		NT_API Bitmap& operator = (Bitmap&& bitmap) noexcept;

#ifdef _WINDEF_
		NT_API void SetHBitmap(HBITMAP hBitmap);

		NT_API _NODISCARD HBITMAP GetHandle() const noexcept;
#endif
		NT_API _NODISCARD Byte* GetData() const noexcept;
		NT_API _NODISCARD uInt2D GetSize() const noexcept;
		NT_API _NODISCARD uInt GetBitCount() const noexcept;
		NT_API _NODISCARD Bool IsCreated() const noexcept;

	private:
#ifdef _WINDEF_
		HBITMAP m_Handle = nullptr;
#endif
		Byte* m_pData = nullptr;
		uInt2D m_Size;
		uInt m_BitCount = 0;

	private:
		Byte* _CopyData(const Bitmap& bitmap);

		void _BGRtoRGB(Byte* pData);
	};
}