#pragma once

namespace Nt {
	class TextEdit : public HandleWindow {
	public:
		TextEdit() noexcept :
			m_CharFormat({ }),
			m_Color(RGB(255, 255, 255))
		{
			m_CharFormat.cbSize = sizeof(m_CharFormat);
			m_CharFormat.dwMask = (CFM_COLOR | CFM_WEIGHT);
		}
		TextEdit(const IntRect& rect, const String& text, const Bool& isRich) {
			Create(rect, text, isRich);
		}

		void Create(const IntRect& rect, const String& text, const Bool& isRich) {
			if (m_pParent == nullptr)
				m_ID = 0;
			if (isRich)
				m_ClassName = MSFTEDIT_CLASS;
			else
				m_ClassName = WC_EDIT;
			Create(rect, text);
		}

		String GetText() const noexcept {
			if (m_hwnd == nullptr)
				return "";

			std::wstring text(GetWindowTextLength(m_hwnd), L'0');
			GetWindowText(m_hwnd, text.data(), text.size() + 1);
			return text;
		}

		void SetBackgroundColor(const uInt3D& color) {
			m_BackgroundColor = color;
			if (m_hwnd)
				SendMessage(m_hwnd, EM_SETBKGNDCOLOR, 0, ToColorRef(m_BackgroundColor));
		}
		void SetTextColor(const uInt3D& color) noexcept {
			m_Color = RGB(color.r, color.g, color.b);
			m_CharFormat.crTextColor = RGB(color.x, color.y, color.z);
			if (m_hwnd)
				SendMessage(m_hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&m_CharFormat);
		}
		void SetTextWeight(const uInt& weight) noexcept {
			m_TextWeight = weight;
			m_CharFormat.wWeight = (Word)weight;
			if (m_hwnd)
				SendMessage(m_hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&m_CharFormat);
		}
		void SetText(const String& text) {
			m_Name = text;
			if (m_hwnd)
				SetWindowText(m_hwnd, m_Name.c_str());
		}
		void SetPlaceholder(const Nt::String& placeholder) {
			m_Placeholder = placeholder;
			if (m_hwnd)
				SendMessage(m_hwnd, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(m_Placeholder.c_str()));
		}

	private:
		std::wstring m_Placeholder;
		CHARFORMAT2 m_CharFormat;
		COLORREF m_Color;
		uInt m_TextWeight;

	private:
		void Create(const IntRect& rect, const String& text) override {
			m_Name = text;
			m_Rect = rect;
			m_Styles |= WS_TABSTOP;
			m_ExStyles |= WS_EX_CLIENTEDGE;

			if (m_pParent)
				m_Styles |= WS_CHILD;

			if (m_ClassName == MSFTEDIT_CLASS)
				LoadMSFTEditDLL();
			_CreateWindow();

			SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(this));
			SetWindowSubclass(m_hwnd, _TextEditProc, 0, 0);

			SendMessage(m_hwnd, EM_SETBKGNDCOLOR, 0, ToColorRef(m_BackgroundColor));
			SendMessage(m_hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&m_CharFormat);
			SendMessage(m_hwnd, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(m_Placeholder.c_str()));

		}
		static LRESULT CALLBACK _TextEditProc(HWND hwnd, uInt uMsg, WPARAM wParam,
			LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
			if (uMsg == WM_COMMAND && HIWORD(wParam) == EN_UPDATE) {
				TextEdit* pThis =
					reinterpret_cast<TextEdit*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (pThis) {
					std::wstring wText(GetWindowTextLength(hwnd), L'\0');
					GetWindowText(hwnd, wText.data(), wText.length() + 1);
					pThis->m_Name = wText;
				}
			}
			return SubClassProc_KillFocus(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
		}
	};
}