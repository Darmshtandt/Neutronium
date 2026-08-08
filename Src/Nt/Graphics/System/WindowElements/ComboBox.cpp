#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOWINOFFSETS
#undef NOMSG

#include <windows.h>
#include <CommCtrl.h>
#include <shobjidl.h>

#include <Nt/Core/MessageWindow.h>
#include <Nt/Graphics/System/WindowElements/ComboBox.h>

namespace Nt {
	ComboBox::ComboBox(const IntRect& windowRect) {
		Create(windowRect);
	}

	void ComboBox::Create(const IntRect& windowRect) {
		Create(windowRect, "");
	}

	void ComboBox::Create(const IntRect& windowRect, [[maybe_unused]] const String& name) {
		m_ClassName = WC_COMBOBOX;
		m_WindowRect = windowRect;
		_CreateWindow();

		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<Long>(this));
		SetWindowSubclass(m_hwnd, _ClassProc, 0, 0);
	}

	LRESULT CALLBACK ComboBox::_ClassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData) {
		const ComboBox* pComboBox = reinterpret_cast<ComboBox*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (pComboBox) {
		}
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}

	void ComboBox::AddElement(const String& element) {
		if (!IsCreated())
			Raise("ComboBox is not created");

		m_Elements.push_back(element);

		const std::wstring wElement = element;
		const LRESULT result = SendMessage(m_hwnd, CB_ADDSTRING, 0, (LPARAM)wElement.c_str());
		if (result == CB_ERR)
			Raise("Failed to add element in ComboBox");
	}

	void ComboBox::RemoveElement(const String& element) {
		if (!IsCreated())
			Raise("ComboBox is not created");

		auto findedElement = std::find(m_Elements.begin(), m_Elements.end(), element);
		if (findedElement == m_Elements.end()) {
			MessageWindow("ComboBox::Remove: element \"" + element + "\" not found", "Error").Show(MessageIcon::ERROR);
			return;
		}

		SendMessage(m_hwnd, CB_DELETESTRING, std::distance(m_Elements.begin(), findedElement), 0);
		m_Elements.erase(findedElement);
	}

	void ComboBox::ClearElements() {
		if (!IsCreated())
			Raise("ComboBox is not created");
		if (m_Elements.size() == 0)
			return;

		SendMessage(m_hwnd, CB_RESETCONTENT, 0, 0);
		m_Elements.clear();
	}

	void ComboBox::SetCurrentElement(const uInt& index) {
		if (!IsCreated())
			Raise("ComboBox is not created");
		if (index >= m_Elements.size())
			Raise("Out of range");

		const LRESULT result = SendMessage(m_hwnd, CB_SETCURSEL, index, 0);
		if (result == CB_ERR)
			Raise("Failed to add element in ComboBox");
	}

}