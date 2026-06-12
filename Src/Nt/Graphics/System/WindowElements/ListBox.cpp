#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOMSG

#include <windows.h>
#include <windowsx.h>

#include <Nt/Graphics/System/WindowElements/ListBox.h>

namespace Nt {
	ListBox::ListBox(const IntRect& windowRect) {
		Create(windowRect);
	}

	void ListBox::Create(const IntRect& windowRect) {
		Create(windowRect, "ListBox");
	}

	void ListBox::Create(const IntRect& windowRect, [[maybe_unused]] const String& name) {
		m_ClassName = WC_LISTBOX;
		m_WindowRect = windowRect;
		_CreateWindow();

		SetWindowSubclass(m_hwnd, SubClassProc_ListBox, 0, 0);
	}

	uInt ListBox::AddItem(const String& text) {
		if (!IsCreated())
			Raise("ListBox not created");

		const std::wstring wText(text);
		ListBox_AddString(m_hwnd, wText.c_str());
		InvalidateRect(nullptr, true);

		++m_ItemCount;
		return (m_ItemCount - 1);
	}

	void ListBox::RemoveItem(const uInt& index) {
		if (!IsCreated())
			Raise("ListBox not created");
		if (index >= m_ItemCount)
			Raise("Out of range");

		ListBox_DeleteString(m_hwnd, index);
		--m_ItemCount;
	}

	void ListBox::SetItemData(const uInt& index, const void* pData) {
		if (!IsCreated())
			Raise("ListBox not created");
		if (index >= m_ItemCount)
			Raise("Out of range");
		ListBox_SetItemData(m_hwnd, index, pData);
	}

	void ListBox::SetItemHeight(const uInt& index, const uInt& height) {
		if (!IsCreated())
			Raise("ListBox not created");
		if (index >= m_ItemCount)
			Raise("Out of range");
		ListBox_SetItemHeight(m_hwnd, index, height);
	}

	Nt::String ListBox::GetText(const uInt& index) const {
		if (!IsCreated())
			Raise("ListBox not created");
		if (index >= m_ItemCount)
			Raise("Out of range");

		const uInt length = ListBox_GetTextLen(m_hwnd, index);
		if (Int(length) == -1)
			Raise("Length is -1");

		std::wstring text(length, L'\0');
		ListBox_GetText(m_hwnd, index, text.data());
		return text;
	}

	Int ListBox::GetSelectionIndex() const {
		if (!IsCreated())
			Raise("ListBox not created");
		return ListBox_GetCurSel(m_hwnd);
	}

	Long ListBox::_GetItemData(const uInt& index) {
		if (!IsCreated())
			Raise("ListBox not created");
		if (index >= m_ItemCount)
			Raise("Out of range");
		return ListBox_GetItemData(m_hwnd, index);
	}
}