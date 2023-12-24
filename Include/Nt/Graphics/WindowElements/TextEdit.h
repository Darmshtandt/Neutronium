#pragma once

namespace Nt {
	class TextEdit : public HandleWindow {
	public:
		static const Int2D DefaultSize;

	public:
		TextEdit() noexcept :
			m_CharFormat({ }),
			m_TextColor(255, 255, 255)
		{
			m_CharFormat.cbSize = sizeof(m_CharFormat);
			m_CharFormat.dwMask = (CFM_COLOR | CFM_WEIGHT);
		}
		TextEdit(const HWND& hwnd) : HandleWindow(hwnd) {
			if (m_hwnd) {
				wChar placeHolder[1024];
				if (Edit_GetCueBannerText(m_hwnd, placeHolder, 1024))
					m_Placeholder = placeHolder;

				SendMessage(m_hwnd, EM_GETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&m_CharFormat));

				m_TextColor = ColorRefToVector(m_CharFormat.crTextColor);
				m_TextWeight = m_CharFormat.wWeight;
			}
		}
		TextEdit(const String& text, const Bool& isRich) {
			Create(text, isRich);
		}
		TextEdit(const IntRect& windowRect, const String& text, const Bool& isRich) {
			Create(windowRect, text, isRich);
		}

		void Create(const String& text, const Bool& isRich) {
			if (m_WindowRect.Right == 0)
				m_WindowRect.Right = DefaultSize.x;

			if (m_WindowRect.Bottom == 0)
				m_WindowRect.Bottom = DefaultSize.y;

			Create(m_WindowRect, text, isRich);
		}
		void Create(const IntRect& windowRect, const String& text, const Bool& isRich) {
			if (IsCreated()) {
				Log::Warning("The handle has already been created");
				return;
			}

			if (m_hParent == nullptr)
				m_ID = 0;
			if (isRich)
				m_ClassName = MSFTEDIT_CLASS;
			else
				m_ClassName = WC_EDIT;
			Create(windowRect, text);
		}

		String GetText() const noexcept {
			if (!IsCreated())
				return "";

			std::wstring text(GetWindowTextLength(m_hwnd), L'0');
			GetWindowText(m_hwnd, text.data(), text.size() + 1);
			return text;
		}

		void SetBackgroundColor(const uInt3D& color) {
			m_BackgroundColor = color;
			if (m_hwnd)
				SendMessage(m_hwnd, EM_SETBKGNDCOLOR, 0, VectorToColorRef(m_BackgroundColor));
		}
		void SetTextColor(const Byte3D& color) noexcept {
			m_TextColor = color;
			m_CharFormat.crTextColor = VectorToColorRef(m_TextColor);
			if (m_hwnd)
				SendMessage(m_hwnd, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&m_CharFormat));
		}
		void SetTextWeight(const uInt& weight) noexcept {
			m_CharFormat.wWeight = m_TextWeight = weight;
			if (m_hwnd)
				SendMessage(m_hwnd, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&m_CharFormat));
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
		Byte3D m_TextColor;
		uInt m_TextWeight;

	private:
		void Create(const IntRect& windowRect, const String& text) override {
			m_Name = text;
			m_WindowRect = windowRect;
			m_Styles |= WS_TABSTOP;
			m_ExStyles |= WS_EX_CLIENTEDGE;

			if (m_hParent)
				m_Styles |= WS_CHILD;

			if (m_ClassName == MSFTEDIT_CLASS)
				LoadMSFTEditDLL();
			_CreateWindow();

			SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(this));
			SetWindowSubclass(m_hwnd, _TextEditProc, 0, 0);

			SendMessage(m_hwnd, EM_SETBKGNDCOLOR, 0, VectorToColorRef(m_BackgroundColor));
			SendMessage(m_hwnd, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&m_CharFormat));
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

	inline const Int2D TextEdit::DefaultSize = { 300, 25 };
}