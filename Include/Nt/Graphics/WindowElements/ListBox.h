#pragma once

namespace Nt {
	class ListBox : public HandleWindow {
	public:
		ListBox() = default;
		ListBox(const IntRect& windowRect) {
			Create(windowRect);
		}

		void Create(const IntRect& windowRect) {
			Create(windowRect, "");
		}

		uInt AddItem(const String& text) {
			if (!IsCreated())
				Raise("ListBox is not created");

			ListBox_AddString(m_hwnd, text.wstr().c_str());
			InvalidateRect(m_hwnd, nullptr, TRUE);
			++m_ItemCount;
			return (m_ItemCount - 1);
		}

		void RemoveItem(const uInt& index) {
			if (!IsCreated())
				Raise("ListBox is not created");
			if (index >= m_ItemCount)
				Raise("Out of range");
			ListBox_DeleteString(m_hwnd, index);
			--m_ItemCount;
		}

		void SetItemData(const uInt& index, const void* pData) {
			if (!IsCreated())
				Raise("ListBox is not created");
			if (index >= m_ItemCount)
				Raise("Out of range");
			ListBox_SetItemData(m_hwnd, index, pData);
		}
		void SetItemHeight(const uInt& index, const uInt& height) {
			if (!IsCreated())
				Raise("ListBox is not created");
			if (index >= m_ItemCount)
				Raise("Out of range");
			ListBox_SetItemHeight(m_hwnd, index, height);
		}

		String GetText(const uInt& index) const {
			if (!IsCreated())
				Raise("ListBox is not created");
			if (index >= m_ItemCount)
				Raise("Out of range");

			const uInt length = ListBox_GetTextLen(m_hwnd, index);
			if (Int(length) == -1)
				Raise("Length is -1");

			std::wstring text(length, L'\0');
			ListBox_GetText(m_hwnd, index, text.data());
			return text;
		}
		Int GetSelectionIndex() const {
			if (!IsCreated())
				Raise("ListBox is not created");
			return ListBox_GetCurSel(m_hwnd);
		}
		void* GetItemData(const uInt& index) {
			if (!IsCreated())
				Raise("ListBox is not created");
			if (index >= m_ItemCount)
				Raise("Out of range");
			return reinterpret_cast<void*>(ListBox_GetItemData(m_hwnd, index));
		}

	private:
		uInt m_ItemCount = 0;

	private:
		void Create(const IntRect& windowRect, [[maybe_unused]] const String& name) override {
			m_ClassName = WC_LISTBOX;
			m_WindowRect = windowRect;
			_CreateWindow();

			SetWindowSubclass(m_hwnd, SubClassProc_ListBox, 0, 0);
		}
	};
}