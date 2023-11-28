#pragma once

namespace Nt {
	class TreeView : public HandleWindow {
	public:
		TreeView() noexcept :
			m_TextWeight(400)
		{ }
		TreeView(const IntRect& rect) : m_TextWeight(400) {
			Create(rect, "");
		}

		void Create(const IntRect& rect) {
			Create(rect, "");
		}
		void Create(const IntRect& rect, const String& name) override {
			m_Name = L"";
			m_ClassName = WC_TREEVIEW;
			m_Rect = rect;
			m_Styles |= (TVS_HASLINES | WS_CHILD);
			m_pParam = this;

			if (m_pParent == nullptr || m_pParent->GetHandle() == nullptr)
				Raise("Missing parent window");

			InitializeCommonControls();
			_CreateWindow();

			SetWindowSubclass(m_hwnd, SubClassProc_TreeView, 0, 0);

			TreeView_SetExtendedStyle(m_hwnd, m_TreeExStyles, m_TreeExStyles);
			TreeView_EndEditLabelNow(m_hwnd, TRUE);
			TreeView_SetBkColor(m_hwnd, ToColorRef(m_BackgroundColor));
			TreeView_SetTextColor(m_hwnd, m_Color);
			
			m_hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
			TreeView_SetImageList(m_hwnd, m_hImageList, TVSIL_NORMAL);
		}

		HTREEITEM Add(const String& item, void* pData, const Int& image, const uInt& mask, const uInt& state, const HTREEITEM hParent = TVI_ROOT) {
			if (m_hwnd == nullptr)
				Raise("TreeView is not created");

			const std::wstring wItem = item;
			m_Items.push_back(wItem);

			TVINSERTSTRUCT tvInsert = { };
			tvInsert.hInsertAfter = hParent;
			tvInsert.hParent = hParent;
			tvInsert.item.mask = mask;
			tvInsert.item.lParam = (LPARAM)pData;
			tvInsert.item.pszText = const_cast<wChar*>(wItem.c_str());
			tvInsert.item.iImage = image;
			tvInsert.item.iSelectedImage = image;
			tvInsert.item.state = state;
			tvInsert.item.stateMask = state;

			return TreeView_InsertItem(m_hwnd, &tvInsert);
		}
		void Remove(const HTREEITEM& hItem) {
			TreeView_DeleteItem(m_hwnd, hItem);
		}
		void RemoveSelection() {
			if (m_hwnd == nullptr)
				Raise("TreeView is not created");

			TVITEM tvItem = { };
			tvItem.mask = TVIF_PARAM | TVIF_STATE;

			HTREEITEM hItem = TreeView_GetNextItem(m_hwnd, nullptr, TVGN_CARET);
			while (hItem != nullptr) {
				tvItem.hItem = hItem;
				TreeView_GetItem(m_hwnd, &tvItem);
				if (tvItem.state & TVIS_SELECTED) {
					tvItem.lParam = 0;
					TreeView_SetItem(m_hwnd, &tvItem);
					TreeView_DeleteItem(m_hwnd, hItem);
				}
				hItem = TreeView_GetNextItem(m_hwnd, hItem, TVGN_CARET);
			}
			TreeView_SelectItem(m_hwnd, nullptr);
		}

		void Clear() {
			if (m_hwnd == nullptr)
				Raise("TreeView is not created");

			m_Items.clear();
			TreeView_DeleteAllItems(m_hwnd);
		}

		void AddTreeExStyles(const DWord& styles) noexcept {
			m_TreeExStyles |= styles;
			if (m_hwnd)
				TreeView_SetExtendedStyle(m_hwnd, m_TreeExStyles, m_TreeExStyles);
		}

		Int AddIconInImageList(const HICON& hIcon) {
			return ImageList_AddIcon(m_hImageList, hIcon);
		}
		void ClearImageList() {
			ImageList_RemoveAll(m_hImageList);
		}

		HTREEITEM GetRootItem() const {
			if (m_hwnd == nullptr)
				Raise("TreeView is not created");
			return TreeView_GetRoot(m_hwnd);
		}
		Bool GetItem(TVITEMEX* pItem) const {
			return TreeView_GetItem(m_hwnd, pItem);
		}
		HTREEITEM GetNextItem(const HTREEITEM& hItem) {
			if (m_hwnd == nullptr)
				Raise("TreeView is not created");
			return TreeView_GetNextItem(m_hwnd, hItem, TVGN_NEXT);
		}

		void SetBackgroundColor(const uInt3D& color) {
			m_BackgroundColor = color;
			if (m_hwnd)
				TreeView_SetBkColor(m_hwnd, ToColorRef(m_BackgroundColor));
		}
		void SetTextColor(const uInt3D& color) {
			m_Color = RGB(color.r, color.g, color.b);
			if (m_hwnd)
				TreeView_SetTextColor(m_hwnd, m_Color);
		}

	private:
		std::vector<std::wstring> m_Items;
		HIMAGELIST m_hImageList;
		COLORREF m_Color;
		uInt m_TreeExStyles;
		String m_Text;
		uInt m_TextWeight;
	};
}