#pragma once

namespace Nt {
	class ListView : public HandleWindow {
	public:
		ListView() = default;
		ListView(const IntRect& rect) {
			Create(rect);
		}

		void Create(const IntRect& rect) {
			Create(rect, "");
		}
		void Create(const IntRect& rect, const String& name) override {
			m_ClassName = WC_LISTVIEW;
			m_Name = name;
			m_Rect = rect;
			_CreateWindow();

			SetWindowSubclass(m_hwnd, SubClassProc_ListView, 0, 0);

			ListView_SetExtendedListViewStyleEx(m_hwnd, m_ListExStyles, m_ListExStyles);
			ListView_SetBkColor(m_hwnd, ToColorRef(m_BackgroundColor));
			ListView_SetTextColor(m_hwnd, ToColorRef(m_TextColor));

			if (!m_EnabledTextBackground)
				ListView_SetTextBkColor(m_hwnd, CLR_NONE);
			else
				ListView_SetTextBkColor(m_hwnd, ToColorRef(m_BackgroundColor));
		}

		void AddListExStyles(const DWord& styles) noexcept {
			m_ListExStyles |= styles;
			if (m_hwnd)
				ListView_SetExtendedListViewStyleEx(m_hwnd, m_ListExStyles, m_ListExStyles);
		}

		void AddColumn(const Int& columnID, const Nt::String& text, const Int& width) {
			if (m_hwnd == nullptr)
				Raise("ListView is not created");

			const std::wstring wText = text;
			m_Columns.push_back(wText);

			LVCOLUMN column = { };
			column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			column.fmt = LVCFMT_CENTER;
			column.cx = width;
			column.pszText = const_cast<wChar*>(wText.c_str());
			column.iSubItem = columnID;
			ListView_InsertColumn(m_hwnd, columnID, &column);
		}
		void AddItem(const Nt::String& text, void* pData, const uInt& mask, const uInt& state, const uInt& image) {
			const std::wstring wText = text;
			m_ItemTexts.push_back(wText);

			LVITEM item = { };
			item.mask = mask;
			item.lParam = (LPARAM)pData;
			item.iImage = image;
			item.state = state;
			item.stateMask = state;
			item.pszText = const_cast<wChar*>(wText.c_str());
			ListView_InsertItem(m_hwnd, &item);
			m_Items.push_back(item);
		}
		void RemoveItem(const uInt& index) {
			if (m_hwnd == nullptr)
				Raise("ListView is not created");
			if (index >= m_Items.size())
				Raise("Out of range");

			ListView_DeleteItem(m_hwnd, index);
			m_Items.erase(m_Items.begin() + index);
			m_ItemTexts.erase(m_ItemTexts.begin() + index);
		}

		void ClearItems() {
			if (m_hwnd == nullptr)
				Raise("ListView is not created");

			ListView_DeleteAllItems(m_hwnd);
			m_Items.clear();
			m_ItemTexts.clear();
		}

		void SetItemState(const Int& index, const uInt& styles, const uInt& mask) {
			if (m_hwnd == nullptr)
				Raise("ListView is not created");
			if (index >= Int(m_Items.size()))
				Raise("Out of range");
			
			ListView_SetItemState(m_hwnd, index, styles, mask);
		}

		void SortItems(const PFNLVCOMPARE& func, const Long& param) {
			ListView_SortItems(m_hwnd, func, param);
		}

		void EnableTextBackground() noexcept {
			if (!m_EnabledTextBackground) {
				m_EnabledTextBackground = true;
				if (m_hwnd)
					ListView_SetTextBkColor(m_hwnd, ToColorRef(m_BackgroundColor));
			}
		}
		void DisableTextBackground() noexcept {
			if (m_EnabledTextBackground) {
				m_EnabledTextBackground = false;
				if (m_hwnd)
					ListView_SetTextBkColor(m_hwnd, CLR_NONE);
			}
		}

		void SetBackgroundColor(const uInt3D& color) override {
			m_BackgroundColor = color;
			if (m_hwnd) {
				Hide();
				ListView_SetBkColor(m_hwnd, ToColorRef(m_BackgroundColor));
				Show();
			}
		}
		void SetTextColor(const uInt3D& color) {
			m_TextColor = color;
			if (m_hwnd) {
				ListView_SetTextColor(m_hwnd, ToColorRef(m_TextColor));
				InvalidateRect(m_hwnd, nullptr, FALSE);
			}
		}
		void SetTextBackgroundColor(const uInt3D& color) {
			m_TextBackgroundColor = color;
			if (m_hwnd) {
				ListView_SetTextBkColor(m_hwnd, ToColorRef(m_TextBackgroundColor));
				InvalidateRect(m_hwnd, nullptr, FALSE);
			}
		}

	private:
		std::vector<LVITEM> m_Items;
		std::vector<std::wstring> m_ItemTexts;
		std::vector<std::wstring> m_Columns;
		uInt3D m_TextBackgroundColor = { 255, 255, 255 };
		uInt3D m_TextColor;
		uInt m_ListExStyles;
		Bool m_EnabledTextBackground = true;
	};
}