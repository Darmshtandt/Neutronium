#pragma once

namespace Nt {
	class TreeView : public HandleWindow {
	public:
		TreeView() noexcept :
			m_TextWeight(400)
		{ 
		}
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

			//SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<Long>(this));
			//SetWindowSubclass(m_hwnd, _ClassProc, 0, 0);

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
		void RemoveSelectedItems() {
			if (m_hwnd == nullptr)
				Raise("TreeView is not created");

			m_SelectedTreeItems.clear();
			for (HTREEITEM& hTreeItem : m_SelectedTreeItems)
				Remove(hTreeItem);
			TreeView_SelectItem(m_hwnd, nullptr);
		}

		void DeselectAll() {
			HTREEITEM hItem = TreeView_GetNextItem(m_hwnd, nullptr, TVGN_ROOT);
			while (hItem != nullptr) {
				TreeView_SetItemState(m_hwnd, hItem, 0, TVIS_SELECTED | TVIS_CUT);
				hItem = TreeView_GetNextItem(m_hwnd, hItem, TVGN_NEXT);
			}
			TreeView_SelectItem(m_hwnd, nullptr);
		}

		void Clear() {
			if (m_hwnd == nullptr)
				Raise("TreeView is not created");
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
		Bool GetItem(TVITEM* pItem) const {
			return TreeView_GetItem(m_hwnd, pItem);
		}
		HTREEITEM GetNextItem(const HTREEITEM& hItem) {
			if (m_hwnd == nullptr)
				Raise("TreeView is not created");
			return TreeView_GetNextItem(m_hwnd, hItem, TVGN_NEXT);
		}

		const std::vector<HTREEITEM>& GetSelectedItems() const noexcept {
			return m_SelectedTreeItems;
		}
		Bool IsRenamingEnabled() const noexcept {
			return m_IsRenammingEnabled;
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
		std::vector<HTREEITEM> m_SelectedTreeItems;
		HIMAGELIST m_hImageList;
		COLORREF m_Color;
		uInt m_TreeExStyles;
		String m_Text;
		uInt m_TextWeight;
		Bool m_IsRenammingEnabled = false;

	private:
		static LRESULT CALLBACK _ClassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
			TreeView* pThis = reinterpret_cast<TreeView*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (pThis == nullptr)
				Raise("This pointer in class procedure is nullptr.");

			switch (uMsg) {
			case WM_LBUTTONDOWN: {
				TVHITTESTINFO hitTestInfo;
				hitTestInfo.pt.x = GET_X_LPARAM(lParam);
				hitTestInfo.pt.y = GET_Y_LPARAM(lParam);
				hitTestInfo.flags = TVHT_ONITEM;

				HTREEITEM hItem = TreeView_HitTest(hwnd, &hitTestInfo);
				if (hItem != nullptr && (hitTestInfo.flags & TVHT_ONITEM)) {
					TreeView_SelectItem(hwnd, hItem);
					pThis->_HandleSelectItem(hItem);
				}
			}
			break;
			case TVM_EDITLABEL:
				pThis->m_IsRenammingEnabled = true;
				break;
			case TVM_ENDEDITLABELNOW:
				pThis->m_IsRenammingEnabled = false;
				break;
			case TVM_DELETEITEM:
				pThis->_HandleDeleteItem();
				break;
			case WM_KEYDOWN:
				if (wParam == VK_ESCAPE) {
					pThis->DeselectAll();
					pThis->m_SelectedTreeItems.clear();
				}
				break;
			}
			return DefSubclassProc(hwnd, uMsg, wParam, lParam);
		}
		void _HandleSelectItem(HTREEITEM hSelectedItem) {
			if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
				for (HTREEITEM& hTreeItem : m_SelectedTreeItems)
					TreeView_SetItemState(m_hwnd, hTreeItem, 0, TVIS_SELECTED);
				m_SelectedTreeItems.clear();
			}

			auto iterator = std::find(
				m_SelectedTreeItems.begin(), m_SelectedTreeItems.end(), hSelectedItem);
			if (iterator != m_SelectedTreeItems.end()) {
				TreeView_SetItemState(m_hwnd, hSelectedItem, 0, TVIS_SELECTED);
				m_SelectedTreeItems.erase(iterator);
			}
			else {
				m_SelectedTreeItems.push_back(hSelectedItem);
			}

			TVITEM tvItem = { };
			tvItem.mask = TVIF_PARAM | TVIF_STATE;
			tvItem.stateMask = TVIS_SELECTED;

			for (HTREEITEM& hTreeItem : m_SelectedTreeItems) {
				tvItem.hItem = hTreeItem;
				TreeView_GetItem(m_hwnd, &tvItem);

				tvItem.state = TVIS_SELECTED;
				TreeView_SetItem(m_hwnd, &tvItem);
			}
		}
		void _HandleDeleteItem() {
			TVITEM tvItem = { };
			tvItem.mask = TVIF_PARAM | TVIF_STATE;
			tvItem.stateMask = TVIS_SELECTED;

			for (HTREEITEM& hTreeItem : m_SelectedTreeItems) {
				tvItem.hItem = hTreeItem;
				TreeView_GetItem(m_hwnd, &tvItem);

				if (tvItem.lParam != 0)
					tvItem.lParam = 0;

				tvItem.state = TVIS_SELECTED;
				TreeView_SetItem(m_hwnd, &tvItem);
			}
		}
	};
}