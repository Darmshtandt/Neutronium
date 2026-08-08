#pragma once

#include <Nt/Graphics/System/HandleWindow.h>

#include <functional>

#ifdef _WINDEF_
#	include <RichEdit.h>
#endif

namespace Nt {
	class TextEdit : public HandleWindow {
	public:
		static constexpr Int2D DefaultSize = { 300, 25 };

	public:
		NT_API TextEdit() noexcept;
#ifdef _WINDEF_
		NT_API TextEdit(const HWND& hwnd);
#endif
		NT_API TextEdit(const String& text, const Bool& isRich);
		NT_API TextEdit(const IntRect& windowRect, const String& text, const Bool& isRich);
		NT_API TextEdit(TextEdit&& edit) noexcept;
		~TextEdit() noexcept override = default;

		NT_API void Create(const String& text, const Bool& isRich);
		NT_API void Create(const IntRect& windowRect, const String& text, const Bool& isRich);

		NT_API void OnUpdate();

		NT_API String GetText() const noexcept;

		NT_API void SetOnUpdate(std::function<void()> onUpdate);
		NT_API void SetBackgroundColor(const Byte3D& color);
		NT_API void SetTextColor(const Byte3D& color) noexcept;
		NT_API void SetTextWeight(const uInt& weight) noexcept;
		NT_API void SetText(const String& text);
		NT_API void SetPlaceholder(const Nt::String& placeholder);

	private:
		std::function<void()> m_OnUpdate;

		std::wstring m_Placeholder;
#ifdef _WINDEF_
		CHARFORMAT2 m_CharFormat = { };
#endif
		Byte3D m_TextColor = { 255, 255, 255 };
		uInt m_TextWeight = 400;

	private:
		NT_API void Create(const IntRect& windowRect, const String& text) override;

#if 0
#ifdef _WINDEF_
		NT_API static LRESULT CALLBACK _TextEditProc(HWND hwnd, uInt uMsg, WPARAM wParam,
			LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
#endif
#endif
	};
}