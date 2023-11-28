#pragma once

namespace Nt {
	class ListView : public HandleWindow {
	public:
		enum ListStyles : uInt {
			LISTSTYLE_ALIGNLEFT = LVS_ALIGNLEFT,
			LISTSTYLE_ALIGNMASK = LVS_ALIGNMASK,
			LISTSTYLE_ALIGNTOP = LVS_ALIGNTOP,
			LISTSTYLE_AUTOARRANGE = LVS_AUTOARRANGE,
			LISTSTYLE_EDITLABELS = LVS_EDITLABELS,
			LISTSTYLE_ICON = LVS_ICON,
			LISTSTYLE_LIST = LVS_LIST,
			LISTSTYLE_NOCOLUMNHEADER = LVS_NOCOLUMNHEADER,
			LISTSTYLE_NOLABELWRAP = LVS_NOLABELWRAP,
			LISTSTYLE_NOSCROLL = LVS_NOSCROLL,
			LISTSTYLE_NOSORTHEADER = LVS_NOSORTHEADER,
			LISTSTYLE_OWNERDATA = LVS_OWNERDATA,
			LISTSTYLE_OWNERDRAWFIXED = LVS_OWNERDRAWFIXED,
			LISTSTYLE_REPORT = LVS_REPORT,
			LISTSTYLE_SHAREIMAGELISTS = LVS_SHAREIMAGELISTS,
			LISTSTYLE_SHOWSELALWAYS = LVS_SHOWSELALWAYS,
			LISTSTYLE_SINGLESEL = LVS_SINGLESEL,
			LISTSTYLE_SMALLICON = LVS_SMALLICON,
			LISTSTYLE_SORTASCENDING = LVS_SORTASCENDING,
			LISTSTYLE_SORTDESCENDING = LVS_SORTDESCENDING,
			LISTSTYLE_TYPEMASK = LVS_TYPEMASK,
			LISTSTYLE_TYPESTYLEMASK = LVS_TYPESTYLEMASK
		};
		enum ExtendedStyles : uInt {
			EX_STYLE_NONE = 0,
			EX_STYLE_AUTOAUTOARRANGE = LVS_EX_AUTOAUTOARRANGE,
			EX_STYLE_AUTOCHECKSELECT = LVS_EX_AUTOCHECKSELECT,
			EX_STYLE_AUTOSIZECOLUMNS = LVS_EX_AUTOSIZECOLUMNS,
			EX_STYLE_BORDERSELECT = LVS_EX_BORDERSELECT,
			EX_STYLE_CHECKBOXES = LVS_EX_CHECKBOXES,
			EX_STYLE_COLUMNOVERFLOW = LVS_EX_COLUMNOVERFLOW,
			EX_STYLE_COLUMNSNAPPOINTS = LVS_EX_COLUMNSNAPPOINTS,
			EX_STYLE_DOUBLEBUFFER = LVS_EX_DOUBLEBUFFER,
			EX_STYLE_FLATSB = LVS_EX_FLATSB,
			EX_STYLE_FULLROWSELECT = LVS_EX_FULLROWSELECT,
			EX_STYLE_GRIDLINES = LVS_EX_GRIDLINES,
			EX_STYLE_HEADERDRAGDROP = LVS_EX_HEADERDRAGDROP,
			EX_STYLE_HEADERINALLVIEWS = LVS_EX_HEADERINALLVIEWS,
			EX_STYLE_HIDELABELS = LVS_EX_HIDELABELS,
			EX_STYLE_INFOTIP = LVS_EX_INFOTIP,
			EX_STYLE_JUSTIFYCOLUMNS = LVS_EX_JUSTIFYCOLUMNS,
			EX_STYLE_LABELTIP = LVS_EX_LABELTIP,
			EX_STYLE_MULTIWORKAREAS = LVS_EX_MULTIWORKAREAS,
			EX_STYLE_ONECLICKACTIVATE = LVS_EX_ONECLICKACTIVATE,
			EX_STYLE_REGIONAL = LVS_EX_REGIONAL,
			EX_STYLE_SIMPLESELECT = LVS_EX_SIMPLESELECT,
			EX_STYLE_SINGLEROW = LVS_EX_SINGLEROW,
			EX_STYLE_SNAPTOGRID = LVS_EX_SNAPTOGRID,
			EX_STYLE_SUBITEMIMAGES = LVS_EX_SUBITEMIMAGES,
			EX_STYLE_TRACKSELECT = LVS_EX_TRACKSELECT,
			EX_STYLE_TRANSPARENTBKGND = LVS_EX_TRANSPARENTBKGND,
			EX_STYLE_TRANSPARENTSHADOWTEXT = LVS_EX_TRANSPARENTSHADOWTEXT,
			EX_STYLE_TWOCLICKACTIVATE = LVS_EX_TWOCLICKACTIVATE,
			EX_STYLE_UNDERLINECOLD = LVS_EX_UNDERLINECOLD,
			EX_STYLE_UNDERLINEHOT = LVS_EX_UNDERLINEHOT,
		};
		enum FindFlags : uInt {
			FIND_FLAG_DATA = LVFI_PARAM,
			FIND_FLAG_PARTIAL = LVFI_PARTIAL,
			FIND_FLAG_STRING = LVFI_STRING,
			FIND_FLAG_SUBSTRING = LVFI_SUBSTRING,
			FIND_FLAG_WRAP = LVFI_WRAP,
			FIND_FLAG_NEAREST_POINT = LVFI_NEARESTXY
		};
		enum ArrangeTypes : uInt {
			ARRANGE_ALIGNLEFT = LVA_ALIGNLEFT,
			ARRANGE_ALIGNTOP = LVA_ALIGNTOP,
			ARRANGE_DEFAULT = LVA_DEFAULT,
			ARRANGE_SNAPTOGRID = LVA_SNAPTOGRID
		};
		enum class ImageListTypes : uInt {
			NORMAL = LVSIL_NORMAL,
			SMALL = LVSIL_SMALL,
			STATE = LVSIL_STATE,
			GROUPHEADER = LVSIL_GROUPHEADER
		};
		enum class ItemRectTypes : uInt {
			BOUNDS = LVIR_BOUNDS,
			ICON = LVIR_ICON,
			LABEL = LVIR_LABEL,
			SELECTBOUNDS = LVIR_SELECTBOUNDS
		};
		enum class GroupRectTypes : uInt {
			GROUP = LVGGR_GROUP,
			HEADER = LVGGR_HEADER,
			LABEL = LVGGR_LABEL,
			SUBSETLINK = LVGGR_SUBSETLINK
		};
		enum class NextType : uInt {
			NEXT = LVNI_ALL,
			PREVIOUS = LVNI_PREVIOUS,
			ABOVE = LVNI_ABOVE,
			BELOW = LVNI_BELOW,
			TOLEFT = LVNI_TOLEFT,
			TORIGHT = LVNI_TORIGHT,
			DIRECTIONMASK = LVNI_DIRECTIONMASK,
			CUT = LVNI_CUT,
			DROPHILITED = LVNI_DROPHILITED,
			FOCUSED = LVNI_FOCUSED,
			SELECTED = LVNI_SELECTED,
			STATEMASK = LVNI_STATEMASK,
			VISIBLEORDER = LVNI_VISIBLEORDER,
			VISIBLEONLY = LVNI_VISIBLEONLY,
			SAMEGROUPONLY = LVNI_SAMEGROUPONLY,
		};
		enum ViewTypes : uInt {
			VIEW_TYPE_DETAILS = LV_VIEW_DETAILS,
			VIEW_TYPE_ICON = LV_VIEW_ICON,
			VIEW_TYPE_LIST = LV_VIEW_LIST,
			VIEW_TYPE_SMALLICON = LV_VIEW_SMALLICON,
			VIEW_TYPE_TILE = LV_VIEW_TILE,
		};

		struct Column {
			enum Masks : uInt {
				MASK_NONE = 0,
				MASK_FORMAT = LVCF_FMT,
				MASK_WIDTH = LVCF_WIDTH,
				MASK_TEXT = LVCF_TEXT,
				MASK_SUBITEM = LVCF_SUBITEM,
				MASK_IMAGE = LVCF_IMAGE,
				MASK_OFFSET_X = LVCF_ORDER,
				MASK_MINWIDTH = LVCF_MINWIDTH,
				MASK_DEFAULTWIDTH = LVCF_DEFAULTWIDTH,
				MASK_IDEALWIDTH = LVCF_IDEALWIDTH,
			};
			enum Formats {
				FORMAT_NONE = 0,
				FORMAT_LEFT = LVCFMT_LEFT,
				FORMAT_RIGHT = LVCFMT_RIGHT,
				FORMAT_CENTER = LVCFMT_CENTER,
				FORMAT_JUSTIFYMASK = LVCFMT_JUSTIFYMASK,
				FORMAT_IMAGE = LVCFMT_IMAGE,
				FORMAT_BITMAP_ON_RIGHT = LVCFMT_BITMAP_ON_RIGHT,
				FORMAT_HAS_IMAGES = LVCFMT_COL_HAS_IMAGES,
				FORMAT_FIXED_WIDTH = LVCFMT_FIXED_WIDTH,
				FORMAT_NO_DPI_SCALE = LVCFMT_NO_DPI_SCALE,
				FORMAT_FIXED_RATIO = LVCFMT_FIXED_RATIO,
				FORMAT_SPLITBUTTON = LVCFMT_SPLITBUTTON
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
				MASK_COLFMT = LVIF_COLFMT,
				MASK_COLUMNS = LVIF_COLUMNS,
				MASK_DI_SETITEM = LVIF_DI_SETITEM,
				MASK_GROUPID = LVIF_GROUPID,
				MASK_IMAGE = LVIF_IMAGE,
				MASK_INDENT = LVIF_INDENT,
				MASK_NORECOMPUTE = LVIF_NORECOMPUTE,
				MASK_DATA = LVIF_PARAM,
				MASK_STATE = LVIF_STATE,
				MASK_TEXT = LVIF_TEXT
			};
			enum States : uInt {
				STATE_NONE = 0,
				STATE_FOCUSED = LVIS_FOCUSED,
				STATE_SELECTED = LVIS_SELECTED,
				STATE_CUT = LVIS_CUT,
				STATE_DROPHILITED = LVIS_DROPHILITED,
				STATE_GLOW = LVIS_GLOW,
				STATE_ACTIVATING = LVIS_ACTIVATING,
				STATE_OVERLAYMASK = LVIS_OVERLAYMASK,
				STATE_STATEIMAGEMASK = LVIS_STATEIMAGEMASK,
			};
			enum Groups {
				GROUP_NONE = I_GROUPIDNONE,
				GROUP_CALLBACK = I_GROUPIDCALLBACK
			};
			enum ColumnFormats {
				COLUMN_FORMAT_LINE_BREAK = LVCFMT_LINE_BREAK,
				COLUMN_FORMAT_FILL = LVCFMT_FILL,
				COLUMN_FORMAT_WRAP = LVCFMT_WRAP,
				COLUMN_FORMAT_NO_TITLE = LVCFMT_NO_TITLE,
				COLUMN_FORMAT_TILE_PLACEMENTMASK = LVCFMT_TILE_PLACEMENTMASK
			};

			Item() = default;
			Item(const LVITEM& lvItem) :
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
				GroupIndex(lvItem.iGroupId)
			{
			}

			LVITEM ToWinApiStruct() const noexcept {
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
				FLAG_ABOVE = LVHT_ABOVE,
				FLAG_BELOW = LVHT_BELOW,
				FLAG_NOWHERE = LVHT_NOWHERE,
				FLAG_ONITEMICON = LVHT_ONITEMICON,
				FLAG_ONITEMLABEL = LVHT_ONITEMLABEL,
				FLAG_ONITEMSTATEICON = LVHT_ONITEMSTATEICON,
				FLAG_TOLEFT = LVHT_TOLEFT,
				FLAG_TORIGHT = LVHT_TORIGHT,
				FLAG_EX_GROUP_HEADER = LVHT_EX_GROUP_HEADER,
				FLAG_EX_GROUP_FOOTER = LVHT_EX_GROUP_FOOTER,
				FLAG_EX_GROUP_COLLAPSE = LVHT_EX_GROUP_COLLAPSE,
				FLAG_EX_GROUP_BACKGROUND = LVHT_EX_GROUP_BACKGROUND,
				FLAG_EX_GROUP_STATEICON = LVHT_EX_GROUP_STATEICON,
				FLAG_EX_GROUP_SUBSETLINK = LVHT_EX_GROUP_SUBSETLINK,
				FLAG_EX_GROUP = LVHT_EX_GROUP,
				FLAG_EX_ONCONTENTS = LVHT_EX_ONCONTENTS,
				FLAG_EX_FOOTER = LVHT_EX_FOOTER
			};

			Long2D Point;
			Flags Flags;
			Int ItemIndex;
			Int SubItemIndex;
			Int GroupIndex;
		};

		struct Group {
			enum Masks {
				MASK_NONE = LVGF_NONE,
				MASK_HEADER = LVGF_HEADER,
				MASK_FOOTER = LVGF_FOOTER,
				MASK_STATE = LVGF_STATE,
				MASK_ALIGN = LVGF_ALIGN,
				MASK_GROUPID = LVGF_GROUPID,
				MASK_SUBTITLE = LVGF_SUBTITLE,
				MASK_TASK = LVGF_TASK,
				MASK_DESCRIPTIONTOP = LVGF_DESCRIPTIONTOP,
				MASK_DESCRIPTIONBOTTOM = LVGF_DESCRIPTIONBOTTOM,
				MASK_TITLEIMAGE = LVGF_TITLEIMAGE,
				MASK_EXTENDEDIMAGE = LVGF_EXTENDEDIMAGE,
				MASK_ITEMS = LVGF_ITEMS,
				MASK_SUBSET = LVGF_SUBSET,
				MASK_SUBSETITEMS = LVGF_SUBSETITEMS
			};
			enum States {
				STATE_NORMAL = LVGS_NORMAL,
				STATE_COLLAPSED = LVGS_COLLAPSED,
				STATE_HIDDEN = LVGS_HIDDEN,
				STATE_NOHEADER = LVGS_NOHEADER,
				STATE_COLLAPSIBLE = LVGS_COLLAPSIBLE,
				STATE_FOCUSED = LVGS_FOCUSED,
				STATE_SELECTED = LVGS_SELECTED,
				STATE_SUBSETED = LVGS_SUBSETED,
				STATE_SUBSETLINKFOCUSED = LVGS_SUBSETLINKFOCUSED
			};
			enum Aligns {
				ALIGN_NONE = 0,
				ALIGN_FOOTER_CENTER = LVGA_FOOTER_CENTER,
				ALIGN_FOOTER_LEFT = LVGA_FOOTER_LEFT,
				ALIGN_FOOTER_RIGHT = LVGA_FOOTER_RIGHT,
				ALIGN_HEADER_CENTER = LVGA_HEADER_CENTER,
				ALIGN_HEADER_LEFT = LVGA_HEADER_LEFT,
				ALIGN_HEADER_RIGHT = LVGA_HEADER_RIGHT
			};

			Group() = default;
			Group(const LVGROUP& lvGroup) :
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
				ItemsCount(lvGroup.cItems)
			{
			}

			LVGROUP ToWinApiStruct() const noexcept {
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
				MASK_BORDERCOLOR = LVGMF_BORDERCOLOR,
				MASK_BORDERSIZE = LVGMF_BORDERSIZE,
				MASK_NONE = LVGMF_NONE,
				MASK_TEXTCOLOR = LVGMF_TEXTCOLOR
			};

			GroupMetrics() = default;
			GroupMetrics(const LVGROUPMETRICS& metrics) :
				Mask(Masks(metrics.mask)),
				Rect(metrics.Left, metrics.Top, metrics.Right, metrics.Bottom)
			{
			}

			LVGROUPMETRICS ToWinApiStruct() const noexcept {
				LVGROUPMETRICS metrics = { };
				metrics.cbSize = sizeof(metrics);
				metrics.Left = Rect.Left;
				metrics.Top = Rect.Top;
				metrics.Right = Rect.Right;
				metrics.Bottom = Rect.Bottom;
				return metrics;
			}

			Masks Mask = MASK_NONE;
			uIntRect Rect;
		};

		struct TileViewInfo {
			enum Masks : uInt {
				MASK_NONE = 0,
				MASK_TILESIZE = LVTVIM_TILESIZE,
				MASK_COLUMNS = LVTVIM_COLUMNS,
				MASK_LABELMARGIN = LVTVIM_LABELMARGIN
			};
			enum SizeFlags: uInt {
				FLAG_AUTOSIZE = LVTVIF_AUTOSIZE,
				FLAG_EXTENDED = LVTVIF_EXTENDED,
				FLAG_FIXEDWIDTH = LVTVIF_FIXEDWIDTH,
				FLAG_FIXEDHEIGHT = LVTVIF_FIXEDHEIGHT,
				FLAG_FIXEDSIZE = LVTVIF_FIXEDSIZE
			};

			TileViewInfo() = default;
			TileViewInfo(const LVTILEVIEWINFO& info) :
				Mask(Masks(info.dwMask)),
				Flags(SizeFlags(info.dwFlags)),
				Size(info.sizeTile),
				LineCount(info.cLines),
				LabelMargin(info.rcLabelMargin)
			{
			}

			LVTILEVIEWINFO ToWinApiStruct() const noexcept {
				LVTILEVIEWINFO info;
				info.cbSize = sizeof(info);
				info.dwMask = DWord(Mask);
				info.dwFlags = DWord(Flags);
				info.sizeTile = Size;
				info.cLines = LineCount;
				info.rcLabelMargin = LabelMargin;
				return info;
			}

			IntRect LabelMargin;
			Int2D Size;
			Int LineCount = 0;
			Masks Mask = MASK_NONE;
			SizeFlags Flags = FLAG_AUTOSIZE;
		};

		struct TileInfo {
			TileInfo() = default;
			TileInfo(const LVTILEINFO& info) :
				ItemIndex(info.iItem)
			{
				if (info.cColumns > 0) {
					if (info.puColumns == nullptr) {
						ColumnsIndices.resize(info.cColumns);
						memcpy(ColumnsIndices.data(), info.puColumns, info.cColumns * sizeof(uInt));
					}
					if (info.piColFmt == nullptr) {
						ColumnsFormats.resize(info.cColumns);
						memcpy(ColumnsFormats.data(), info.piColFmt, info.cColumns * sizeof(Int));
					}
				}
			}

			LVTILEINFO ToWinApiStruct() const noexcept {
				LVTILEINFO info;
				info.cbSize = sizeof(info);
				info.iItem = ItemIndex;
				info.cColumns = ColumnsIndices.size();
				info.puColumns = PUINT(ColumnsIndices.data());
				info.piColFmt = PINT(ColumnsFormats.data());
				return info;
			}

			Int ItemIndex;
			std::vector<uInt> ColumnsIndices;
			std::vector<Column::Formats> ColumnsFormats;
		};

		using SortCompare = Int(*)(const uInt& leftItemIndex, const uInt& rightItemIndex, const uInt& param);
		using SortGroupCompare = PFNLVGROUPCOMPARE;

	public:
		ListView() = default;
		ListView(const IntRect& windowRect) {
			Create(windowRect);
		}

		void Create(const IntRect& windowRect) {
			Create(windowRect, "");
		}
		void Create(const IntRect& windowRect, const String& name) override {
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

		void AddExtendedStyles(const ExtendedStyles& styles, const ExtendedStyles& mask) noexcept {
			m_ExtendedStyles = ExtendedStyles(m_ExtendedStyles & ~mask);
			m_ExtendedStyles = ExtendedStyles(m_ExtendedStyles | (styles & mask));
			if (m_hwnd)
				ListView_SetExtendedListViewStyleEx(m_hwnd, m_ExtendedStyles, m_ExtendedStyles);
		}

		Int AddItem(const Item& item) {
			m_Items.push_back(item);
			const LVITEM lvItem = (m_Items.end() - 1)->ToWinApiStruct();
			return _SendMessage(LVM_INSERTITEM, 0, reinterpret_cast<Long>(&lvItem));
		}
		Bool RemoveItem(const uInt& index) {
			if (index < m_Items.size())
				m_Items.erase(m_Items.begin() + index);
			return _SendMessage(LVM_DELETEITEM, index, 0);
		}
		Bool ClearItems() {
			m_Items.clear();
			return _SendMessage(LVM_DELETEALLITEMS, 0, 0);
		}

		Bool AddColumn(const Column& column, const uInt& index) {
			return _SendMessage(LVM_INSERTCOLUMN, index, reinterpret_cast<Long>(&column));
		}
		Bool RemoveColumn(const uInt& index) {
			return _SendMessage(LVM_DELETECOLUMN, index, 0);
		}

		Int InsertGroup(const Group& group, const uInt& itemIndex) {
			return _SendMessage(LVM_INSERTGROUP, itemIndex, reinterpret_cast<Long>(&group));
		}
		void InsertGroupSorted(const SortGroupCompare& function, void* pData, const Group& group) {
			LVINSERTGROUPSORTED insertData;
			insertData.lvGroup = group.ToWinApiStruct();
			insertData.pfnGroupCompare = function;
			insertData.pvData = pData;
			_SendMessage(LVM_INSERTGROUPSORTED, reinterpret_cast<uInt>(&insertData), 0);
		}
		Int RemoveGroup(const uInt& groupID) {
			return _SendMessage(LVM_REMOVEGROUP, groupID, 0);
		}
		void RemoveAllGroups() {
			_SendMessage(LVM_REMOVEALLGROUPS, 0, 0);
		}

		void MoveGroup(const uInt& groupID, const uInt& toIndex) {
			_SendMessage(LVM_MOVEGROUP, groupID, toIndex);
		}
		void MoveItemToGroup(const uInt& groupIDFrom, const uInt& groupIDTo) {
			_SendMessage(LVM_MOVEITEMTOGROUP, groupIDFrom, groupIDTo);
		}
		Int EnableGroupView(const Bool& isEnabled) {
			return _SendMessage(LVM_ENABLEGROUPVIEW, isEnabled, 0);
		}
		Bool SortGroups(const SortGroupCompare& function, void* pParam) {
			return _SendMessage(LVM_SORTGROUPS, reinterpret_cast<uInt>(function), reinterpret_cast<Long>(pParam));
		}
		Bool HasGroup(const uInt& groupID) const {
			return _SendMessage(LVM_HASGROUP, groupID, 0);
		};

		Bool Redraw(const uInt& indexFirst, const uInt& indexLast) const {
			return _SendMessage(LVM_REDRAWITEMS, indexFirst, indexLast);
		}
		Bool Update(const uInt& itemIndex) {
			return _SendMessage(LVM_UPDATE, itemIndex, 0);
		}

		Int FindItem(const FindFlags& flags, const std::wstring& searchText, const Long& searchData, const Int2D& position, const Keyboard::Key& key, const uInt& startIndex = -1) const {
			const Bool keyIsLeftOrRightArrow = (key == Keyboard::KEY_LEFT || key == Keyboard::KEY_RIGHT);
			const Bool keyIsUpOrDownArrow = (key == Keyboard::KEY_UP || key == Keyboard::KEY_DOWN);
			const Bool keyIsHomeOrEnd = (key == Keyboard::KEY_HOME || key == Keyboard::KEY_END);
			const Bool keyIsPriorOrNext = (key == Keyboard::KEY_PRIOR || key == Keyboard::KEY_NEXT);

			if (!(keyIsLeftOrRightArrow || keyIsUpOrDownArrow || keyIsHomeOrEnd || keyIsPriorOrNext))
				Log::Warning("This Key is not supported");

			LVFINDINFO findInfo;
			findInfo.flags = flags;
			findInfo.psz = searchText.c_str();
			findInfo.lParam = searchData;
			findInfo.pt = position;
			findInfo.vkDirection = key;
			return _SendMessage(LVM_FINDITEM, startIndex, reinterpret_cast<Long>(&findInfo));
		}
		Bool SortItems(const SortCompare& function, const Long& param) {
			return _SendMessage(LVM_SORTITEMS, param, reinterpret_cast<Long>(function));
		}
		Bool SortItemsEx(const SortCompare& function, const Long& param) {
			return _SendMessage(LVM_SORTITEMSEX, param, reinterpret_cast<Long>(function));
		}

		Int HitTest(HitTextInfo* pHitTextInfo) const {
			return _SendMessage(LVM_HITTEST, -1, reinterpret_cast<Long>(pHitTextInfo));
		}
		Int SubItemHitTest(HitTextInfo* pHitTextInfo) const {
			return _SendMessage(LVM_SUBITEMHITTEST, -1, reinterpret_cast<Long>(pHitTextInfo));
		}

		Bool ToggleVisible(const uInt& listIndex, const Bool& isVisible) const {
			return _SendMessage(LVM_ENSUREVISIBLE, listIndex, MAKELONG(isVisible, 0));
		}
		Bool Scroll(const Int2D& scrollValue) {
			return _SendMessage(LVM_SCROLL, scrollValue.x, scrollValue.y);
		}
		Bool Arrange(const ArrangeTypes& type) {
			return _SendMessage(LVM_ARRANGE, type, 0);
		}

		TextEdit BeginEditLable(const uInt& listIndex) {
			return TextEdit(reinterpret_cast<HWND>(_SendMessage(LVM_EDITLABEL, listIndex, 0)));
		}
		void CancelEditLabel() {
			_SendMessage(LVM_CANCELEDITLABEL, 0, 0);
		}

		ImageList CreateDragImage(const uInt& itemIndex, const Int2D& position) {
			const POINT point = position;
			const Long lParam = reinterpret_cast<Long>(&point);
			const HIMAGELIST hImageList = reinterpret_cast<HIMAGELIST>(_SendMessage(LVM_CREATEDRAGIMAGE, itemIndex, lParam));
			return ImageList(hImageList);
		}
		Bool InsertMarkHitTest(Long2D* pCoordinates, const Bool& insertAfter, const Int& itemIndex) {
			LVINSERTMARK lvInsertMark;
			lvInsertMark.cbSize = sizeof(lvInsertMark);
			lvInsertMark.dwFlags = (insertAfter) ? LVIM_AFTER : 0;
			lvInsertMark.iItem = itemIndex;

			const uInt wParam = reinterpret_cast<uInt>(pCoordinates);
			const Long lParam = reinterpret_cast<Long>(&lvInsertMark);
			return _SendMessage(LVM_INSERTMARKHITTEST, wParam, lParam);
		}

		Int ItemIndexToID(const uInt& itemIndex) const {
			return _SendMessage(LVM_MAPINDEXTOID, itemIndex, 0);
		}
		Int ItemIDToIndex(const uInt& itemID) const {
			return _SendMessage(LVM_MAPIDTOINDEX, itemID, 0);
		}

		Int2D GetApproximateSize() const {
			const DWord size = _SendMessage(LVM_APPROXIMATEVIEWRECT, -1, -1);
			return Int2D(LOWORD(size), HIWORD(size));
		}
		Byte3D GetBackgroundColor() const noexcept {
			return m_BackgroundColor;
		}
		Byte3D GetTextColor() const noexcept {
			return m_TextColor;
		}
		Byte3D GetTextBackgroundColor() const noexcept {
			return m_TextBackgroundColor;
		}
		ImageList GetImageList(const ImageListTypes& type) const {
			const HIMAGELIST handle = reinterpret_cast<HIMAGELIST>(_SendMessage(LVM_GETIMAGELIST, uInt(type), 0));
			return ImageList(handle);
		}
		uInt GetItemCount() const {
			return _SendMessage(LVM_GETITEMCOUNT, 0, 0);
		}
		Bool GetItem(Item* pItem) const {
			if (pItem == nullptr)
				Raise("The item pointer passed is null");

			LVITEM lvItem = pItem->ToWinApiStruct();
			const Bool result = _SendMessage(LVM_GETITEM, 0, reinterpret_cast<Long>(&lvItem));

			(*pItem) = lvItem;
			return result;
		}
		IntRect GetItemRect(const uInt& itemIndex, const ItemRectTypes& type) const {
			RECT itemRect = { };
			itemRect.left = Long(type);
			_SendMessage(LVM_GETITEMRECT, itemIndex, reinterpret_cast<Long>(&itemRect));
			return itemRect;
		}
		IntRect GetItemRectFromGroup(const uInt& itemIndex, const Int& subItemIndex, const uInt& goupID, const ItemRectTypes& type) const {
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
		Item::States GetItemState(const uInt& itemIndex, const Item::States& stateMask) const {
			return Item::States(_SendMessage(LVM_GETITEMSTATE, itemIndex, stateMask));
		}
		String GetItemText(const uInt& itemIndex, const uInt& subItemIndex) const {
			if (m_Styles & ListStyles::LISTSTYLE_OWNERDATA) {
				Log::Warning("ListView::GetItemText is not supported under the LISTSTYLE_OWNERDATA style");
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
		Int2D GetItemSpacing(const Bool& isSmallIcon) const {
			const DWord spacing = _SendMessage(LVM_GETITEMSPACING, isSmallIcon, 0);
			return Int2D(LOWORD(spacing), HIWORD(spacing));
		}
		Bool GetItemPosition(const uInt& itemIndex, Long2D* pPosition) const {
			return _SendMessage(LVM_GETITEMPOSITION, itemIndex, reinterpret_cast<Long>(pPosition));
		}
		Item::States GetCallbackMask() const {
			return Item::States(_SendMessage(LVM_GETCALLBACKMASK, 0, 0));
		}
		Int GetNextItem(const Int& itemIndex, const NextType& type) const {
			return _SendMessage(LVM_GETNEXTITEM, itemIndex, Long(type));
		}
		Int GetStringWidth(const std::wstring& text) const {
			return _SendMessage(LVM_GETSTRINGWIDTH, 0, reinterpret_cast<Long>(text.data()));
		}
		TextEdit GetTextEdit() const {
			const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(LVM_GETEDITCONTROL, 0, 0));
			return TextEdit(hwnd);
		}
		Bool GetColumn(const Int& columnIndex, Column* pColumn) const {
			return _SendMessage(LVM_GETCOLUMN, columnIndex, reinterpret_cast<Long>(pColumn));
		}
		Int GetColumnWidth(const Int& columnIndex) const {
			return _SendMessage(LVM_GETCOLUMNWIDTH, columnIndex, 0);
		}
		//Header GetHeader() const {
		//	const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(LVM_GETHEADER, 0, 0));
		//	return Header(hwnd);
		//}
		IntRect GetViewRect() const {
			RECT rect = { };
			_SendMessage(LVM_GETVIEWRECT, 0, reinterpret_cast<Long>(&rect));
			return rect;
		}
		Int GetTopIndex() const {
			return _SendMessage(LVM_GETTOPINDEX, 0, 0);
		}
		Int GetCountPerPage() const {
			return _SendMessage(LVM_GETCOUNTPERPAGE, 0, 0);
		}
		Bool GetOrigin(Int2D* pOrigin) const {
			return _SendMessage(LVM_GETORIGIN, 0, reinterpret_cast<Long>(pOrigin));
		}
		Bool GetCheckState(const uInt& itemIndex) const {
			return (_SendMessage(LVM_GETITEMSTATE, itemIndex, LVIS_STATEIMAGEMASK) >> 12) - 1;
		}
		Int GetSelectedCount() const {
			return _SendMessage(LVM_GETSELECTEDCOUNT, 0, 0);
		}
		std::wstring GetSearchString() const {
			const uInt length = _SendMessage(LVM_GETISEARCHSTRING, 0, 0);
			std::wstring searchString(L'\0', length);
			ListView_GetISearchString(m_hwnd, searchString.data());
			return searchString;
		}
		ExtendedStyles GetExtendedStyle() const {
			return ExtendedStyles(_SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0));
		}
		IntRect GetSubItemRect(const Int& itemIndex, const Int& subItemIndex, const ItemRectTypes& type) const {
			RECT rect = { };
			rect.left = Long(type);
			rect.top = subItemIndex;
			_SendMessage(LVM_GETSUBITEMRECT, itemIndex, reinterpret_cast<Long>(&rect));
			return rect;
		}
		Bool GetColumnOrderArray(const uInt& columnCount, Int* columnIndexArray) const {
			return _SendMessage(LVM_GETCOLUMNORDERARRAY, columnCount, reinterpret_cast<Long>(columnIndexArray));
		}
		Int GetHotItem() const {
			return _SendMessage(LVM_GETHOTITEM, 0, 0);
		}
		//Cursor GetHotCursor() const {
		//	const HCURSOR hCursor = reinterpret_cast<HCURSOR>(_SendMessage(LVM_GETHOTCURSOR, 0, 0));
		//	return Cursor(hCursor);
		//}
		void GetWorkAreas(const Int& arraySize, LongRect* rectArray) const {
			_SendMessage(LVM_GETWORKAREAS, arraySize, reinterpret_cast<Long>(rectArray));
		}
		uInt GetNumberOfWorkAreas() const {
			uInt count;
			_SendMessage(LVM_GETNUMBEROFWORKAREAS, 0, reinterpret_cast<Long>(&count));
			return count;
		}
		Int GetSelectionMark() const {
			return _SendMessage(LVM_GETSELECTIONMARK, 0, 0);
		}
		uInt GetMouseHoverTime() const {
			return _SendMessage(LVM_GETHOVERTIME, 0, 0);
		}
		Tooltip GetToolTips() const {
			const HWND hwnd = reinterpret_cast<HWND>(_SendMessage(LVM_GETTOOLTIPS, 0, 0));
			return Tooltip(hwnd);
		}
		ViewTypes GetView() const {
			return ViewTypes(_SendMessage(LVM_GETVIEW, 0, 0));
		}

		Bool GetGroup(const uInt& groupID, Group* pGroup) const {
			return (_SendMessage(LVM_GETGROUPINFO, groupID, reinterpret_cast<Long>(pGroup)) != -1);
		}
		Int GetGroupCount() const {
			return _SendMessage(LVM_GETGROUPCOUNT, 0, 0);
		}
		Bool GetGroupByIndex(const uInt& groupID, Group* pGroup) const {
			return _SendMessage(LVM_GETGROUPINFOBYINDEX, groupID, reinterpret_cast<Long>(pGroup));
		}
		IntRect GetGroupRect(const uInt& groupID, const GroupRectTypes& type) const {
			RECT rect = { };
			rect.top = Long(type);
			_SendMessage(LVM_GETGROUPRECT, groupID, reinterpret_cast<Long>(&rect));
			return rect;
		}
		GroupMetrics GetGroupMetrics() const {
			LVGROUPMETRICS metrics;
			metrics.cbSize = sizeof(metrics);
			_SendMessage(LVM_GETGROUPMETRICS, 0, reinterpret_cast<Long>(&metrics));
			return metrics;
		}
		Group::States GetGroupState(const uInt& groupID, const Group::States& mask) const {
			return Group::States(_SendMessage(LVM_GETGROUPSTATE, groupID, Long(mask)));
		}
		uInt GetFocusedGroup() const {
			return _SendMessage(LVM_GETFOCUSEDGROUP, 0, 0);
		}
		TileViewInfo GetTileViewInfo() const {
			LVTILEVIEWINFO info;
			info.cbSize = sizeof(info);
			_SendMessage(LVM_GETTILEVIEWINFO, 0, reinterpret_cast<Long>(&info));
			return info;
		}
		TileInfo GetTileInfo() const {
			LVTILEINFO info;
			info.cbSize = sizeof(info);
			_SendMessage(LVM_GETTILEINFO, 0, reinterpret_cast<Long>(&info));
			return info;
		}
		Int GetInsertMark(const Int& itemIndex, const Bool& isAfter) const {
			LVINSERTMARK lvInsertMask;
			lvInsertMask.cbSize = sizeof(lvInsertMask);
			lvInsertMask.dwFlags = (isAfter) ? LVIM_AFTER : 0;
			lvInsertMask.iItem = itemIndex;
			return _SendMessage(LVM_GETINSERTMARK, 0, reinterpret_cast<Long>(&lvInsertMask));
		}
		IntRect GetInsertMarkRect() const {
			RECT rect = { };
			_SendMessage(LVM_GETINSERTMARKRECT, 0, reinterpret_cast<Long>(&rect));
			return rect;
		}
		Byte3D GetInsertMarkColor() const {
			return ColorRefToVector(_SendMessage(LVM_GETINSERTMARKCOLOR, 0, 0));
		}
		uInt GetSelectedColumn() const {
			return _SendMessage(LVM_GETSELECTEDCOLUMN, 0, 0);
		}
		Byte3D GetOutlineColor() const {
			return ColorRefToVector(_SendMessage(LVM_GETOUTLINECOLOR, 0, 0));
		}
		ImageList GetImageList() const {
			const HIMAGELIST handle = reinterpret_cast<HIMAGELIST>(_SendMessage(LVM_GETIMAGELIST, LVSIL_GROUPHEADER, 0));
			return ImageList(handle);
		}
		std::wstring GetEmptyText() const {
			wChar buffer[0x7FFF];
			_SendMessage(LVM_GETEMPTYTEXT, 0x7FFF, reinterpret_cast<Long>(buffer));
			return buffer;
		}
		IntRect GetFooterRect() const {
			RECT rect = { };
			_SendMessage(LVM_GETFOOTERRECT, 0, reinterpret_cast<Long>(&rect));
			return rect;
		}
		//GetFooterInfo() const {
		//	(BOOL)_SendMessage(LVM_GETFOOTERINFO, (WPARAM)(0), (LPARAM)(plvfi))
		//}
		IntRect GetFooterItemRect(const uInt& itemIndex) const {
			RECT rect = { };
			_SendMessage(LVM_GETFOOTERITEMRECT, itemIndex, reinterpret_cast<Long>(&rect));
			return rect;
		}
		//GetFooterItem() const {
		//	(BOOL)_SendMessage(LVM_GETFOOTERITEM, (WPARAM)(iItem), (LPARAM)(pfi))
		//}
		//GetNextItemIndex() const {
		//	(BOOL)_SendMessage(LVM_GETNEXTITEMINDEX, (WPARAM)(LVITEMINDEX*)(plvii), MAKELPARAM((flags), 0))
		//}
		//GetBackgroundImage() const {
		//	(BOOL)_SendMessage(LVM_GETBKIMAGE, 0, (LPARAM)(plvbki))
		//}
		Bool IsUseUnicode() const {
			return _SendMessage(LVM_GETUNICODEFORMAT, 0, 0);
		}
		Bool IsGroupViewEnabled() const {
			return _SendMessage(LVM_ISGROUPVIEWENABLED, 0, 0);
		}
		//Bool IsItemVisible() const {
		//	(UINT)_SendMessage(LVM_ISITEMVISIBLE, (WPARAM)(index), (LPARAM)0)
		//}

		void SetItemState(const Int& index, const uInt& styles, const uInt& mask) {
			if (!IsCreated())
				Raise("ListView is not created");
			ListView_SetItemState(m_hwnd, index, styles, mask);
		}

		void SortItems(const PFNLVCOMPARE& func, const Long& param) {
			if (!IsCreated())
				Raise("ListView is not created");
			ListView_SortItems(m_hwnd, func, param);
		}

		void EnableTextBackground() noexcept {
			if (!m_EnabledTextBackground) {
				m_EnabledTextBackground = true;
				if (m_hwnd)
					ListView_SetTextBkColor(m_hwnd, VectorToColorRef(m_BackgroundColor));
			}
		}
		void DisableTextBackground() noexcept {
			if (m_EnabledTextBackground) {
				m_EnabledTextBackground = false;
				if (m_hwnd)
					ListView_SetTextBkColor(m_hwnd, CLR_NONE);
			}
		}

		void SetBackgroundColor(const Byte3D& color) override {
			m_BackgroundColor = color;
			if (m_hwnd) {
				Hide();
				ListView_SetBkColor(m_hwnd, VectorToColorRef(m_BackgroundColor));
				Show();
			}
		}
		void SetTextColor(const uInt3D& color) {
			m_TextColor = color;
			if (m_hwnd) {
				ListView_SetTextColor(m_hwnd, VectorToColorRef(m_TextColor));
				InvalidateRect(m_hwnd, nullptr, FALSE);
			}
		}
		void SetTextBackgroundColor(const uInt3D& color) {
			m_TextBackgroundColor = color;
			if (m_hwnd) {
				ListView_SetTextBkColor(m_hwnd, VectorToColorRef(m_TextBackgroundColor));
				InvalidateRect(m_hwnd, nullptr, FALSE);
			}
		}

	private:
		std::vector<Item> m_Items;
		Byte3D m_TextBackgroundColor = { 255, 255, 255 };
		Byte3D m_TextColor;
		ExtendedStyles m_ExtendedStyles;
		Bool m_EnabledTextBackground = true;
	};
}