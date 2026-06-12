#pragma once

#include <Nt/Graphics/System/HandleWindow.h>

namespace Nt {
	class Text {
	public:
		enum Format : Long {
			FORMAT_NONE = 0x00000000,
			FORMAT_TOP = 0x00000000,
			FORMAT_LEFT = 0x00000000,
			FORMAT_CENTER = 0x00000001,
			FORMAT_RIGHT = 0x00000002,
			FORMAT_VCENTER = 0x00000004,
			FORMAT_BOTTOM = 0x00000008,
			FORMAT_WORDBREAK = 0x00000010,
			FORMAT_SINGLELINE = 0x00000020,
			FORMAT_EXPANDTABS = 0x00000040,
			FORMAT_TABSTOP = 0x00000080,
			FORMAT_NOCLIP = 0x00000100,
			FORMAT_EXTERNALLEADING = 0x00000200,
			FORMAT_CALCRECT = 0x00000400,
			FORMAT_NOPREFIX = 0x00000800,
			FORMAT_INTERNAL = 0x00001000,
			FORMAT_EDITCONTROL = 0x00002000,
			FORMAT_PATH_ELLIPSIS = 0x00004000,
			FORMAT_END_ELLIPSIS = 0x00008000,
			FORMAT_MODIFYSTRING = 0x00010000,
			FORMAT_RTLREADING = 0x00020000,
			FORMAT_WORD_ELLIPSIS = 0x00040000,
			FORMAT_NOFULLWIDTHCHARBREAK = 0x00080000,
			FORMAT_HIDEPREFIX = 0x00100000,
			FORMAT_PREFIXONLY = 0x00200000
		};

	public:
		Text() noexcept = default;
		NT_API Text(const Int2D& position, const String& text) noexcept;
		NT_API Text(const IntRect& rect, const String& text) noexcept;

		NT_API void Draw(const HandleWindow& window);
#ifdef _WINDEF_
		NT_API void Draw(const HDC& hdc);
#endif

		NT_API void AddFormat(const uInt& format) noexcept;
		NT_API void RemoveFormat(const uInt& format) noexcept;

		NT_API void EnableBackground() noexcept;
		NT_API void DisableBackground() noexcept;
		NT_API void EnableDefaultRectSize() noexcept;
		NT_API void DisableDefaultRectSize() noexcept;

		NT_API _NODISCARD uInt GetFormat() const noexcept;
		NT_API _NODISCARD String GetText() const noexcept;
		NT_API _NODISCARD IntRect GetRect() const noexcept;
		NT_API _NODISCARD uInt2D GetFontSize() const noexcept;
		NT_API _NODISCARD uInt3D GetBackgroundColor() const noexcept;
		NT_API _NODISCARD uInt3D GetTextColor() const noexcept;
		NT_API _NODISCARD Bool IsEnabledBackground() const noexcept;
		NT_API _NODISCARD Bool IsEnabledDefaultRectSize() const noexcept;

		NT_API void SetFormat(const uInt& format) noexcept;
		NT_API void SetText(const String& text) noexcept;
		NT_API void SetRect(const IntRect& rect) noexcept;
		NT_API void SetPosition(const Int2D& position) noexcept;
		NT_API void SetSize(const Int2D& size) noexcept;
		NT_API void SetBackgroundColor(const Byte3D& color) noexcept;
		NT_API void SetColor(const Byte3D& color) noexcept;
		NT_API void SetWeight(const uInt& weight) noexcept;
		NT_API void SetFontSize(const uInt2D& size) noexcept;

	private:
		std::wstring m_Text;
		IntRect m_Rect;
		uInt m_Format = FORMAT_NONE;
		DWord m_BackgroundColor = (255 + (255 << 8) + (255 << 16));
		DWord m_Color = 0;
		uInt2D m_FontSize = { 6, 16 };
		uInt m_Weight = 400;
		Bool m_IsEnabledBackground = false;
		Bool m_IsEnabledDefaultRectSize = true;
	};
}