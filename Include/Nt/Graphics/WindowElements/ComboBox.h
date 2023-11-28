#pragma once

namespace Nt {
	class ComboBox : public HandleWindow {
	public:
		ComboBox() = default;
		ComboBox(const IntRect& windowRect) {
			Create(windowRect);
		}

		void Create(const IntRect& windowRect) {
			Create(windowRect, "");
		}
		void AddElement(const String& element) {
			if (!IsCreated())
				Raise("ComboBox is not created");

			m_Elements.push_back(element);
			const LRESULT result = SendMessage(m_hwnd, CB_ADDSTRING, 0, (LPARAM)element.wstr().c_str());
			if (result == CB_ERR)
				Raise("Failed to add element in ComboBox");
		}
		void RemoveElement(const String& element) {
			if (!IsCreated())
				Raise("ComboBox is not created");

			auto findedElement = std::find(m_Elements.begin(), m_Elements.end(), element.wstr());
			if (findedElement == m_Elements.end()) {
				ErrorBoxA(("ComboBox::Remove: element \"" + element + "\" not found").c_str(), "Error");
				return;
			}

			SendMessage(m_hwnd, CB_DELETESTRING, std::distance(m_Elements.begin(), findedElement), 0);
			m_Elements.erase(findedElement);
		}
		void ClearElements() {
			if (!IsCreated())
				Raise("ComboBox is not created");
			if (m_Elements.size() == 0)
				return;
			
			SendMessage(m_hwnd, CB_RESETCONTENT, 0, 0);
			m_Elements.clear();
		}
		void SetCurrentElement(const uInt& index) {
			if (!IsCreated())
				Raise("ComboBox is not created");
			if (index >= m_Elements.size())
				Raise("Out os range");

			const LRESULT result = SendMessage(m_hwnd, CB_SETCURSEL, index, 0);
			if (result == CB_ERR)
				Raise("Failed to add element in ComboBox");
		}

	private:
		std::vector<std::wstring> m_Elements;

	private:
		void Create(const IntRect& windowRect, [[maybe_unused]] const String& name) override {
			m_ClassName = WC_COMBOBOX;
			m_WindowRect = windowRect;
			_CreateWindow();

			SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<Long>(this));
			SetWindowSubclass(m_hwnd, _ClassProc, 0, 0);
		}

		static LRESULT CALLBACK _ClassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
			const ComboBox* pComboBox = reinterpret_cast<ComboBox*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (pComboBox) {
			}
			return DefSubclassProc(hwnd, uMsg, wParam, lParam);
		}
	};
}