#pragma once

#include <Nt/Graphics/System/HandleWindow.h>
#include <Nt/Graphics/System/Icon.h>

namespace Nt {
	class Tooltip : public HandleWindow {
	public:
		enum Styles : uInt {
			STYLE_NONE = 0x00,
			STYLE_ALWAYSTIP = 0x01,
			STYLE_BALLOON = 0x40,
			STYLE_CLOSE = 0x80,
			STYLE_NOANIMATE = 0x10,
			STYLE_NOFADE = 0x20,
			STYLE_NOPREFIX = 0x02,
			STYLE_USEVISUALSTYLE = 0x100,
		};
		enum DisplayFlags : uInt {
			FLAG_NONE = 0x0000,
			FLAG_ABSOLUTE = 0x0080,
			FLAG_CENTERTIP = 0x0002,
			FLAG_IDISHWND = 0x0001,
			FLAG_PARSELINKS = 0x1000,
			FLAG_RTLREADING = 0x0004,
			FLAG_SUBCLASS = 0x0010,
			FLAG_TRACK = 0x0020,
			FLAG_TRANSPARENT = 0x0100
		};
		enum DelayTimeFlags {
			DELAYFLAG_AUTOOPOP = 2,
			DELAYFLAG_INITIAL = 3,
			DELAYFLAG_RESHOW = 1,
			DELAYFLAG_AUTOMATIC = 0
		};
		enum IconType {
			ICON_NONE = 0,
			ICON_INFO = 1,
			ICON_WARNING = 2,
			ICON_ERROR = 3,
			ICON_INFO_LARGE = 4,
			ICON_WARNING_LARGE = 5,
			ICON_ERROR_LARGE = 6
		};

		struct Info {
			Info() noexcept = default;
#ifdef _WINDEF_
			NT_API Info(const TTTOOLINFO& info) noexcept;

			NT_API TTTOOLINFO ToWinApiStruct() const noexcept;

			HWND ParentHandle = nullptr;
#endif
			std::wstring Text;
			IntRect Rect;
			DisplayFlags Flags = FLAG_NONE;
			uInt ID = 0;
			Long Param = 0;
			void* Reserved = nullptr;
		};
		struct HitTestInfo {
			HitTestInfo() = default;
#ifdef _WINDEF_
			NT_API HitTestInfo(const TTHITTESTINFO& info) noexcept;

			NT_API TTHITTESTINFO ToWinApiStruct() const noexcept;

			HWND ParentHandle;
#endif
			Int2D Point;
			Info ToolInfo;
		};

		struct Title {
			Title() = default;
#ifdef _WINDEF_
			NT_API Title(const TTGETTITLE& title) noexcept;

			NT_API TTGETTITLE ToWinApiStruct() const noexcept;
#endif

			uInt BitmapID = uInt(-1);
			uInt TextLength = 0;
			wChar* pText = nullptr;
		};

	public:
		Tooltip() = default;
		~Tooltip() noexcept override = default;
#ifdef _WINDEF_
		NT_API Tooltip(const HWND& hwnd);
#endif

		NT_API void Create();

		NT_API void AddTool(const HandleWindow& parent, const DisplayFlags& flags, const uInt& id, const String& text);
		NT_API void RemoveTool(const HandleWindow& parent, const uInt& ID);

		NT_API Bool AdjustRect(const Bool& isTextRect, IntRect* pRect) const;

		NT_API void Update();

		NT_API void Hide();
		NT_API void Show();
		NT_API HandleWindow ShowFromPoint(const Int2D& position);

		NT_API Bool ToggleActivate(const Bool& isActive);
		NT_API void ToggleTrackActivate(const HandleWindow& parent, const uInt& id, const Bool& isActive);
		NT_API void TrackPosition(const Int2D& position);

		NT_API Bool HitTest(HitTestInfo* pInfo) const;

		NT_API Info EnumTools(const uInt& parentID);

		NT_API Int2D GeSize(Info* pInfo) const;
		NT_API Bool GetCurrentTool(Info* pInfo) const;
		NT_API Int GetDelayTime(const DelayTimeFlags& flags) const;
		NT_API IntRect GetMargin() const;
		NT_API Int GetMaxTipWidth() const;
		NT_API String GetText(const HandleWindow& parent, const uInt& id) const;
		NT_API Byte3D GetTipBackgroundColor() const;
		NT_API Byte3D GetTipTextColor() const;
		NT_API Title GetTitle() const;
		NT_API uInt GetToolCount() const;
		NT_API Info GetToolInfo() const;

		NT_API Bool SetToolRect(const HandleWindow& parent, const uInt& id, const IntRect& newRect);
		NT_API void SetDelaytime(const DelayTimeFlags& flags, const uInt& delay);
		NT_API void SetMargin(const IntRect& newMargin);
		NT_API Int SetMaxtipWidth(const Int& maxWidth = DEFAULT_VALUE);
		NT_API Byte3D SetTipBackgroundColor(const Byte3D& newColor);
		NT_API Byte3D SetTipTextColor(const Byte3D& newColor);
		NT_API Bool SetTitle(const IconType& iconType, const std::wstring& text);
		NT_API Bool SetTitle(const Icon& icon, const std::wstring& text);
		NT_API void SetToolInfo(const Info& info);
		NT_API void SetWindowTheme(const std::wstring& themeName);
		NT_API void SetTipText(const HandleWindow& parent, const uInt& id, const std::wstring& text);

	private:
		using HandleWindow::GetParent;
		using HandleWindow::SetParent;

		NT_API void Create([[maybe_unused]] const IntRect& windowRect, [[maybe_unused]] const String& name) override;
	};
}