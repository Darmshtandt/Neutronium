#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOMSG
#undef NOWINOFFSETS

#include <windows.h>
#include <Nt/Graphics/System/WindowElements/TreeView.h>
#include <Nt/Core/MessageWindow.h>

namespace Nt {
	TreeView::TreeView(const IntRect& windowRect) :
		m_TreeExStyles(EX_STYLE_NONE),
		m_TextWeight(400)
	{
		Create(windowRect, "");
	}

	void TreeView::Create(const IntRect& windowRect, const String& name) {
		m_Name = name;
		m_ClassName = WC_TREEVIEW;
		m_WindowRect = windowRect;
		m_Styles |= (TVS_HASLINES | HandleWindow::STYLE_CHILD);
		m_pParam = this;

		RequireNotNull(m_hParent, "Missing parent window");

		InitializeCommonControls();
		_CreateWindow();

		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<Long>(this));
		SetWindowSubclass(m_hwnd, SubClassProc, 0, 0);

		TreeView_SetExtendedStyle(m_hwnd, m_TreeExStyles, m_TreeExStyles);
		TreeView_EndEditLabelNow(m_hwnd, TRUE);
		TreeView_SetBkColor(m_hwnd, VectorToColorRef(m_BackgroundColor));
		TreeView_SetTextColor(m_hwnd, VectorToColorRef(m_TextColor));

		m_hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
		TreeView_SetImageList(m_hwnd, m_hImageList, TVSIL_NORMAL);
	}

	void TreeView::Create(const IntRect& windowRect) {
		Create(windowRect, "");
	}

	TreeView::ItemID TreeView::Add(const Item& item, const ItemID& parentItemID, const ItemID& afterItemID) {
		TVINSERTSTRUCT tvInsert = { };
		tvInsert.hInsertAfter = afterItemID;
		tvInsert.hParent = parentItemID;
		tvInsert.item = item.ToWinApiStruct();

		const Long lParam = reinterpret_cast<Long>(&tvInsert);
		ItemID itemID = reinterpret_cast<ItemID>(_SendMessage(TVM_INSERTITEM, 0, lParam));
		return RequireNotNull(itemID, "Failed to add item");
	}

	Bool TreeView::Remove(const ItemID& itemID) {
		return _SendMessage(TVM_DELETEITEM, 0, reinterpret_cast<Long>(itemID));
	}

	void TreeView::RemoveSelectedItems() {
		m_SelectedTreeItems.clear();
		for (ItemID& itemID : m_SelectedTreeItems)
			Remove(itemID);

		_SendMessage(TVM_SELECTITEM, TVGN_CARET, 0);
	}

	Bool TreeView::Clear() {
		return _SendMessage(TVM_DELETEITEM, 0, reinterpret_cast<Long>(TVI_ROOT));
	}

	void TreeView::Select(const ItemID& itemID) {
		_SendMessage(TVM_SELECTITEM, TVGN_CARET, reinterpret_cast<Long>(itemID));
		SetItemState(itemID, Item::STATE_SELECTED, Item::STATE_SELECTED);
	}

	void TreeView::Deselect(const ItemID& itemID) {
		_SendMessage(TVM_SELECTITEM, TVGN_CARET, 0);
		SetItemState(itemID, Item::STATE_NONE, Item::STATE_SELECTED);
	}

	void TreeView::DeselectAll() {
		if (!IsCreated())
			Raise("TreeView not created");

		ItemID itemID = reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_ROOT, 0));
		while (itemID != nullptr) {
			TreeView_SetItemState(m_hwnd, itemID, 0, TVIS_SELECTED);
			itemID = TreeView_GetNextItem(m_hwnd, itemID, TVGN_NEXT);
		}
		TreeView_SelectItem(m_hwnd, nullptr);
	}

	HRESULT TreeView::AddTreeExStyles(const ExtendStyles& styles) noexcept {
		const ExtendStyles updatedStyles = ExtendStyles(m_TreeExStyles | styles);
		return SetExtendedStyle(updatedStyles, updatedStyles);
	}

	HRESULT TreeView::RemoveTreeExStyles(const ExtendStyles& styles) noexcept {
		const ExtendStyles updatedStyles = ExtendStyles(m_TreeExStyles & (~styles));
		return SetExtendedStyle(updatedStyles, updatedStyles);
	}

	TreeView::HitTestInfo TreeView::HitTest(const Int2D& point) {
		HitTestInfo info = { };
		info.Point = point;
		_SendMessage(TVM_HITTEST, 0, reinterpret_cast<Long>(&point));
		return info;
	}

	Bool TreeView::Expand(const ItemID& itemID, const ExpandTypes& type) {
		return _SendMessage(TVM_EXPAND, type, reinterpret_cast<Long>(itemID));
	}

	TextEdit TreeView::BeginEditLabel(const ItemID& itemID) {
		const Long lParam = reinterpret_cast<Long>(itemID);
		const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(TVM_EDITLABEL, 0, lParam));
		return TextEdit(hwnd);
	}

	Bool TreeView::EndEditLabelNow(const Bool& cancelWithoutSaving) {
		return _SendMessage(TVM_ENDEDITLABELNOW, cancelWithoutSaving, 0);
	}

	ImageList TreeView::CreateDragImage(const ItemID& itemID) {
		const Long lParam = reinterpret_cast<Long>(itemID);
		const HIMAGELIST hList = reinterpret_cast<HIMAGELIST>(_SendMessage(TVM_CREATEDRAGIMAGE, 0, lParam));
		return ImageList(hList);
	}

	Bool TreeView::SelectDropTarget(const ItemID& itemID) {
		return _SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE, reinterpret_cast<Long>(itemID));
	}

	Bool TreeView::SelectSetFirstVisible(const ItemID& itemID) {
		return _SendMessage(TVM_SELECTITEM, TVGN_FIRSTVISIBLE, reinterpret_cast<Long>(itemID));
	}

	Bool TreeView::SortChildren(const ItemID& itemID, const SortFunction& sortFunction, const Long& param, const Bool& recurse) {
		SortStruct sortStruct;
		sortStruct.ItemID = itemID;
		sortStruct.Function = sortFunction;
		sortStruct.Param = param;
		return _SendMessage(TVM_SORTCHILDRENCB, recurse, reinterpret_cast<Long>(&sortStruct));
	}

	Bool TreeView::SortChildren(const ItemID& itemID, const Bool& recurse) {
		return _SendMessage(TVM_SORTCHILDREN, recurse, reinterpret_cast<Long>(itemID));
	}

	Bool TreeView::EnsureVisible(const ItemID& itemID) {
		return _SendMessage(TVM_ENSUREVISIBLE, 0, reinterpret_cast<Long>(itemID));
	}

	Int TreeView::ShowInfoTip(const ItemID& itemID) {
		return _SendMessage(TVM_SHOWINFOTIP, 0, reinterpret_cast<Long>(itemID));
	}

	void TreeView::HandleNotify(NMHDR* pNMHDR) {
		if (pNMHDR) {
			switch (pNMHDR->code) {
			case TVN_SELCHANGED:
			{
				ItemID hSelectedItem = TreeView_GetSelection(m_hwnd);
				if (hSelectedItem == nullptr)
					break;

				if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
					for (ItemID& itemID : m_SelectedTreeItems)
						TreeView_SetItemState(m_hwnd, itemID, 0, TVIS_SELECTED);
					m_SelectedTreeItems.clear();
				}

				TVITEM tvItem = { };
				tvItem.mask = TVIF_PARAM | TVIF_STATE;
				tvItem.stateMask = TVIS_SELECTED;

				auto iterator = std::find(
					m_SelectedTreeItems.begin(),
					m_SelectedTreeItems.end(), hSelectedItem);
				if (iterator != m_SelectedTreeItems.end()) {
					m_SelectedTreeItems.erase(iterator);
					TreeView_SetItemState(m_hwnd, hSelectedItem, 0, TVIS_SELECTED);
				}
				else {
					m_SelectedTreeItems.push_back(hSelectedItem);
				}

				for (ItemID& itemID : m_SelectedTreeItems)
					TreeView_SetItemState(m_hwnd, itemID, 0, TVIS_SELECTED);
				break;
			}
			case TVN_BEGINLABELEDIT:
				m_IsRenammingEnabled = false;
				break;
			case TVN_ENDLABELEDIT:
				m_IsRenammingEnabled = true;
				break;
			case TVN_DELETEITEM:
			{
				TVITEM tvItem = { };
				tvItem.mask = TVIF_PARAM | TVIF_STATE;
				tvItem.stateMask = TVIS_SELECTED;

				for (ItemID& itemID : m_SelectedTreeItems) {
					tvItem.hItem = itemID;
					TreeView_GetItem(m_hwnd, &tvItem);

					tvItem.lParam = 0;
					tvItem.state = TVIS_SELECTED;
					TreeView_SetItem(m_hwnd, &tvItem);
				}
			}
			break;
			}
		}
	}

	TreeView::ItemID TreeView::GetRootItem() const {
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_ROOT, 0));
	}

	TreeView::ItemID TreeView::GetNextItem(const ItemID& itemID) const {
		const Long lParam = reinterpret_cast<Long>(itemID);
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_NEXT, lParam));
	}

	TreeView::ItemID TreeView::GetPrevItem(const ItemID& itemID) const {
		const Long lParam = reinterpret_cast<Long>(itemID);
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_PREVIOUS, lParam));
	}

	TreeView::ItemID TreeView::GetNextParent(const ItemID& childID) const {
		const Long lParam = reinterpret_cast<Long>(childID);
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_PARENT, lParam));
	}

	TreeView::ItemID TreeView::GetNextChild(const ItemID& parentID) const {
		const Long lParam = reinterpret_cast<Long>(parentID);
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_CHILD, lParam));
	}

	TreeView::ItemID TreeView::GetFirstVisible() const {
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0));
	}

	TreeView::ItemID TreeView::GetNextVisible(const ItemID& itemID) const {
		const Long lParam = reinterpret_cast<Long>(itemID);
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, lParam));
	}

	TreeView::ItemID TreeView::GetPrevVisible(const ItemID& itemID) const {
		const Long lParam = reinterpret_cast<Long>(itemID);
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, lParam));
	}

	TreeView::ItemID TreeView::GetLastVisible() const {
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_LASTVISIBLE, 0));
	}

	TreeView::ItemID TreeView::GetDropHilight() const {
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_DROPHILITE, 0));
	}

	TreeView::ItemID TreeView::GetNextSelected(const ItemID& itemID) const {
		const Long lParam = reinterpret_cast<Long>(itemID);
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_NEXTSELECTED, lParam));
	}

	TreeView::ItemID TreeView::GetSelection() const {
		return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_CARET, 0));
	}

	Bool TreeView::GetItem(Item* pItem) const {
		TVITEM tvItem = RequireNotNull(pItem)->ToWinApiStruct();

		const Bool result = _SendMessage(TVM_GETITEM, 0, reinterpret_cast<Long>(&tvItem));
		if (result)
			(*pItem) = tvItem;

		return result;
	}

	IntRect TreeView::GetItemRect(const ItemID& itemID, const Bool& isTextRect) const {
		void* pData = itemID;
		_SendMessage(TVM_GETITEMRECT, isTextRect, reinterpret_cast<Long>(pData));
		if (pData == nullptr)
			return IntRect();
		return *reinterpret_cast<RECT*>(pData);
	}

	uInt TreeView::GetCount() const {
		return _SendMessage(TVM_GETCOUNT, 0, 0);
	}

	uInt TreeView::GetIndent() const {
		return _SendMessage(TVM_GETINDENT, 0, 0);
	}

	ImageList TreeView::GetImageList(const Bool& imageListIsState) const {
		const HIMAGELIST hList = reinterpret_cast<HIMAGELIST>(_SendMessage(TVM_GETIMAGELIST, imageListIsState, 0));
		return ImageList(hList);
	}

	TextEdit TreeView::GetTextEdit() const {
		const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(TVM_GETEDITCONTROL, 0, 0));
		return TextEdit(hwnd);
	}

	uInt TreeView::GetVisibleCount() const {
		return _SendMessage(TVM_GETVISIBLECOUNT, 0, 0);
	}

	Tooltip TreeView::GetToolTips() const {
		const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(TVM_GETTOOLTIPS, 0, 0));
		return Tooltip(hwnd);
	}

	std::wstring TreeView::GetSearchString() const {
		const uInt length = _SendMessage(TVM_GETISEARCHSTRING, 0, 0);
		std::wstring searchString(L"\0", length);
		TreeView_GetISearchString(m_hwnd, searchString.data());
		return searchString;
	}

	uInt TreeView::GetItemHeight() const {
		return _SendMessage(TVM_GETITEMHEIGHT, 0, 0);
	}

	uInt TreeView::GetScrollTime() const {
		return _SendMessage(TVM_GETSCROLLTIME, 0, 0);
	}

	Byte3D TreeView::GetInsertMarkColor() const {
		return ColorRefToVector(_SendMessage(TVM_GETINSERTMARKCOLOR, 0, 0));
	}

	TreeView::Item::States TreeView::GetItemState(const ItemID& itemID, const Item::States& stateMask) const {
		return Item::States(_SendMessage(TVM_GETITEMSTATE, reinterpret_cast<uInt>(itemID), stateMask));
	}

	Bool TreeView::GetCheckState(const ItemID& itemID) const {
		const uInt result = _SendMessage(TVM_GETITEMSTATE, reinterpret_cast<uInt>(itemID), TVIS_STATEIMAGEMASK);
		return (result >> 12) - 1;
	}

	Byte3D TreeView::GetLineColor() const {
		return ColorRefToVector(_SendMessage(TVM_GETLINECOLOR, 0, 0));
	}

	TreeView::ExtendStyles TreeView::GetExtendedStyle() const {
		return ExtendStyles(_SendMessage(TVM_GETEXTENDEDSTYLE, 0, 0));
	}

	uInt TreeView::GetSelectedCount() const {
		return _SendMessage(TVM_GETSELECTEDCOUNT, 0, 0);
	}

	IntRect TreeView::GetItemPartRect(const ItemID& itemID) const {
		TVGETITEMPARTRECTINFO info;
		info.hti = itemID;
		info.partID = TVGIPR_BUTTON;

		RECT rect;
		info.prc = &rect;
		_SendMessage(TVM_GETITEMPARTRECT, 0, reinterpret_cast<Long>(&info));
		return rect;
	}

	Bool TreeView::IsUseUnicode() const {
		return _SendMessage(TVM_GETUNICODEFORMAT, 0, 0);
	}

	const std::vector<TreeView::ItemID>& TreeView::GetSelectedItems() const noexcept {
		return m_SelectedTreeItems;
	}

	Bool TreeView::IsRenamingEnabled() const noexcept {
		return m_IsRenammingEnabled;
	}

	void TreeView::SetBackgroundColor(const Byte3D& color) {
		if (m_BackgroundColor == color)
			return;

		m_BackgroundColor = color;
		if (m_hwnd != nullptr && !TreeView_SetBkColor(m_hwnd, VectorToColorRef(color)))
			Log::Instance().Warning("Failed to change background color");
	}

	void TreeView::SetTextColor(const Byte3D& color) {
		if (m_TextColor == color)
			return;

		m_TextColor = color;
		if (m_hwnd != nullptr && !TreeView_SetTextColor(m_hwnd, VectorToColorRef(color)))
			Log::Instance().Warning("Failed to change text color");
	}

	void TreeView::SetItemState(const ItemID& itemID, const Item::States& state, const Item::States& stateMask) {
		RequireNotNull(itemID);

		Item item = { };
		item.Mask = TreeView::Item::Masks(TreeView::Item::MASK_STATE);
		item.ID = itemID;
		if (!GetItem(&item))
			Raise("The passed itemID does not belong to the TreeView");

		if (stateMask == Item::STATE_SELECTED) {
			auto selectedItemIterator = std::find(
				m_SelectedTreeItems.begin(), m_SelectedTreeItems.end(), itemID);
			if (state == Item::STATE_SELECTED)
				m_SelectedTreeItems.push_back(itemID);
			else if (selectedItemIterator != m_SelectedTreeItems.end())
				m_SelectedTreeItems.erase(selectedItemIterator);
		}

		TVITEM tvItem = { };
		tvItem.mask = TVIF_STATE;
		tvItem.hItem = itemID;
		tvItem.stateMask = stateMask;
		tvItem.state = state;
		_SendMessage(TVM_SETITEM, 0, reinterpret_cast<Long>(&tvItem));
	}

	Bool TreeView::SetItem(Item* pItem) {
		TVITEM tvItem = RequireNotNull(pItem)->ToWinApiStruct();
		const Bool result = _SendMessage(TVM_SETITEM, 0, reinterpret_cast<Long>(&tvItem));
		(*pItem) = tvItem;
		return result;
	}

	ImageList TreeView::SetImageList(const ImageList& imageList, const Bool& isImageListState) {
		const Long lParam = reinterpret_cast<Long>(imageList.GetHandle());
		const uInt style = (isImageListState) ? TVSIL_STATE : TVSIL_NORMAL;

		const HIMAGELIST hList =
			reinterpret_cast<HIMAGELIST>(TreeView_SetImageList(m_hwnd, lParam, style));
		return ImageList(hList);
	}

	HRESULT TreeView::SetExtendedStyle(const ExtendStyles& styles, const ExtendStyles& mask) {
		m_TreeExStyles = ExtendStyles(m_TreeExStyles & ~mask);
		m_TreeExStyles = ExtendStyles(m_TreeExStyles | styles);
		return TreeView_SetExtendedStyle(m_hwnd, uInt(styles), Long(mask));
	}

	Bool TreeView::SetIndent(const uInt& indent) {
		return _SendMessage(TVM_SETINDENT, indent, 0);
	}

	void TreeView::SetToolTips(const Tooltip& tooltip) {
		_SendMessage(TVM_SETTOOLTIPS, reinterpret_cast<uInt>(tooltip.GetHandle()), 0);
	}

	void TreeView::ToggleUnicode(const Bool& isUseEnicode) {
		_SendMessage(TVM_SETUNICODEFORMAT, isUseEnicode, 0);
	}

	void TreeView::SetItemHeight(const uInt& height) {
		_SendMessage(TVM_SETITEMHEIGHT, height, 0);
	}

	void TreeView::SetScrollTime(const Int& scrollTime) {
		_SendMessage(TVM_SETSCROLLTIME, scrollTime, 0);
	}

	Byte3D TreeView::SetInsertMarkColor(const Byte3D& color) {
		return ColorRefToVector(_SendMessage(TVM_SETINSERTMARKCOLOR, 0, VectorToColorRef(color)));
	}

	void TreeView::SetCheckState(const ItemID& itemID, const Bool& isChecked) {
		const Item::States value = Item::States(INDEXTOSTATEIMAGEMASK(uInt(isChecked) + 1));
		SetItemState(itemID, value, Item::STATE_STATEIMAGEMASK);
	}

	Byte3D TreeView::SetLineColor(const Byte3D& color) {
		return ColorRefToVector(_SendMessage(TVM_SETLINECOLOR, 0, VectorToColorRef(color)));
	}

	void TreeView::SetAutoScrollInfo(const uInt& offsetPerSecond, const uInt& updateTime) {
		_SendMessage(TVM_SETAUTOSCROLLINFO, offsetPerSecond, updateTime);
	}

	void TreeView::_HandleSelectItem(ItemID hSelectedItem) {
		if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
			for (ItemID& itemID : m_SelectedTreeItems)
				TreeView_SetItemState(m_hwnd, itemID, 0, TVIS_SELECTED);
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

		for (ItemID& itemID : m_SelectedTreeItems) {
			tvItem.hItem = itemID;
			TreeView_GetItem(m_hwnd, &tvItem);

			tvItem.state = TVIS_SELECTED;
			TreeView_SetItem(m_hwnd, &tvItem);
		}
	}

	void TreeView::_HandleDeleteItem() {
		TVITEM tvItem = { };
		tvItem.mask = TVIF_PARAM | TVIF_STATE;
		tvItem.stateMask = TVIS_SELECTED;

		for (ItemID& itemID : m_SelectedTreeItems) {
			tvItem.hItem = itemID;
			TreeView_GetItem(m_hwnd, &tvItem);

			if (tvItem.lParam != 0)
				tvItem.lParam = 0;

			tvItem.state = TVIS_SELECTED;
			TreeView_SetItem(m_hwnd, &tvItem);
		}
		m_SelectedTreeItems.clear();
	}


	TreeView::Item::Item(const TVITEM& tvItem) :
		Mask(Masks(tvItem.mask)),
		ID(tvItem.hItem),
		State(States(tvItem.state)),
		StateMask(States(tvItem.stateMask)),
		Text(tvItem.pszText, tvItem.cchTextMax),
		ImageID(tvItem.iImage),
		SelectedImageID(tvItem.iSelectedImage),
		Children(Childrens(tvItem.cChildren)),
		Data(tvItem.lParam) 
	{
	}

	TVITEM TreeView::Item::ToWinApiStruct() const noexcept {
		TVITEM tvItem = { };
		tvItem.mask = Mask;
		tvItem.hItem = ID;
		tvItem.state = State;
		tvItem.stateMask = StateMask;
		tvItem.pszText = const_cast<wChar*>(Text.c_str());
		tvItem.cchTextMax = Text.size();
		tvItem.iImage = ImageID;
		tvItem.iSelectedImage = SelectedImageID;
		tvItem.cChildren = Children;
		tvItem.lParam = Data;
		return tvItem;
	}

}