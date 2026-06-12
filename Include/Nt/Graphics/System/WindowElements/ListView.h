#pragma once

#include <Nt/Graphics/System/WindowElements/CommonControls.h>
#include <Nt/Graphics/System/WindowElements/TextEdit.h>
#include <Nt/Graphics/System/WindowElements/Tooltip.h>
#include <Nt/Graphics/System/WindowElements/ImageList.h>
#include <Nt/Core/Input.h>

namespace Nt {
	class ListView : public HandleWindow {
	public:
		enum ListStyles : uInt {
			LISTSTYLE_ALIGNLEFT = 0x0800,
			LISTSTYLE_ALIGNMASK = 0x0c00,
			LISTSTYLE_ALIGNTOP = 0x0000,
			LISTSTYLE_AUTOARRANGE = 0x0100,
			LISTSTYLE_EDITLABELS = 0x0200,
			LISTSTYLE_ICON = 0x0000,
			LISTSTYLE_LIST = 0x0003,
			LISTSTYLE_NOCOLUMNHEADER = 0x4000,
			LISTSTYLE_NOLABELWRAP = 0x0080,
			LISTSTYLE_NOSCROLL = 0x2000,
			LISTSTYLE_NOSORTHEADER = 0x8000,
			LISTSTYLE_OWNERDATA = 0x1000,
			LISTSTYLE_OWNERDRAWFIXED = 0x0400,
			LISTSTYLE_REPORT = 0x0001,
			LISTSTYLE_SHAREIMAGELISTS = 0x0040,
			LISTSTYLE_SHOWSELALWAYS = 0x0008,
			LISTSTYLE_SINGLESEL = 0x0004,
			LISTSTYLE_SMALLICON = 0x0002,
			LISTSTYLE_SORTASCENDING = 0x0010,
			LISTSTYLE_SORTDESCENDING = 0x0020,
			LISTSTYLE_TYPEMASK = 0x0003,
			LISTSTYLE_TYPESTYLEMASK = 0xfc00
		};
		enum ExtendedStyles : uInt {
			EX_STYLE_NONE = 0,
			EX_STYLE_AUTOAUTOARRANGE = 0x01000000,
			EX_STYLE_AUTOCHECKSELECT = 0x08000000,
			EX_STYLE_AUTOSIZECOLUMNS = 0x10000000,
			EX_STYLE_BORDERSELECT = 0x00008000,
			EX_STYLE_CHECKBOXES = 0x00000004,
			EX_STYLE_COLUMNOVERFLOW = 0x80000000,
			EX_STYLE_COLUMNSNAPPOINTS = 0x40000000,
			EX_STYLE_DOUBLEBUFFER = 0x00010000,
			EX_STYLE_FLATSB = 0x00000100,
			EX_STYLE_FULLROWSELECT = 0x00000020,
			EX_STYLE_GRIDLINES = 0x00000001,
			EX_STYLE_HEADERDRAGDROP = 0x00000010,
			EX_STYLE_HEADERINALLVIEWS = 0x02000000,
			EX_STYLE_HIDELABELS = 0x00020000,
			EX_STYLE_INFOTIP = 0x00000400,
			EX_STYLE_JUSTIFYCOLUMNS = 0x00200000,
			EX_STYLE_LABELTIP = 0x00004000,
			EX_STYLE_MULTIWORKAREAS = 0x00002000,
			EX_STYLE_ONECLICKACTIVATE = 0x00000040,
			EX_STYLE_REGIONAL = 0x00000200,
			EX_STYLE_SIMPLESELECT = 0x00100000,
			EX_STYLE_SINGLEROW = 0x00040000,
			EX_STYLE_SNAPTOGRID = 0x00080000,
			EX_STYLE_SUBITEMIMAGES = 0x00000002,
			EX_STYLE_TRACKSELECT = 0x00000008,
			EX_STYLE_TRANSPARENTBKGND = 0x00400000,
			EX_STYLE_TRANSPARENTSHADOWTEXT = 0x00800000,
			EX_STYLE_TWOCLICKACTIVATE = 0x00000080,
			EX_STYLE_UNDERLINECOLD = 0x00001000,
			EX_STYLE_UNDERLINEHOT = 0x00000800,
		};
		enum FindFlags : uInt {
			FIND_FLAG_DATA = 0x0001,
			FIND_FLAG_PARTIAL = 0x0008,
			FIND_FLAG_STRING = 0x0002,
			FIND_FLAG_SUBSTRING = 0x0004,
			FIND_FLAG_WRAP = 0x0020,
			FIND_FLAG_NEAREST_POINT = 0x0040
		};
		enum ArrangeTypes : uInt {
			ARRANGE_ALIGNLEFT = 0x0001,
			ARRANGE_ALIGNTOP = 0x0002,
			ARRANGE_DEFAULT = 0x0000,
			ARRANGE_SNAPTOGRID = 0x0005
		};
		enum class ImageListTypes : uInt {
			NORMAL = 0,
			SMALL = 1,
			STATE = 2,
			GROUPHEADER = 3
		};
		enum class ItemRectTypes : uInt {
			BOUNDS = 0,
			ICON = 1,
			LABEL = 2,
			SELECTBOUNDS = 3
		};
		enum class GroupRectTypes : uInt {
			GROUP = 0,
			HEADER = 1,
			LABEL = 2,
			SUBSETLINK = 3
		};
		enum class NextType : uInt {
			NEXT = 0x0000,
			PREVIOUS = 0x0020,
			ABOVE = 0x0100,
			BELOW = 0x0200,
			TOLEFT = 0x0400,
			TORIGHT = 0x0800,
			DIRECTIONMASK = (ABOVE | BELOW | TOLEFT | TORIGHT),
			CUT = 0x0004,
			DROPHILITED = 0x0008,
			FOCUSED = 0x0001,
			SELECTED = 0x0002,
			STATEMASK = (FOCUSED | SELECTED | CUT | DROPHILITED),
			VISIBLEORDER = 0x0010,
			VISIBLEONLY = 0x0040,
			SAMEGROUPONLY = 0x0080,
		};
		enum ViewTypes : uInt {
			VIEW_TYPE_DETAILS = 0x0001,
			VIEW_TYPE_ICON = 0x0000,
			VIEW_TYPE_LIST = 0x0003,
			VIEW_TYPE_SMALLICON = 0x0002,
			VIEW_TYPE_TILE = 0x0004,
		};

		struct Column {
			enum Masks : uInt {
				MASK_NONE = 0,
				MASK_FORMAT = 0x0001,
				MASK_WIDTH = 0x0002,
				MASK_TEXT = 0x0004,
				MASK_SUBITEM = 0x0008,
				MASK_IMAGE = 0x0010,
				MASK_OFFSET_X = 0x0020,
				MASK_MINWIDTH = 0x0040,
				MASK_DEFAULTWIDTH = 0x0080,
				MASK_IDEALWIDTH = 0x0100,
			};
			enum Formats {
				FORMAT_NONE = 0x0000,
				FORMAT_LEFT = 0x0000,
				FORMAT_RIGHT = 0x0001,
				FORMAT_CENTER = 0x0002,
				FORMAT_JUSTIFYMASK = 0x0003,
				FORMAT_IMAGE = 0x0800,
				FORMAT_BITMAP_ON_RIGHT = 0x1000,
				FORMAT_HAS_IMAGES = 0x8000,
				FORMAT_FIXED_WIDTH = 0x00100,
				FORMAT_NO_DPI_SCALE = 0x40000,
				FORMAT_FIXED_RATIO = 0x80000,
				FORMAT_SPLITBUTTON = 0x1000000
			};

			Masks Mask = MASK_NONE;
			Formats Format = FORMAT_NONE;
			Int Width = 0;
			wChar* TextBufferPtr = nullptr;
			Int BufferSize = 0;
			Int SubItemIndex = 0;
			Int ImageIndex = 0;
			Int OffsetX = 0;
			Int MinWidth = 0;
			Int DefaultWidth = 0;
			Int IdealWidth = 0;
		};

		struct Item {
			enum Masks : uInt {
				MASK_NONE = 0,
				MASK_COLFMT = 0x00010000,
				MASK_COLUMNS = 0x00000200,
				MASK_DI_SETITEM = 0x1000,
				MASK_GROUPID = 0x00000100,
				MASK_IMAGE = 0x00000002,
				MASK_INDENT = 0x00000010,
				MASK_NORECOMPUTE = 0x00000800,
				MASK_DATA = 0x00000004,
				MASK_STATE = 0x00000008,
				MASK_TEXT = 0x00000001
			};
			enum States : uInt {
				STATE_NONE = 0,
				STATE_FOCUSED = 0x0001,
				STATE_SELECTED = 0x0002,
				STATE_CUT = 0x0004,
				STATE_DROPHILITED = 0x0008,
				STATE_GLOW = 0x0010,
				STATE_ACTIVATING = 0x0020,
				STATE_OVERLAYMASK = 0x0F00,
				STATE_STATEIMAGEMASK = 0xF000,
			};
			enum Groups {
				GROUP_NONE = -2,
				GROUP_CALLBACK = -1
			};
			enum ColumnFormats {
				COLUMN_FORMAT_LINE_BREAK = 0x100000,
				COLUMN_FORMAT_FILL = 0x200000,
				COLUMN_FORMAT_WRAP = 0x400000,
				COLUMN_FORMAT_NO_TITLE = 0x800000,
				COLUMN_FORMAT_TILE_PLACEMENTMASK = (COLUMN_FORMAT_LINE_BREAK | COLUMN_FORMAT_FILL)
			};

			Item() = default;
			NT_API Item(const LVITEM& lvItem);

			NT_API LVITEM ToWinApiStruct() const noexcept;

			Masks Mask = MASK_NONE;
			Int ItemIndex = 0;
			Int SubItemIndex = 0;
			States State = STATE_NONE;
			States StateMask = STATE_NONE;
			std::wstring Text;
			Int ImageIndex = 0;
			Long Data = 0;
			Int Indent = 0;
			Groups Group = GROUP_NONE;
			uInt ColumnsCount = 0;
			uInt* ColumnIDArray = nullptr;
			Int* ColumnFormatArray = nullptr;
			Int GroupIndex = -1;
		};

		struct HitTextInfo {
			enum Flags {
				FLAG_ABOVE = 0x00000008,
				FLAG_BELOW = 0x00000010,
				FLAG_NOWHERE = 0x00000001,
				FLAG_ONITEMICON = 0x00000002,
				FLAG_ONITEMLABEL = 0x00000004,
				FLAG_ONITEMSTATEICON = 0x00000008,
				FLAG_TOLEFT = 0x00000040,
				FLAG_TORIGHT = 0x00000020,
				FLAG_EX_GROUP_HEADER = 0x10000000,
				FLAG_EX_GROUP_FOOTER = 0x20000000,
				FLAG_EX_GROUP_COLLAPSE = 0x40000000,
				FLAG_EX_GROUP_BACKGROUND = 0x80000000,
				FLAG_EX_GROUP_STATEICON = 0x01000000,
				FLAG_EX_GROUP_SUBSETLINK = 0x02000000,
				FLAG_EX_GROUP = 
					(FLAG_EX_GROUP_BACKGROUND |
						FLAG_EX_GROUP_COLLAPSE | 
						FLAG_EX_GROUP_FOOTER | 
						FLAG_EX_GROUP_HEADER | 
						FLAG_EX_GROUP_STATEICON | 
						FLAG_EX_GROUP_SUBSETLINK),
				FLAG_EX_ONCONTENTS = 0x04000000,
				FLAG_EX_FOOTER = 0x08000000
			};

			Long2D Point;
			Flags Flags;
			Int ItemIndex;
			Int SubItemIndex;
			Int GroupIndex;
		};

		struct Group {
			enum Masks {
				MASK_NONE = 0x00000000,
				MASK_HEADER = 0x00000001,
				MASK_FOOTER = 0x00000002,
				MASK_STATE = 0x00000004,
				MASK_ALIGN = 0x00000008,
				MASK_GROUPID = 0x00000010,
				MASK_SUBTITLE = 0x00000100,
				MASK_TASK = 0x00000200,
				MASK_DESCRIPTIONTOP = 0x00000400,
				MASK_DESCRIPTIONBOTTOM = 0x00000800,
				MASK_TITLEIMAGE = 0x00001000,
				MASK_EXTENDEDIMAGE = 0x00002000,
				MASK_ITEMS = 0x00004000,
				MASK_SUBSET = 0x00008000,
				MASK_SUBSETITEMS = 0x00010000
			};
			enum States {
				STATE_NORMAL = 0x00000000,
				STATE_COLLAPSED = 0x00000001,
				STATE_HIDDEN = 0x00000002,
				STATE_NOHEADER = 0x00000004,
				STATE_COLLAPSIBLE = 0x00000008,
				STATE_FOCUSED = 0x00000010,
				STATE_SELECTED = 0x00000020,
				STATE_SUBSETED = 0x00000040,
				STATE_SUBSETLINKFOCUSED = 0x00000080
			};
			enum Aligns {
				ALIGN_NONE = 0x00000000,
				ALIGN_FOOTER_CENTER = 0x00000010,
				ALIGN_FOOTER_LEFT = 0x00000008,
				ALIGN_FOOTER_RIGHT = 0x00000020,
				ALIGN_HEADER_CENTER = 0x00000002,
				ALIGN_HEADER_LEFT = 0x00000001,
				ALIGN_HEADER_RIGHT = 0x00000004
			};

			Group() = default;
			NT_API Group(const LVGROUP& lvGroup);

			NT_API LVGROUP ToWinApiStruct() const noexcept;

			std::wstring Header;
			std::wstring Footer;
			std::wstring Subtitle;
			std::wstring Task;
			std::wstring DescriptionTop;
			std::wstring DescriptionBottom;
			std::wstring SubsetTitle;
			Masks Mask = MASK_NONE;
			Int GroupID = 0;
			States StateMask = STATE_NORMAL;
			States State = STATE_NORMAL;
			Aligns Align = ALIGN_NONE;
			Int TitleImageIndex;
			Int ExtendedImageIndex;
			Int FirstItemIndex;
			uInt ItemsCount;
		};

		struct GroupMetrics {
			enum Masks {
				MASK_BORDERCOLOR = 0x00000002,
				MASK_BORDERSIZE = 0x00000001,
				MASK_NONE = 0x00000000,
				MASK_TEXTCOLOR = 0x00000004
			};

			GroupMetrics() = default;
			NT_API GroupMetrics(const LVGROUPMETRICS& metrics);

			NT_API LVGROUPMETRICS ToWinApiStruct() const noexcept;

			Masks Mask = MASK_NONE;
			uIntRect Rect;
		};

		struct TileViewInfo {
			enum Masks : uInt {
				MASK_NONE = 0x00000000,
				MASK_TILESIZE = 0x00000001,
				MASK_COLUMNS = 0x00000002,
				MASK_LABELMARGIN = 0x00000004
			};
			enum SizeFlags: uInt {
				FLAG_AUTOSIZE = 0x00000000,
				FLAG_EXTENDED = 0x00000004,
				FLAG_FIXEDWIDTH = 0x00000001,
				FLAG_FIXEDHEIGHT = 0x00000002,
				FLAG_FIXEDSIZE = 0x00000003
			};

			TileViewInfo() = default;
			NT_API TileViewInfo(const LVTILEVIEWINFO& info);

			NT_API LVTILEVIEWINFO ToWinApiStruct() const noexcept;

			IntRect LabelMargin;
			Int2D Size;
			Int LineCount = 0;
			Masks Mask = MASK_NONE;
			SizeFlags Flags = FLAG_AUTOSIZE;
		};

		struct TileInfo {
			TileInfo() = default;
			NT_API TileInfo(const LVTILEINFO& info);

			NT_API LVTILEINFO ToWinApiStruct() const noexcept;

			Int ItemIndex;
			std::vector<uInt> ColumnsIndices;
			std::vector<Column::Formats> ColumnsFormats;
		};

		using SortCompare = Int(*)(const uInt& leftItemIndex, const uInt& rightItemIndex, const uInt& param);
		using SortGroupCompare = PFNLVGROUPCOMPARE;

	public:
		ListView() = default;
		NT_API ListView(const IntRect& windowRect);
		~ListView() noexcept override = default;

		NT_API void Create(const IntRect& windowRect);
		NT_API void Create(const IntRect& windowRect, const String& name) override;

		NT_API void AddExtendedStyles(const ExtendedStyles& styles, const ExtendedStyles& mask) noexcept;

		NT_API Int AddItem(const Item& item);
		NT_API Bool RemoveItem(const uInt& index);
		NT_API Bool ClearItems();

		NT_API Bool AddColumn(const Column& column, const uInt& index);
		NT_API Bool RemoveColumn(const uInt& index);

		NT_API Int InsertGroup(const Group& group, const uInt& itemIndex);
		NT_API void InsertGroupSorted(const SortGroupCompare& function, void* pData, const Group& group);
		NT_API Int RemoveGroup(const uInt& groupID);
		NT_API void RemoveAllGroups();

		NT_API void MoveGroup(const uInt& groupID, const uInt& toIndex);
		NT_API void MoveItemToGroup(const uInt& groupIDFrom, const uInt& groupIDTo);
		NT_API Int EnableGroupView(const Bool& isEnabled);
		NT_API Bool SortGroups(const SortGroupCompare& function, void* pParam);
		NT_API Bool HasGroup(const uInt& groupID) const;;

		NT_API Bool Redraw(const uInt& indexFirst, const uInt& indexLast) const;
		NT_API Bool Update(const uInt& itemIndex);

		NT_API Int FindItem(const FindFlags& flags, const std::wstring& searchText, const Long& searchData, const Int2D& position, const Key& key, const uInt& startIndex = -1) const;
		NT_API Bool SortItems(const SortCompare& function, const Long& param);
		NT_API Bool SortItemsEx(const SortCompare& function, const Long& param);

		NT_API Int HitTest(HitTextInfo* pHitTextInfo) const;
		NT_API Int SubItemHitTest(HitTextInfo* pHitTextInfo) const;

		NT_API Bool ToggleVisible(const uInt& listIndex, const Bool& isVisible) const;
		NT_API Bool Scroll(const Int2D& scrollValue);
		NT_API Bool Arrange(const ArrangeTypes& type);

		NT_API TextEdit BeginEditLable(const uInt& listIndex);
		NT_API void CancelEditLabel();

		NT_API ImageList CreateDragImage(const uInt& itemIndex, const Int2D& position);
		NT_API Bool InsertMarkHitTest(Long2D* pCoordinates, const Bool& insertAfter, const Int& itemIndex);

		NT_API Int ItemIndexToID(const uInt& itemIndex) const;
		NT_API Int ItemIDToIndex(const uInt& itemID) const;

		NT_API Int2D GetApproximateSize() const;
		NT_API Byte3D GetBackgroundColor() const noexcept;
		NT_API Byte3D GetTextColor() const noexcept;
		NT_API Byte3D GetTextBackgroundColor() const noexcept;
		NT_API ImageList GetImageList(const ImageListTypes& type) const;
		NT_API uInt GetItemCount() const;
		NT_API Bool GetItem(Item* pItem) const;
		NT_API IntRect GetItemRect(const uInt& itemIndex, const ItemRectTypes& type) const;
		NT_API IntRect GetItemRectFromGroup(const uInt& itemIndex, const Int& subItemIndex, const uInt& goupID, const ItemRectTypes& type) const;
		NT_API Item::States GetItemState(const uInt& itemIndex, const Item::States& stateMask) const;
		NT_API String GetItemText(const uInt& itemIndex, const uInt& subItemIndex) const;
		NT_API Int2D GetItemSpacing(const Bool& isSmallIcon) const;
		NT_API Bool GetItemPosition(const uInt& itemIndex, Long2D* pPosition) const;
		NT_API Item::States GetCallbackMask() const;
		NT_API Int GetNextItem(const Int& itemIndex, const NextType& type) const;
		NT_API Int GetStringWidth(const std::wstring& text) const;
		NT_API TextEdit GetTextEdit() const;
		NT_API Bool GetColumn(const Int& columnIndex, Column* pColumn) const;
		NT_API Int GetColumnWidth(const Int& columnIndex) const;
		//Header GetHeader() const {
		//	const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(LVM_GETHEADER, 0, 0));
		//	return Header(hwnd);
		//}
		NT_API IntRect GetViewRect() const;
		NT_API Int GetTopIndex() const;
		NT_API Int GetCountPerPage() const;
		NT_API Bool GetOrigin(Int2D* pOrigin) const;
		NT_API Bool GetCheckState(const uInt& itemIndex) const;
		NT_API Int GetSelectedCount() const;
		NT_API std::wstring GetSearchString() const;
		NT_API ExtendedStyles GetExtendedStyle() const;
		NT_API IntRect GetSubItemRect(const Int& itemIndex, const Int& subItemIndex, const ItemRectTypes& type) const;
		NT_API Bool GetColumnOrderArray(const uInt& columnCount, Int* columnIndexArray) const;
		NT_API Int GetHotItem() const;
		//Cursor GetHotCursor() const {
		//	const HCURSOR hCursor = reinterpret_cast<HCURSOR>(_SendMessage(LVM_GETHOTCURSOR, 0, 0));
		//	return Cursor(hCursor);
		//}
		NT_API void GetWorkAreas(const Int& arraySize, LongRect* rectArray) const;
		NT_API uInt GetNumberOfWorkAreas() const;
		NT_API Int GetSelectionMark() const;
		NT_API uInt GetMouseHoverTime() const;
		NT_API Tooltip GetToolTips() const;
		NT_API ViewTypes GetView() const;

		NT_API Bool GetGroup(const uInt& groupID, Group* pGroup) const;
		NT_API Int GetGroupCount() const;
		NT_API Bool GetGroupByIndex(const uInt& groupID, Group* pGroup) const;
		NT_API IntRect GetGroupRect(const uInt& groupID, const GroupRectTypes& type) const;
		NT_API GroupMetrics GetGroupMetrics() const;
		NT_API Group::States GetGroupState(const uInt& groupID, const Group::States& mask) const;
		NT_API uInt GetFocusedGroup() const;
		NT_API TileViewInfo GetTileViewInfo() const;
		NT_API TileInfo GetTileInfo() const;
		NT_API Int GetInsertMark(const Int& itemIndex, const Bool& isAfter) const;
		NT_API IntRect GetInsertMarkRect() const;
		NT_API Byte3D GetInsertMarkColor() const;
		NT_API uInt GetSelectedColumn() const;
		NT_API Byte3D GetOutlineColor() const;
		NT_API ImageList GetImageList() const;
		NT_API std::wstring GetEmptyText() const;
		NT_API IntRect GetFooterRect() const;
		//GetFooterInfo() const {
		//	(BOOL)_SendMessage(LVM_GETFOOTERINFO, (WPARAM)(0), (LPARAM)(plvfi))
		//}
		NT_API IntRect GetFooterItemRect(const uInt& itemIndex) const;
		//GetFooterItem() const {
		//	(BOOL)_SendMessage(LVM_GETFOOTERITEM, (WPARAM)(iItem), (LPARAM)(pfi))
		//}
		//GetNextItemIndex() const {
		//	(BOOL)_SendMessage(LVM_GETNEXTITEMINDEX, (WPARAM)(LVITEMINDEX*)(plvii), MAKELPARAM((flags), 0))
		//}
		//GetBackgroundImage() const {
		//	(BOOL)_SendMessage(LVM_GETBKIMAGE, 0, (LPARAM)(plvbki))
		//}
		NT_API Bool IsUseUnicode() const;
		NT_API Bool IsGroupViewEnabled() const;
		//Bool IsItemVisible() const {
		//	(UINT)_SendMessage(LVM_ISITEMVISIBLE, (WPARAM)(index), (LPARAM)0)
		//}

		NT_API void SetItemState(const Int& index, const uInt& styles, const uInt& mask);

		NT_API void SortItems(const PFNLVCOMPARE& func, const Long& param);

		NT_API void EnableTextBackground() noexcept;
		NT_API void DisableTextBackground() noexcept;

		NT_API void SetBackgroundColor(const Byte3D& color) override;
		NT_API void SetTextColor(const Byte3D& color);
		NT_API void SetTextBackgroundColor(const Byte3D& color);

	private:
		std::vector<Item> m_Items;
		Byte3D m_TextBackgroundColor = { 255, 255, 255 };
		Byte3D m_TextColor;
		ExtendedStyles m_ExtendedStyles = EX_STYLE_NONE;
		Bool m_EnabledTextBackground = true;
	};
}