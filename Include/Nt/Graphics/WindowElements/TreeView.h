#pragma once

namespace Nt {
	class TreeView : public HandleWindow {
	public:
		using ItemID = HTREEITEM;

		inline static const ItemID FirstID = TVI_FIRST;
		inline static const ItemID LastID = TVI_LAST;
		inline static const ItemID RootID = TVI_ROOT;

	public:
		struct Item {
			enum Masks : uInt {
				MASK_NONE = 0,
				MASK_TEXT = TVIF_TEXT,
				MASK_IMAGE = TVIF_IMAGE,
				MASK_DATA = TVIF_PARAM,
				MASK_STATE = TVIF_STATE,
				MASK_HANDLE = TVIF_HANDLE,
				MASK_SELECTEDIMAGE = TVIF_SELECTEDIMAGE,
				MASK_CHILDREN = TVIF_CHILDREN,
				MASK_INTEGRAL = TVIF_INTEGRAL,

#if (_WIN32_IE >= 0x0600)
				MASK_STATEEX = TVIF_STATEEX,
				MASK_EXPANDEDIMAGE = TVIF_EXPANDEDIMAGE,
				MASK_ALL = (MASK_TEXT | MASK_IMAGE | MASK_DATA | MASK_STATE |
				MASK_SELECTEDIMAGE | MASK_HANDLE | MASK_CHILDREN | MASK_INTEGRAL |
					MASK_STATEEX | MASK_EXPANDEDIMAGE),
#else									  
				MASK_ALL = (MASK_TEXT | MASK_IMAGE | MASK_DATA | MASK_STATE |
				MASK_SELECTEDIMAGE | MASK_HANDLE | MASK_CHILDREN | MASK_INTEGRAL),
#endif									  
			};
			enum States : uInt {
				STATE_NONE = 0,
				STATE_SELECTED = TVIS_SELECTED,
				STATE_CUT = TVIS_CUT,
				STATE_DROPHILITED = TVIS_DROPHILITED,
				STATE_BOLD = TVIS_BOLD,
				STATE_EXPANDED = TVIS_EXPANDED,
				STATE_EXPANDEDONCE = TVIS_EXPANDEDONCE,
				STATE_EXPANDPARTIAL = TVIS_EXPANDPARTIAL,
				STATE_OVERLAYMASK = TVIS_OVERLAYMASK,
				STATE_STATEIMAGEMASK = TVIS_STATEIMAGEMASK,
				STATE_USERMASK = TVIS_USERMASK,

#if (_WIN32_IE >= 0x0600)
				EX_FLAT = TVIS_EX_FLAT,
#	if (NTDDI_VERSION >= NTDDI_VISTA)
				EX_DISABLED = TVIS_EX_DISABLED,
#	endif
				EX_ALL = TVIS_EX_ALL,
#endif
			};
			enum Childrens : Int {
				CHILDREN_NO_ONE = 0,
				CHILDREN_ONE_OR_MORE = 1,
				CHILDREN_CALLBACK = I_CHILDRENCALLBACK,
				CHILDREN_AUTO = I_CHILDRENAUTO,
			};

			Item() = default;
			Item(const TVITEM& tvItem) :
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

			TVITEM ToWinApiStruct() const noexcept {
				TVITEM tvItem;
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

			Masks Mask = MASK_NONE;
			ItemID ID = 0;
			States State = STATE_NONE;
			States StateMask = STATE_NONE;
			std::wstring Text;
			Int ImageID = 0;
			Int SelectedImageID = 0;
			Childrens Children = CHILDREN_ONE_OR_MORE;
			Long Data = 0;
		};
		enum ExpandTypes {
			EXPAND_COLLAPSE = TVE_COLLAPSE,
			EXPAND_EXPAND = TVE_EXPAND,
			EXPAND_TOGGLE = TVE_TOGGLE,
			EXPAND_EXPANDPARTIAL = TVE_EXPANDPARTIAL,
			EXPAND_COLLAPSERESET = TVE_COLLAPSERESET,
		};
		enum ExtendStyles {
			EX_STYLE_MULTISELECT = TVS_EX_MULTISELECT,
			EX_STYLE_DOUBLEBUFFER = TVS_EX_DOUBLEBUFFER,
			EX_STYLE_NOINDENTSTATE = TVS_EX_NOINDENTSTATE,
			EX_STYLE_RICHTOOLTIP = TVS_EX_RICHTOOLTIP,
			EX_STYLE_AUTOHSCROLL = TVS_EX_AUTOHSCROLL,
			EX_STYLE_FADEINOUTEXPANDOS = TVS_EX_FADEINOUTEXPANDOS,
			EX_STYLE_PARTIALCHECKBOXES = TVS_EX_PARTIALCHECKBOXES,
			EX_STYLE_EXCLUSIONCHECKBOXES = TVS_EX_EXCLUSIONCHECKBOXES,
			EX_STYLE_DIMMEDCHECKBOXES = TVS_EX_DIMMEDCHECKBOXES,
			EX_STYLE_DRAWIMAGEASYNC = TVS_EX_DRAWIMAGEASYNC,
		};

		struct HitTestInfo {
			enum {
				HITTEST_NONE = 0,
				HITTEST_NOWHERE = TVHT_NOWHERE,
				HITTEST_ONITEMICON = TVHT_ONITEMICON,
				HITTEST_ONITEMLABEL = TVHT_ONITEMLABEL,
				HITTEST_ONITEM = TVHT_ONITEM,
				HITTEST_ONITEMINDENT = TVHT_ONITEMINDENT,
				HITTEST_ONITEMBUTTON = TVHT_ONITEMBUTTON,
				HITTEST_ONITEMRIGHT = TVHT_ONITEMRIGHT,
				HITTEST_ONITEMSTATEICON = TVHT_ONITEMSTATEICON,
				HITTEST_ABOVE = TVHT_ABOVE,
				HITTEST_BELOW = TVHT_BELOW,
				HITTEST_TORIGHT = TVHT_TORIGHT,
				HITTEST_TOLEFT = TVHT_TOLEFT,
			};

			Long2D Point;
			uInt Flags = HITTEST_NONE;
			ItemID ItemID = nullptr;
		};

		using SortFunction = Int(*)(const Item& leftItem, const Item& rightItem, const Long& param);

		struct SortStruct {
			ItemID ItemID;
			SortFunction Function;
			Long Param;
		};

	public:
		TreeView() noexcept :
			m_TextWeight(400)
		{
		}
		TreeView(const IntRect& windowRect) : m_TextWeight(400) {
			Create(windowRect, "");
		}

		void Create(const IntRect& windowRect) {
			Create(windowRect, "");
		}
		void Create(const IntRect& windowRect, const String& name) override {
			m_Name = L"";
			m_ClassName = WC_TREEVIEW;
			m_WindowRect = windowRect;
			m_Styles |= (TVS_HASLINES | WS_CHILD);
			m_pParam = this;

			if (m_hParent == nullptr)
				Raise("Missing parent window");

			InitializeCommonControls();
			_CreateWindow();

			//SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<Long>(this));
			//SetWindowSubclass(m_hwnd, TreeView_ClassProc, 0, 0);

			TreeView_SetExtendedStyle(m_hwnd, m_TreeExStyles, m_TreeExStyles);
			TreeView_EndEditLabelNow(m_hwnd, TRUE);
			TreeView_SetBkColor(m_hwnd, VectorToColorRef(m_BackgroundColor));
			TreeView_SetTextColor(m_hwnd, VectorToColorRef(m_TextColor));

			m_hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
			TreeView_SetImageList(m_hwnd, m_hImageList, TVSIL_NORMAL);
		}

		ItemID Add(const Item& item, const ItemID& parentItemID = RootID, const ItemID& afterItemID = RootID) {
			const std::wstring wItem = item.Text;

			TVINSERTSTRUCT tvInsert = { };
			tvInsert.hInsertAfter = afterItemID;
			tvInsert.hParent = parentItemID;
			tvInsert.item = item.ToWinApiStruct();

			const Long lParam = reinterpret_cast<Long>(&tvInsert);
			ItemID itemID = reinterpret_cast<ItemID>(_SendMessage(TVM_INSERTITEM, 0, lParam));
			if (itemID == nullptr)
				Raise("Failed to add item");
			return itemID;
		}
		Bool Remove(const ItemID& itemID) {
			return _SendMessage(TVM_DELETEITEM, 0, reinterpret_cast<Long>(itemID));
		}
		void RemoveSelectedItems() {
			m_SelectedTreeItems.clear();
			for (ItemID& itemID : m_SelectedTreeItems)
				Remove(itemID);

			_SendMessage(TVM_SELECTITEM, TVGN_CARET, 0);
		}
		Bool Clear() {
			return _SendMessage(TVM_DELETEITEM, 0, reinterpret_cast<Long>(TVI_ROOT));
		}

		void Select(const ItemID& itemID) {
			_SendMessage(TVM_SELECTITEM, TVGN_CARET, reinterpret_cast<Long>(itemID));
			SetItemState(itemID, Item::STATE_SELECTED, Item::STATE_SELECTED);
		}
		void Deselect(const ItemID& itemID) {
			_SendMessage(TVM_SELECTITEM, TVGN_CARET, 0);
			SetItemState(itemID, Item::STATE_NONE, Item::STATE_SELECTED);
		}
		void DeselectAll() {
			if (!IsCreated())
				Raise("TreeView is not created");

			ItemID itemID = reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_ROOT, 0));
			while (itemID != nullptr) {
				TreeView_SetItemState(m_hwnd, itemID, 0, TVIS_SELECTED);
				itemID = TreeView_GetNextItem(m_hwnd, itemID, TVGN_NEXT);
			}
			TreeView_SelectItem(m_hwnd, nullptr);
		}

		HRESULT AddTreeExStyles(const ExtendStyles& styles) noexcept {
			return SetExtendedStyle(m_TreeExStyles, m_TreeExStyles);
		}
		HRESULT RemodeTreeExStyles(const ExtendStyles& styles) noexcept {
			return SetExtendedStyle(styles, m_TreeExStyles);
		}

		HitTestInfo HitTest(const Int2D& point) {
			HitTestInfo info = { };
			info.Point = point;
			_SendMessage(TVM_HITTEST, 0, reinterpret_cast<Long>(&point));
			return info;
		}
		Bool Expand(const ItemID& itemID, const ExpandTypes& type) {
			return _SendMessage(TVM_EXPAND, type, reinterpret_cast<Long>(itemID));
		}
		TextEdit BeginEditLabel(const ItemID& itemID) {
			const Long lParam = reinterpret_cast<Long>(itemID);
			const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(TVM_EDITLABEL, 0, lParam));
			return TextEdit(hwnd);
		}
		Bool EndEditLabelNow(const Bool& cancelWithoutSaving) {
			return _SendMessage(TVM_ENDEDITLABELNOW, cancelWithoutSaving, 0);
		}

		ImageList CreateDragImage(const ItemID& itemID) {
			const Long lParam = reinterpret_cast<Long>(itemID);
			const HIMAGELIST hList = reinterpret_cast<HIMAGELIST>(_SendMessage(TVM_CREATEDRAGIMAGE, 0, lParam));
			return ImageList(hList);
		}
		Bool SelectDropTarget(const ItemID& itemID) {
			return _SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE, reinterpret_cast<Long>(itemID));
		}
		Bool SelectSetFirstVisible(const ItemID& itemID) {
			return _SendMessage(TVM_SELECTITEM, TVGN_FIRSTVISIBLE, reinterpret_cast<Long>(itemID));
		}

		Bool SortChildren(const ItemID& itemID, const Bool& recurse) {
			return _SendMessage(TVM_SORTCHILDREN, recurse, reinterpret_cast<Long>(itemID));
		}
		Bool SortChildren(const ItemID& itemID, const SortFunction& sortFunction, const Long& param, const Bool& recurse) {
			SortStruct sortStruct;
			sortStruct.ItemID = itemID;
			sortStruct.Function = sortFunction;
			sortStruct.Param = param;
			return _SendMessage(TVM_SORTCHILDRENCB, recurse, reinterpret_cast<Long>(&sortStruct));
		}

		Bool EnsureVisible(const ItemID& itemID) {
			return _SendMessage(TVM_ENSUREVISIBLE, 0, reinterpret_cast<Long>(itemID));
		}
		Int ShowInfoTip(const ItemID& itemID) {
			return _SendMessage(TVM_SHOWINFOTIP, 0, reinterpret_cast<Long>(itemID));
		}

		void HandleNotify(NMHDR* pNMHDR) {
			if (pNMHDR) {
				switch (pNMHDR->code) {
				case TVN_SELCHANGED: {
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
				case TVN_DELETEITEM: {
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

		ItemID GetRootItem() const {
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_ROOT, 0));
		}
		ItemID GetNextItem(const ItemID& itemID) const {
			const Long lParam = reinterpret_cast<Long>(itemID);
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_NEXT, lParam));
		}
		ItemID GetPrevItem(const ItemID& itemID) const {
			const Long lParam = reinterpret_cast<Long>(itemID);
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_PREVIOUS, lParam));
		}
		ItemID GetNextParent(const ItemID& childID) const {
			const Long lParam = reinterpret_cast<Long>(childID);
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_PARENT, lParam));
		}
		ItemID GetNextChild(const ItemID& parentID) const {
			const Long lParam = reinterpret_cast<Long>(parentID);
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_CHILD, lParam));
		}
		ItemID GetFirstVisible() const {
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0));
		}
		ItemID GetNextVisible(const ItemID& itemID) const {
			const Long lParam = reinterpret_cast<Long>(itemID);
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, lParam));
		}
		ItemID GetPrevVisible(const ItemID& itemID) const {
			const Long lParam = reinterpret_cast<Long>(itemID);
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, lParam));
		}
		ItemID GetLastVisible() const {
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_LASTVISIBLE, 0));
		}
		ItemID GetDropHilight() const {
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_DROPHILITE, 0));
		}
		ItemID GetNextSelected(const ItemID& itemID) const {
			const Long lParam = reinterpret_cast<Long>(itemID);
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_NEXTSELECTED, lParam));
		}
		ItemID GetSelection() const {
			return reinterpret_cast<ItemID>(_SendMessage(TVM_GETNEXTITEM, TVGN_CARET, 0));
		}
		Bool GetItem(Item* pItem) const {
			if (pItem == nullptr)
				Raise("The item pointer passed is null");

			TVITEM tvItem = pItem->ToWinApiStruct();
			const Bool result = _SendMessage(TVM_GETITEM, 0, reinterpret_cast<Long>(&tvItem));
			(*pItem) = tvItem;
			return result;
		}
		IntRect GetItemRect(const ItemID& itemID, const Bool& isTextRect) const {
			void* pData = itemID;
			_SendMessage(TVM_GETITEMRECT, isTextRect, reinterpret_cast<Long>(pData));
			if (pData == nullptr)
				return IntRect();
			return *reinterpret_cast<RECT*>(pData);
		}
		uInt GetCount() const {
			return _SendMessage(TVM_GETCOUNT, 0, 0);
		}
		uInt GetIndent() const {
			return _SendMessage(TVM_GETINDENT, 0, 0);
		}
		ImageList GetImageList(const Bool& imageListIsState) const {
			const HIMAGELIST hList = reinterpret_cast<HIMAGELIST>(_SendMessage(TVM_GETIMAGELIST, imageListIsState, 0));
			return ImageList(hList);
		}
		TextEdit GetTextEdit() const {
			const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(TVM_GETEDITCONTROL, 0, 0));
			return TextEdit(hwnd);
		}
		uInt GetVisibleCount() const {
			return _SendMessage(TVM_GETVISIBLECOUNT, 0, 0);
		}
		Tooltip GetToolTips() const {
			const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(TVM_GETTOOLTIPS, 0, 0));
			return Tooltip(hwnd);
		}
		std::wstring GetSearchString() const {
			const uInt length = _SendMessage(TVM_GETISEARCHSTRING, 0, 0);
			std::wstring searchString(L'\0', length);
			TreeView_GetISearchString(m_hwnd, searchString.data());
			return searchString;
		}
		uInt GetItemHeight() const {
			return _SendMessage(TVM_GETITEMHEIGHT, 0, 0);
		}
		uInt GetScrollTime() const {
			return _SendMessage(TVM_GETSCROLLTIME, 0, 0);
		}
		Byte3D GetInsertMarkColor() const {
			return ColorRefToVector(_SendMessage(TVM_GETINSERTMARKCOLOR, 0, 0));
		}
		Item::States GetItemState(const ItemID& itemID, const Item::States& stateMask) const {
			return Item::States(_SendMessage(TVM_GETITEMSTATE, reinterpret_cast<uInt>(itemID), stateMask));
		}
		Bool GetCheckState(const ItemID& itemID) const {
			const uInt result = _SendMessage(TVM_GETITEMSTATE, reinterpret_cast<uInt>(itemID), TVIS_STATEIMAGEMASK);
			return (result >> 12) - 1;
		}
		Byte3D GetLineColor() const {
			return ColorRefToVector(_SendMessage(TVM_GETLINECOLOR, 0, 0));
		}
		ExtendStyles GetExtendedStyle() const {
			return ExtendStyles(_SendMessage(TVM_GETEXTENDEDSTYLE, 0, 0));
		}
		uInt GetSelectedCount() const {
			return _SendMessage(TVM_GETSELECTEDCOUNT, 0, 0);
		}
		IntRect GetItemPartRect(const ItemID& itemID) const {
			TVGETITEMPARTRECTINFO info;
			info.hti = itemID;
			info.partID = TVGIPR_BUTTON;

			RECT rect;
			info.prc = &rect;
			_SendMessage(TVM_GETITEMPARTRECT, 0, reinterpret_cast<Long>(&info));
			return rect;
		}
		Bool IsUseUnicode() const {
			return _SendMessage(TVM_GETUNICODEFORMAT, 0, 0);
		}
		const std::vector<ItemID>& GetSelectedItems() const noexcept {
			return m_SelectedTreeItems;
		}
		Bool IsRenamingEnabled() const noexcept {
			return m_IsRenammingEnabled;
		}

		void SetBackgroundColor(const Byte3D& color) {
			if (m_BackgroundColor == color)
				return;

			if (m_hwnd) {
				m_BackgroundColor = ColorRefToVector(TreeView_SetBkColor(m_hwnd, VectorToColorRef(color)));
				if (m_BackgroundColor != color)
					Log::Warning("Failed to change background color");
			}
			else {
				m_BackgroundColor = color;
			}
		}
		void SetTextColor(const Byte3D& color) {
			if (m_TextColor == color)
				return;

			if (m_hwnd) {
				m_TextColor = ColorRefToVector(TreeView_SetTextColor(m_hwnd, VectorToColorRef(color)));
				if (m_TextColor != color)
					Log::Warning("Failed to change text color");
			}
			else {
				m_TextColor = color;
			}
		}
		void SetItemState(const ItemID& itemID, const Item::States& state, const Item::States& stateMask) {
			if (itemID == nullptr)
				Raise("The ItemID passed is null");

			Item item;
			item.Mask = Nt::TreeView::Item::Masks(Nt::TreeView::Item::MASK_STATE);
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

			TVITEM tvItem;
			tvItem.mask = TVIF_STATE;
			tvItem.hItem = itemID;
			tvItem.stateMask = stateMask;
			tvItem.state = state;
			_SendMessage(TVM_SETITEM, 0, reinterpret_cast<Long>(&tvItem));
		}
		Bool SetItem(Item* pItem) {
			if (pItem == nullptr)
				Raise("The item pointer passed is null");
			
			TVITEM tvItem = pItem->ToWinApiStruct();
			const Bool result = _SendMessage(TVM_SETITEM, 0, reinterpret_cast<Long>(&tvItem));
			(*pItem) = tvItem;
			return result;
		}
		ImageList SetImageList(const ImageList& imageList, const Bool& isImageListState) {
			const Long lParam = reinterpret_cast<Long>(imageList.GetHandle());
			const uInt style = (isImageListState) ? TVSIL_STATE : TVSIL_NORMAL;

			const HIMAGELIST hList = 
				reinterpret_cast<HIMAGELIST>(TreeView_SetImageList(m_hwnd, lParam, style));
			return ImageList(hList);
		}
		HRESULT SetExtendedStyle(const ExtendStyles& styles, const ExtendStyles& mask) {
			m_TreeExStyles = ExtendStyles(m_TreeExStyles & ~mask);
			m_TreeExStyles = ExtendStyles(m_TreeExStyles | styles);
			return TreeView_SetExtendedStyle(m_hwnd, uInt(styles), Long(mask));
		}
		Bool SetIndent(const uInt& indent) {
			return _SendMessage(TVM_SETINDENT, indent, 0);
		}
		void SetToolTips(const Tooltip& tooltip) {
			_SendMessage(TVM_SETTOOLTIPS, reinterpret_cast<uInt>(tooltip.GetHandle()), 0);
		}
		void ToggleUnicode(const Bool& isUseEnicode) {
			_SendMessage(TVM_SETUNICODEFORMAT, isUseEnicode, 0);
		}
		void SetItemHeight(const uInt& height) {
			_SendMessage(TVM_SETITEMHEIGHT, height, 0);
		}
		void SetScrollTime(const Int& scrollTime) {
			_SendMessage(TVM_SETSCROLLTIME, scrollTime, 0);
		}
		Byte3D SetInsertMarkColor(const Byte3D& color) {
			return ColorRefToVector(_SendMessage(TVM_SETINSERTMARKCOLOR, 0, VectorToColorRef(color)));
		}
		void SetCheckState(const ItemID& itemID, const Bool& isChecked) {
			const Item::States value = Item::States(INDEXTOSTATEIMAGEMASK(uInt(isChecked) + 1));
			SetItemState(itemID, value, Item::STATE_STATEIMAGEMASK);
		}
		Byte3D SetLineColor(const Byte3D& color) {
			return ColorRefToVector(_SendMessage(TVM_SETLINECOLOR, 0, VectorToColorRef(color)));
		}
		void SetAutoScrollInfo(const uInt& offsetPerSecond, const uInt& updateTime) {
			_SendMessage(TVM_SETAUTOSCROLLINFO, offsetPerSecond, updateTime);
		}

	private:
		std::vector<ItemID> m_SelectedTreeItems;
		HIMAGELIST m_hImageList;
		Byte3D m_TextColor;
		ExtendStyles m_TreeExStyles;
		String m_Text;
		uInt m_TextWeight;
		Bool m_IsRenammingEnabled = false;

	private:
		void _HandleSelectItem(ItemID hSelectedItem) {
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
		void _HandleDeleteItem() {
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
	};
}