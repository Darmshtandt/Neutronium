#pragma once

#include <Nt/Graphics/System/WindowElements/CommonControls.h>
#include <Nt/Graphics/System/WindowElements/ImageList.h>
#include <Nt/Graphics/System/WindowElements/TextEdit.h>
#include <Nt/Graphics/System/WindowElements/Tooltip.h>

namespace Nt {
	class TreeView : public HandleWindow {
	public:
		using ItemID = HTREEITEM;
		
		inline static const ItemID FirstID = ((ItemID)(uLong)-0x0FFFF);
		inline static const ItemID LastID = ((ItemID)(uLong)-0x0FFFE);
		inline static const ItemID RootID = ((ItemID)(uLong)-0x10000);

	public:
		struct Item {
			enum Masks : uInt {
				MASK_NONE = 0x0000,
				MASK_TEXT = 0x0001,
				MASK_IMAGE = 0x0002,
				MASK_DATA = 0x0004,
				MASK_STATE = 0x0008,
				MASK_HANDLE = 0x0010,
				MASK_SELECTEDIMAGE = 0x0020,
				MASK_CHILDREN = 0x0040,
				MASK_INTEGRAL = 0x0080,

#if (_WIN32_IE >= 0x0600)
				MASK_STATEEX = 0x0100,
				MASK_EXPANDEDIMAGE = 0x0200,
				MASK_ALL = (MASK_TEXT | MASK_IMAGE | MASK_DATA | MASK_STATE |
				MASK_SELECTEDIMAGE | MASK_HANDLE | MASK_CHILDREN | MASK_INTEGRAL |
					MASK_STATEEX | MASK_EXPANDEDIMAGE),
#else									  
				MASK_ALL = (MASK_TEXT | MASK_IMAGE | MASK_DATA | MASK_STATE |
				MASK_SELECTEDIMAGE | MASK_HANDLE | MASK_CHILDREN | MASK_INTEGRAL),
#endif									  
			};
			enum States : uInt {
				STATE_NONE = 0x0000,
				STATE_SELECTED = 0x0002,
				STATE_CUT = 0x0004,
				STATE_DROPHILITED = 0x0008,
				STATE_BOLD = 0x0010,
				STATE_EXPANDED = 0x0020,
				STATE_EXPANDEDONCE = 0x0040,
				STATE_EXPANDPARTIAL = 0x0080,
				STATE_OVERLAYMASK = 0x0F00,
				STATE_STATEIMAGEMASK = 0xF000,
				STATE_USERMASK = 0xF000,

#if (_WIN32_IE >= 0x0600)
				EX_FLAT = 0x0001,
#	if (NTDDI_VERSION >= NTDDI_VISTA)
				EX_DISABLED = 0x0002,
#	endif
				EX_ALL = 0x0002,
#endif
			};
			enum Childrens : Int {
				CHILDREN_NO_ONE = 0,
				CHILDREN_ONE_OR_MORE = 1,
				CHILDREN_CALLBACK = -1,
				CHILDREN_AUTO = -2,
			};

			Item() = default;
			NT_API Item(const TVITEM& tvItem);

			NT_API TVITEM ToWinApiStruct() const noexcept;

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
			EXPAND_COLLAPSE = 0x0001,
			EXPAND_EXPAND = 0x0002,
			EXPAND_TOGGLE = 0x0003,
			EXPAND_EXPANDPARTIAL = 0x4000,
			EXPAND_COLLAPSERESET = 0x8000,
		};
		enum ExtendStyles {
			EX_STYLE_NONE = 0x0000,
			EX_STYLE_MULTISELECT = 0x0002,
			EX_STYLE_DOUBLEBUFFER = 0x0004,
			EX_STYLE_NOINDENTSTATE = 0x0008,
			EX_STYLE_RICHTOOLTIP = 0x0010,
			EX_STYLE_AUTOHSCROLL = 0x0020,
			EX_STYLE_FADEINOUTEXPANDOS = 0x0040,
			EX_STYLE_PARTIALCHECKBOXES = 0x0080,
			EX_STYLE_EXCLUSIONCHECKBOXES = 0x0100,
			EX_STYLE_DIMMEDCHECKBOXES = 0x0200,
			EX_STYLE_DRAWIMAGEASYNC = 0x0400,
		};

		struct HitTestInfo {
			enum Hittest {
				HITTEST_NONE = 0x0000,
				HITTEST_NOWHERE = 0x0001,
				HITTEST_ONITEMICON = 0x0002,
				HITTEST_ONITEMLABEL = 0x0004,
				HITTEST_ONITEMINDENT = 0x0008,
				HITTEST_ONITEMBUTTON = 0x0010,
				HITTEST_ONITEMRIGHT = 0x0020,
				HITTEST_ONITEMSTATEICON = 0x0040,
				HITTEST_ONITEM = (HITTEST_ONITEMICON | HITTEST_ONITEMLABEL | HITTEST_ONITEMSTATEICON),
				HITTEST_ABOVE = 0x0100,
				HITTEST_BELOW = 0x0200,
				HITTEST_TORIGHT = 0x0400,
				HITTEST_TOLEFT = 0x0800,
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
		TreeView() noexcept = default;
		NT_API TreeView(const IntRect& windowRect);
		~TreeView() noexcept override = default;

		NT_API void Create(const IntRect& windowRect);
		NT_API void Create(const IntRect& windowRect, const String& name) override;

		NT_API ItemID Add(const Item& item, const ItemID& parentItemID = RootID, const ItemID& afterItemID = RootID);
		NT_API Bool Remove(const ItemID& itemID);
		NT_API void RemoveSelectedItems();
		NT_API Bool Clear();

		NT_API void Select(const ItemID& itemID);
		NT_API void Deselect(const ItemID& itemID);
		NT_API void DeselectAll();

		NT_API HRESULT AddTreeExStyles(const ExtendStyles& styles) noexcept;
		NT_API HRESULT RemoveTreeExStyles(const ExtendStyles& styles) noexcept;

		NT_API HitTestInfo HitTest(const Int2D& point);
		NT_API Bool Expand(const ItemID& itemID, const ExpandTypes& type);
		NT_API TextEdit BeginEditLabel(const ItemID& itemID);
		NT_API Bool EndEditLabelNow(const Bool& cancelWithoutSaving);

		NT_API ImageList CreateDragImage(const ItemID& itemID);
		NT_API Bool SelectDropTarget(const ItemID& itemID);
		NT_API Bool SelectSetFirstVisible(const ItemID& itemID);

		NT_API Bool SortChildren(const ItemID& itemID, const Bool& recurse);
		NT_API Bool SortChildren(const ItemID& itemID, const SortFunction& sortFunction, const Long& param, const Bool& recurse);

		NT_API Bool EnsureVisible(const ItemID& itemID);
		NT_API Int ShowInfoTip(const ItemID& itemID);

		NT_API void HandleNotify(NMHDR* pNMHDR);

		NT_API ItemID GetRootItem() const;
		NT_API ItemID GetNextItem(const ItemID& itemID) const;
		NT_API ItemID GetPrevItem(const ItemID& itemID) const;
		NT_API ItemID GetNextParent(const ItemID& childID) const;
		NT_API ItemID GetNextChild(const ItemID& parentID) const;
		NT_API ItemID GetFirstVisible() const;
		NT_API ItemID GetNextVisible(const ItemID& itemID) const;
		NT_API ItemID GetPrevVisible(const ItemID& itemID) const;
		NT_API ItemID GetLastVisible() const;
		NT_API ItemID GetDropHilight() const;
		NT_API ItemID GetNextSelected(const ItemID& itemID) const;
		NT_API ItemID GetSelection() const;
		NT_API Bool GetItem(Item* pItem) const;
		NT_API IntRect GetItemRect(const ItemID& itemID, const Bool& isTextRect) const;
		NT_API uInt GetCount() const;
		NT_API uInt GetIndent() const;
		NT_API ImageList GetImageList(const Bool& imageListIsState) const;
		NT_API TextEdit GetTextEdit() const;
		NT_API uInt GetVisibleCount() const;
		NT_API Tooltip GetToolTips() const;
		NT_API std::wstring GetSearchString() const;
		NT_API uInt GetItemHeight() const;
		NT_API uInt GetScrollTime() const;
		NT_API Byte3D GetInsertMarkColor() const;
		NT_API Item::States GetItemState(const ItemID& itemID, const Item::States& stateMask) const;
		NT_API Bool GetCheckState(const ItemID& itemID) const;
		NT_API Byte3D GetLineColor() const;
		NT_API ExtendStyles GetExtendedStyle() const;
		NT_API uInt GetSelectedCount() const;
		NT_API IntRect GetItemPartRect(const ItemID& itemID) const;
		NT_API Bool IsUseUnicode() const;
		NT_API const std::vector<ItemID>& GetSelectedItems() const noexcept;
		NT_API Bool IsRenamingEnabled() const noexcept;

		NT_API void SetBackgroundColor(const Byte3D& color);
		NT_API void SetTextColor(const Byte3D& color);
		NT_API void SetItemState(const ItemID& itemID, const Item::States& state, const Item::States& stateMask);
		NT_API Bool SetItem(Item* pItem);
		NT_API ImageList SetImageList(const ImageList& imageList, const Bool& isImageListState);
		NT_API HRESULT SetExtendedStyle(const ExtendStyles& styles, const ExtendStyles& mask);
		NT_API Bool SetIndent(const uInt& indent);
		NT_API void SetToolTips(const Tooltip& tooltip);
		NT_API void ToggleUnicode(const Bool& isUseEnicode);
		NT_API void SetItemHeight(const uInt& height);
		NT_API void SetScrollTime(const Int& scrollTime);
		NT_API Byte3D SetInsertMarkColor(const Byte3D& color);
		NT_API void SetCheckState(const ItemID& itemID, const Bool& isChecked);
		NT_API Byte3D SetLineColor(const Byte3D& color);
		NT_API void SetAutoScrollInfo(const uInt& offsetPerSecond, const uInt& updateTime);

	private:
		std::vector<ItemID> m_SelectedTreeItems;
		HIMAGELIST m_hImageList = nullptr;
		Byte3D m_TextColor;
		ExtendStyles m_TreeExStyles = EX_STYLE_NONE;
		String m_Text;
		uInt m_TextWeight = 400;
		Bool m_IsRenammingEnabled = false;

	private:
		NT_API void _HandleSelectItem(ItemID hSelectedItem);
		NT_API void _HandleDeleteItem();
	};
}