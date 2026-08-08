#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOMSG

#include <windows.h>

#include <Nt/Graphics/System/WindowElements/ListView.h>

namespace Nt {
	Bool ListView::GetItem(Item* pItem) const {
		LVITEM lvItem = RequireNotNull(pItem)->ToWinApiStruct();
		const Bool result = _SendMessage(LVM_GETITEM, 0, reinterpret_cast<Long>(&lvItem));

		(*pItem) = lvItem;
		return result;
	}

	Nt::IntRect ListView::GetItemRect(const uInt& itemIndex, const ItemRectTypes& type) const {
		RECT itemRect = { };
		itemRect.left = Long(type);
		_SendMessage(LVM_GETITEMRECT, itemIndex, reinterpret_cast<Long>(&itemRect));
		return itemRect;
	}

	Nt::IntRect ListView::GetItemRectFromGroup(const uInt& itemIndex, const Int& subItemIndex, const uInt& goupID, const ItemRectTypes& type) const {
		LVITEMINDEX lvItemIndex;
		lvItemIndex.iGroup = goupID;
		lvItemIndex.iItem = itemIndex;
		const uInt wParam = reinterpret_cast<uInt>(&lvItemIndex);

		RECT itemRect = { };
		itemRect.left = Long(type);
		itemRect.top = subItemIndex;
		const uInt lParam = reinterpret_cast<Long>(&itemRect);

		_SendMessage(LVM_GETITEMINDEXRECT, wParam, lParam);
		return itemRect;
	}

	Nt::ListView::Item::States ListView::GetItemState(const uInt& itemIndex, const Item::States& stateMask) const {
		return Item::States(_SendMessage(LVM_GETITEMSTATE, itemIndex, stateMask));
	}

	Nt::String ListView::GetItemText(const uInt& itemIndex, const uInt& subItemIndex) const {
		if (m_Styles & ListStyles::LISTSTYLE_OWNERDATA) {
			Log::Instance().Warning("ListView::GetItemText not supported under the LISTSTYLE_OWNERDATA style");
			return "";
		}

		LV_ITEM item = { };
		item.iSubItem = subItemIndex;

		uInt textLength = _SendMessage(LVM_GETITEMTEXT, itemIndex, reinterpret_cast<Long>(&item));
		std::wstring text(textLength, L'0');

		item.cchTextMax = textLength;
		item.pszText = text.data();
		_SendMessage(LVM_GETITEMTEXT, itemIndex, reinterpret_cast<Long>(&item));
		return text;
	}

	Nt::Int2D ListView::GetItemSpacing(const Bool& isSmallIcon) const {
		const DWord spacing = _SendMessage(LVM_GETITEMSPACING, isSmallIcon, 0);
		return Int2D(LOWORD(spacing), HIWORD(spacing));
	}

	Bool ListView::GetItemPosition(const uInt& itemIndex, Long2D* pPosition) const {
		return _SendMessage(LVM_GETITEMPOSITION, itemIndex, reinterpret_cast<Long>(pPosition));
	}

	Nt::ListView::Item::States ListView::GetCallbackMask() const {
		return Item::States(_SendMessage(LVM_GETCALLBACKMASK, 0, 0));
	}

	Int ListView::GetNextItem(const Int& itemIndex, const NextType& type) const {
		return _SendMessage(LVM_GETNEXTITEM, itemIndex, Long(type));
	}

	Int ListView::GetStringWidth(const std::wstring& text) const {
		return _SendMessage(LVM_GETSTRINGWIDTH, 0, reinterpret_cast<Long>(text.data()));
	}

	Nt::TextEdit ListView::GetTextEdit() const {
		const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(LVM_GETEDITCONTROL, 0, 0));
		return TextEdit(hwnd);
	}

	Bool ListView::GetColumn(const Int& columnIndex, Column* pColumn) const {
		return _SendMessage(LVM_GETCOLUMN, columnIndex, reinterpret_cast<Long>(pColumn));
	}

	Int ListView::GetColumnWidth(const Int& columnIndex) const {
		return _SendMessage(LVM_GETCOLUMNWIDTH, columnIndex, 0);
	}

	Nt::IntRect ListView::GetViewRect() const {
		RECT rect = { };
		_SendMessage(LVM_GETVIEWRECT, 0, reinterpret_cast<Long>(&rect));
		return rect;
	}

	Int ListView::GetTopIndex() const {
		return _SendMessage(LVM_GETTOPINDEX, 0, 0);
	}

	Int ListView::GetCountPerPage() const {
		return _SendMessage(LVM_GETCOUNTPERPAGE, 0, 0);
	}

	Bool ListView::GetOrigin(Int2D* pOrigin) const {
		return _SendMessage(LVM_GETORIGIN, 0, reinterpret_cast<Long>(pOrigin));
	}

	Bool ListView::GetCheckState(const uInt& itemIndex) const {
		return (_SendMessage(LVM_GETITEMSTATE, itemIndex, LVIS_STATEIMAGEMASK) >> 12) - 1;
	}

	Int ListView::GetSelectedCount() const {
		return _SendMessage(LVM_GETSELECTEDCOUNT, 0, 0);
	}

	std::wstring ListView::GetSearchString() const {
		const uInt length = _SendMessage(LVM_GETISEARCHSTRING, 0, 0);
		std::wstring searchString(L"\0", length);
		ListView_GetISearchString(m_hwnd, searchString.data());
		return searchString;
	}

	Nt::ListView::ExtendedStyles ListView::GetExtendedStyle() const {
		return ExtendedStyles(_SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0));
	}

	Nt::IntRect ListView::GetSubItemRect(const Int& itemIndex, const Int& subItemIndex, const ItemRectTypes& type) const {
		RECT rect = { };
		rect.left = Long(type);
		rect.top = subItemIndex;
		_SendMessage(LVM_GETSUBITEMRECT, itemIndex, reinterpret_cast<Long>(&rect));
		return rect;
	}

	Bool ListView::GetColumnOrderArray(const uInt& columnCount, Int* columnIndexArray) const {
		return _SendMessage(LVM_GETCOLUMNORDERARRAY, columnCount, reinterpret_cast<Long>(columnIndexArray));
	}

	Int ListView::GetHotItem() const {
		return _SendMessage(LVM_GETHOTITEM, 0, 0);
	}

	void ListView::GetWorkAreas(const Int& arraySize, LongRect* rectArray) const {
		_SendMessage(LVM_GETWORKAREAS, arraySize, reinterpret_cast<Long>(rectArray));
	}

	uInt ListView::GetNumberOfWorkAreas() const {
		uInt count;
		_SendMessage(LVM_GETNUMBEROFWORKAREAS, 0, reinterpret_cast<Long>(&count));
		return count;
	}

	Int ListView::GetSelectionMark() const {
		return _SendMessage(LVM_GETSELECTIONMARK, 0, 0);
	}

	uInt ListView::GetMouseHoverTime() const {
		return _SendMessage(LVM_GETHOVERTIME, 0, 0);
	}

	Nt::Tooltip ListView::GetToolTips() const {
		const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(LVM_GETTOOLTIPS, 0, 0));
		return Tooltip(hwnd);
	}

	Nt::ListView::ViewTypes ListView::GetView() const {
		return ViewTypes(_SendMessage(LVM_GETVIEW, 0, 0));
	}

	Bool ListView::GetGroup(const uInt& groupID, Group* pGroup) const {
		return (_SendMessage(LVM_GETGROUPINFO, groupID, reinterpret_cast<Long>(pGroup)) != -1);
	}

	Int ListView::GetGroupCount() const {
		return _SendMessage(LVM_GETGROUPCOUNT, 0, 0);
	}

	Bool ListView::GetGroupByIndex(const uInt& groupID, Group* pGroup) const {
		return _SendMessage(LVM_GETGROUPINFOBYINDEX, groupID, reinterpret_cast<Long>(pGroup));
	}

	Nt::IntRect ListView::GetGroupRect(const uInt& groupID, const GroupRectTypes& type) const {
		RECT rect = { };
		rect.top = Long(type);
		_SendMessage(LVM_GETGROUPRECT, groupID, reinterpret_cast<Long>(&rect));
		return rect;
	}

	Nt::ListView::GroupMetrics ListView::GetGroupMetrics() const {
		LVGROUPMETRICS metrics;
		metrics.cbSize = sizeof(metrics);
		_SendMessage(LVM_GETGROUPMETRICS, 0, reinterpret_cast<Long>(&metrics));
		return metrics;
	}

	Nt::ListView::Group::States ListView::GetGroupState(const uInt& groupID, const Group::States& mask) const {
		return Group::States(_SendMessage(LVM_GETGROUPSTATE, groupID, Long(mask)));
	}

	uInt ListView::GetFocusedGroup() const {
		return _SendMessage(LVM_GETFOCUSEDGROUP, 0, 0);
	}

	Nt::ListView::TileViewInfo ListView::GetTileViewInfo() const {
		LVTILEVIEWINFO info;
		info.cbSize = sizeof(info);
		_SendMessage(LVM_GETTILEVIEWINFO, 0, reinterpret_cast<Long>(&info));
		return info;
	}

	Nt::ListView::TileInfo ListView::GetTileInfo() const {
		LVTILEINFO info;
		info.cbSize = sizeof(info);
		_SendMessage(LVM_GETTILEINFO, 0, reinterpret_cast<Long>(&info));
		return info;
	}

	Int ListView::GetInsertMark(const Int& itemIndex, const Bool& isAfter) const {
		LVINSERTMARK lvInsertMask;
		lvInsertMask.cbSize = sizeof(lvInsertMask);
		lvInsertMask.dwFlags = (isAfter) ? LVIM_AFTER : 0;
		lvInsertMask.iItem = itemIndex;
		return _SendMessage(LVM_GETINSERTMARK, 0, reinterpret_cast<Long>(&lvInsertMask));
	}

	Nt::IntRect ListView::GetInsertMarkRect() const {
		RECT rect = { };
		_SendMessage(LVM_GETINSERTMARKRECT, 0, reinterpret_cast<Long>(&rect));
		return rect;
	}

	Nt::Byte3D ListView::GetInsertMarkColor() const {
		return ColorRefToVector(_SendMessage(LVM_GETINSERTMARKCOLOR, 0, 0));
	}

	uInt ListView::GetSelectedColumn() const {
		return _SendMessage(LVM_GETSELECTEDCOLUMN, 0, 0);
	}

	Nt::Byte3D ListView::GetOutlineColor() const {
		return ColorRefToVector(_SendMessage(LVM_GETOUTLINECOLOR, 0, 0));
	}

	Int ListView::HitTest(HitTextInfo* pHitTextInfo) const {
		return _SendMessage(LVM_HITTEST, uInt(-1), reinterpret_cast<Long>(pHitTextInfo));
	}

	Int ListView::SubItemHitTest(HitTextInfo* pHitTextInfo) const {
		return _SendMessage(LVM_SUBITEMHITTEST, uInt(-1), reinterpret_cast<Long>(pHitTextInfo));
	}

	Bool ListView::ToggleVisible(const uInt& listIndex, const Bool& isVisible) const {
		return _SendMessage(LVM_ENSUREVISIBLE, listIndex, MAKELONG(isVisible, 0));
	}

	Bool ListView::Scroll(const Int2D& scrollValue) {
		return _SendMessage(LVM_SCROLL, scrollValue.x, scrollValue.y);
	}

	Bool ListView::Arrange(const ArrangeTypes& type) {
		return _SendMessage(LVM_ARRANGE, type, 0);
	}

	Nt::TextEdit ListView::BeginEditLable(const uInt& listIndex) {
		return TextEdit(reinterpret_cast<HWND>(_SendMessage(LVM_EDITLABEL, listIndex, 0)));
	}

	void ListView::CancelEditLabel() {
		_SendMessage(LVM_CANCELEDITLABEL, 0, 0);
	}

	ImageList ListView::CreateDragImage(const uInt& itemIndex, const Int2D& position) {
		const POINT point = position;
		const Long lParam = reinterpret_cast<Long>(&point);
		const HIMAGELIST hImageList = reinterpret_cast<HIMAGELIST>(_SendMessage(LVM_CREATEDRAGIMAGE, itemIndex, lParam));
		return ImageList(hImageList);
	}

	Bool ListView::InsertMarkHitTest(Long2D* pCoordinates, const Bool& insertAfter, const Int& itemIndex) {
		LVINSERTMARK lvInsertMark;
		lvInsertMark.cbSize = sizeof(lvInsertMark);
		lvInsertMark.dwFlags = (insertAfter) ? LVIM_AFTER : 0;
		lvInsertMark.iItem = itemIndex;

		const uInt wParam = reinterpret_cast<uInt>(pCoordinates);
		const Long lParam = reinterpret_cast<Long>(&lvInsertMark);
		return _SendMessage(LVM_INSERTMARKHITTEST, wParam, lParam);
	}

	Int ListView::ItemIndexToID(const uInt& itemIndex) const {
		return _SendMessage(LVM_MAPINDEXTOID, itemIndex, 0);
	}

	Int ListView::ItemIDToIndex(const uInt& itemID) const {
		return _SendMessage(LVM_MAPIDTOINDEX, itemID, 0);
	}

	Nt::Int2D ListView::GetApproximateSize() const {
		const DWord size = _SendMessage(LVM_APPROXIMATEVIEWRECT, uInt(-1), -1);
		return Int2D(LOWORD(size), HIWORD(size));
	}

	Nt::Byte3D ListView::GetBackgroundColor() const noexcept {
		return m_BackgroundColor;
	}

	Nt::Byte3D ListView::GetTextColor() const noexcept {
		return m_TextColor;
	}

	Nt::Byte3D ListView::GetTextBackgroundColor() const noexcept {
		return m_TextBackgroundColor;
	}

	ImageList ListView::GetImageList() const {
		const HIMAGELIST handle = reinterpret_cast<HIMAGELIST>(_SendMessage(LVM_GETIMAGELIST, LVSIL_GROUPHEADER, 0));
		return ImageList(handle);
	}

	ImageList ListView::GetImageList(const ImageListTypes& type) const {
		const HIMAGELIST handle = reinterpret_cast<HIMAGELIST>(_SendMessage(LVM_GETIMAGELIST, uInt(type), 0));
		return ImageList(handle);
	}

	uInt ListView::GetItemCount() const {
		return _SendMessage(LVM_GETITEMCOUNT, 0, 0);
	}

	std::wstring ListView::GetEmptyText() const {
		wChar buffer[0x7FFF] = { };
		_SendMessage(LVM_GETEMPTYTEXT, 0x7FFF, reinterpret_cast<Long>(buffer));
		return buffer;
	}

	Nt::IntRect ListView::GetFooterRect() const {
		RECT rect = { };
		_SendMessage(LVM_GETFOOTERRECT, 0, reinterpret_cast<Long>(&rect));
		return rect;
	}

	Nt::IntRect ListView::GetFooterItemRect(const uInt& itemIndex) const {
		RECT rect = { };
		_SendMessage(LVM_GETFOOTERITEMRECT, itemIndex, reinterpret_cast<Long>(&rect));
		return rect;
	}

	Bool ListView::IsUseUnicode() const {
		return _SendMessage(LVM_GETUNICODEFORMAT, 0, 0);
	}

	Bool ListView::IsGroupViewEnabled() const {
		return _SendMessage(LVM_ISGROUPVIEWENABLED, 0, 0);
	}

	void ListView::SetItemState(const Int& index, const uInt& styles, const uInt& mask) {
		if (!IsCreated())
			Raise("ListView not created");
		ListView_SetItemState(m_hwnd, index, styles, mask);
	}

	ListView::ListView(const IntRect& windowRect) {
		Create(windowRect);
	}

	void ListView::Create(const IntRect& windowRect, const String& name) {
		m_ClassName = WC_LISTVIEW;
		m_Name = name;
		m_WindowRect = windowRect;
		_CreateWindow();

		SetWindowSubclass(m_hwnd, SubClassProc_ListView, 0, 0);

		ListView_SetExtendedListViewStyleEx(m_hwnd, m_ExtendedStyles, m_ExtendedStyles);
		ListView_SetBkColor(m_hwnd, VectorToColorRef(m_BackgroundColor));
		ListView_SetTextColor(m_hwnd, VectorToColorRef(m_TextColor));

		if (!m_EnabledTextBackground)
			ListView_SetTextBkColor(m_hwnd, CLR_NONE);
		else
			ListView_SetTextBkColor(m_hwnd, VectorToColorRef(m_BackgroundColor));
	}

	void ListView::Create(const IntRect& windowRect) {
		Create(windowRect, "");
	}

	void ListView::AddExtendedStyles(const ExtendedStyles& styles, const ExtendedStyles& mask) noexcept {
		m_ExtendedStyles = ExtendedStyles(m_ExtendedStyles & ~mask);
		m_ExtendedStyles = ExtendedStyles(m_ExtendedStyles | (styles & mask));
		if (m_hwnd)
			ListView_SetExtendedListViewStyleEx(m_hwnd, m_ExtendedStyles, m_ExtendedStyles);
	}

	Int ListView::AddItem(const Item& item) {
		m_Items.push_back(item);
		const LVITEM lvItem = (m_Items.end() - 1)->ToWinApiStruct();
		return _SendMessage(LVM_INSERTITEM, 0, reinterpret_cast<Long>(&lvItem));
	}

	Bool ListView::RemoveItem(const uInt& index) {
		if (index < m_Items.size())
			m_Items.erase(m_Items.begin() + index);
		return _SendMessage(LVM_DELETEITEM, index, 0);
	}

	Bool ListView::ClearItems() {
		m_Items.clear();
		return _SendMessage(LVM_DELETEALLITEMS, 0, 0);
	}

	Bool ListView::AddColumn(const Column& column, const uInt& index) {
		return _SendMessage(LVM_INSERTCOLUMN, index, reinterpret_cast<Long>(&column));
	}

	Bool ListView::RemoveColumn(const uInt& index) {
		return _SendMessage(LVM_DELETECOLUMN, index, 0);
	}

	Int ListView::InsertGroup(const Group& group, const uInt& itemIndex) {
		return _SendMessage(LVM_INSERTGROUP, itemIndex, reinterpret_cast<Long>(&group));
	}

	void ListView::InsertGroupSorted(const SortGroupCompare& function, void* pData, const Group& group) {
		LVINSERTGROUPSORTED insertData;
		insertData.lvGroup = group.ToWinApiStruct();
		insertData.pfnGroupCompare = function;
		insertData.pvData = pData;
		_SendMessage(LVM_INSERTGROUPSORTED, reinterpret_cast<uInt>(&insertData), 0);
	}

	Int ListView::RemoveGroup(const uInt& groupID) {
		return _SendMessage(LVM_REMOVEGROUP, groupID, 0);
	}

	void ListView::RemoveAllGroups() {
		_SendMessage(LVM_REMOVEALLGROUPS, 0, 0);
	}

	void ListView::MoveGroup(const uInt& groupID, const uInt& toIndex) {
		_SendMessage(LVM_MOVEGROUP, groupID, toIndex);
	}

	void ListView::MoveItemToGroup(const uInt& groupIDFrom, const uInt& groupIDTo) {
		_SendMessage(LVM_MOVEITEMTOGROUP, groupIDFrom, groupIDTo);
	}

	Int ListView::EnableGroupView(const Bool& isEnabled) {
		return _SendMessage(LVM_ENABLEGROUPVIEW, isEnabled, 0);
	}

	Bool ListView::SortGroups(const SortGroupCompare& function, void* pParam) {
		return _SendMessage(LVM_SORTGROUPS, reinterpret_cast<uInt>(function), reinterpret_cast<Long>(pParam));
	}

	Bool ListView::HasGroup(const uInt& groupID) const {
		return _SendMessage(LVM_HASGROUP, groupID, 0);
	}

	Bool ListView::Redraw(const uInt& indexFirst, const uInt& indexLast) const {
		return _SendMessage(LVM_REDRAWITEMS, indexFirst, indexLast);
	}

	Bool ListView::Update(const uInt& itemIndex) {
		return _SendMessage(LVM_UPDATE, itemIndex, 0);
	}

	Int ListView::FindItem(const FindFlags& flags, const std::wstring& searchText, const Long& searchData, const Int2D& position, const Key& key, const uInt& startIndex /*= -1*/) const {
		const Bool keyIsLeftOrRightArrow = (key == KEY_LEFT || key == KEY_RIGHT);
		const Bool keyIsUpOrDownArrow = (key == KEY_UP || key == KEY_DOWN);
		const Bool keyIsHomeOrEnd = (key == KEY_HOME || key == KEY_END);
		const Bool keyIsPriorOrNext = (key == KEY_PRIOR || key == KEY_NEXT);

		if (!(keyIsLeftOrRightArrow || keyIsUpOrDownArrow || keyIsHomeOrEnd || keyIsPriorOrNext))
			Log::Instance().Warning("This Key not supported");

		LVFINDINFO findInfo;
		findInfo.flags = flags;
		findInfo.psz = searchText.c_str();
		findInfo.lParam = searchData;
		findInfo.pt = position;
		findInfo.vkDirection = uInt(key);
		return _SendMessage(LVM_FINDITEM, startIndex, reinterpret_cast<Long>(&findInfo));
	}

	void ListView::SortItems(const PFNLVCOMPARE& func, const Long& param) {
		if (!IsCreated())
			Raise("ListView not created");
		ListView_SortItems(m_hwnd, func, param);
	}

	Bool ListView::SortItems(const SortCompare& function, const Long& param) {
		return _SendMessage(LVM_SORTITEMS, param, reinterpret_cast<Long>(function));
	}

	Bool ListView::SortItemsEx(const SortCompare& function, const Long& param) {
		return _SendMessage(LVM_SORTITEMSEX, param, reinterpret_cast<Long>(function));
	}

	void ListView::EnableTextBackground() noexcept {
		if (!m_EnabledTextBackground) {
			m_EnabledTextBackground = true;
			if (m_hwnd)
				ListView_SetTextBkColor(m_hwnd, VectorToColorRef(m_BackgroundColor));
		}
	}

	void ListView::DisableTextBackground() noexcept {
		if (m_EnabledTextBackground) {
			m_EnabledTextBackground = false;
			if (m_hwnd)
				ListView_SetTextBkColor(m_hwnd, CLR_NONE);
		}
	}

	void ListView::SetBackgroundColor(const Byte3D& color) {
		m_BackgroundColor = color;
		if (m_hwnd) {
			Hide();
			ListView_SetBkColor(m_hwnd, VectorToColorRef(m_BackgroundColor));
			Show();
		}
	}

	void ListView::SetTextColor(const Byte3D& color) {
		m_TextColor = color;
		if (m_hwnd) {
			ListView_SetTextColor(m_hwnd, VectorToColorRef(m_TextColor));
			InvalidateRect(nullptr, false);
		}
	}

	void ListView::SetTextBackgroundColor(const Byte3D& color) {
		m_TextBackgroundColor = color;
		if (m_hwnd) {
			ListView_SetTextBkColor(m_hwnd, VectorToColorRef(m_TextBackgroundColor));
			InvalidateRect(nullptr, false);
		}
	}

	ListView::TileInfo::TileInfo(const LVTILEINFO& info) :
		ItemIndex(info.iItem) {
		if (info.cColumns > 0) {
			if (info.puColumns != nullptr) {
				ColumnsIndices.resize(info.cColumns);
				memcpy(ColumnsIndices.data(), info.puColumns, info.cColumns * sizeof(uInt));
			}
			if (info.piColFmt != nullptr) {
				ColumnsFormats.resize(info.cColumns);
				memcpy(ColumnsFormats.data(), info.piColFmt, info.cColumns * sizeof(Int));
			}
		}
	}

	LVTILEINFO ListView::TileInfo::ToWinApiStruct() const noexcept {
		LVTILEINFO info;
		info.cbSize = sizeof(info);
		info.iItem = ItemIndex;
		info.cColumns = ColumnsIndices.size();
		info.puColumns = PUINT(ColumnsIndices.data());
		info.piColFmt = PINT(ColumnsFormats.data());
		return info;
	}

	ListView::TileViewInfo::TileViewInfo(const LVTILEVIEWINFO& info) :
		Mask(Masks(info.dwMask)),
		Flags(SizeFlags(info.dwFlags)),
		Size(info.sizeTile),
		LineCount(info.cLines),
		LabelMargin(info.rcLabelMargin) {

	}

	LVTILEVIEWINFO ListView::TileViewInfo::ToWinApiStruct() const noexcept {
		LVTILEVIEWINFO info;
		info.cbSize = sizeof(info);
		info.dwMask = DWord(Mask);
		info.dwFlags = DWord(Flags);
		info.sizeTile = Size;
		info.cLines = LineCount;
		info.rcLabelMargin = LabelMargin;
		return info;
	}

	ListView::GroupMetrics::GroupMetrics(const LVGROUPMETRICS& metrics) :
		Mask(Masks(metrics.mask)),
		Rect(metrics.Left, metrics.Top, metrics.Right, metrics.Bottom) {

	}

	LVGROUPMETRICS ListView::GroupMetrics::ToWinApiStruct() const noexcept {
		LVGROUPMETRICS metrics = { };
		metrics.cbSize = sizeof(metrics);
		metrics.Left = Rect.Left;
		metrics.Top = Rect.Top;
		metrics.Right = Rect.Right;
		metrics.Bottom = Rect.Bottom;
		return metrics;
	}

	ListView::Group::Group(const LVGROUP& lvGroup) :
		Header(lvGroup.pszHeader),
		Footer(lvGroup.pszFooter),
		Subtitle(lvGroup.pszSubtitle),
		Task(lvGroup.pszTask),
		DescriptionTop(lvGroup.pszDescriptionTop),
		DescriptionBottom(lvGroup.pszDescriptionBottom),
		SubsetTitle(lvGroup.pszSubsetTitle),
		Mask(Masks(lvGroup.mask)),
		GroupID(lvGroup.iGroupId),
		StateMask(States(lvGroup.stateMask)),
		State(States(lvGroup.state)),
		Align(Aligns(lvGroup.uAlign)),
		TitleImageIndex(lvGroup.iTitleImage),
		ExtendedImageIndex(lvGroup.iExtendedImage),
		FirstItemIndex(lvGroup.iFirstItem),
		ItemsCount(lvGroup.cItems) {

	}

	LVGROUP ListView::Group::ToWinApiStruct() const noexcept {
		LVGROUP lvGroup;
		lvGroup.cbSize = sizeof(lvGroup);
		lvGroup.mask = Mask;
		lvGroup.pszHeader = const_cast<wChar*>(Header.c_str());
		lvGroup.cchHeader = Header.length();
		lvGroup.pszFooter = const_cast<wChar*>(Footer.c_str());
		lvGroup.cchFooter = Footer.length();
		lvGroup.iGroupId = GroupID;
		lvGroup.stateMask = StateMask;
		lvGroup.state = State;
		lvGroup.uAlign = Align;
		lvGroup.pszSubtitle = const_cast<wChar*>(Subtitle.c_str());
		lvGroup.cchSubtitle = Subtitle.length();
		lvGroup.pszTask = const_cast<wChar*>(Task.c_str());
		lvGroup.cchTask = Task.length();
		lvGroup.pszDescriptionTop = const_cast<wChar*>(DescriptionTop.c_str());
		lvGroup.cchDescriptionTop = DescriptionTop.length();
		lvGroup.pszDescriptionBottom = const_cast<wChar*>(DescriptionBottom.c_str());
		lvGroup.cchDescriptionBottom = DescriptionBottom.length();
		lvGroup.iTitleImage = TitleImageIndex;
		lvGroup.iExtendedImage = ExtendedImageIndex;
		lvGroup.iFirstItem = FirstItemIndex;
		lvGroup.cItems = ItemsCount;
		lvGroup.pszSubsetTitle = const_cast<wChar*>(SubsetTitle.c_str());
		lvGroup.cchSubsetTitle = SubsetTitle.length();
		return lvGroup;
	}

	ListView::Item::Item(const LVITEM& lvItem) :
		Mask(Masks(lvItem.mask)),
		ItemIndex(lvItem.iItem),
		SubItemIndex(lvItem.iSubItem),
		State(States(lvItem.state)),
		StateMask(States(lvItem.stateMask)),
		Text(lvItem.pszText, lvItem.cchTextMax),
		ImageIndex(lvItem.iImage),
		Data(lvItem.lParam),
		Indent(lvItem.iIndent),
		Group(Groups(lvItem.iGroup)),
		ColumnsCount(lvItem.cColumns),
		ColumnIDArray(lvItem.puColumns),
		ColumnFormatArray(lvItem.piColFmt),
		GroupIndex(lvItem.iGroupId) {

	}

	LVITEM ListView::Item::ToWinApiStruct() const noexcept {
		LVITEM lvItem;
		lvItem.mask = Mask;
		lvItem.iItem = ItemIndex;
		lvItem.iSubItem = SubItemIndex;
		lvItem.state = State;
		lvItem.stateMask = StateMask;
		lvItem.pszText = const_cast<wChar*>(Text.data());
		lvItem.cchTextMax = Text.length();
		lvItem.iImage = ImageIndex;
		lvItem.lParam = Data;
		lvItem.iIndent = Indent;
		lvItem.iGroup = Group;
		lvItem.cColumns = ColumnsCount;
		lvItem.puColumns = ColumnIDArray;
		lvItem.piColFmt = ColumnFormatArray;
		lvItem.iGroupId = GroupIndex;
		return lvItem;
	}
}