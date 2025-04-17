#pragma once

#include <Nt/Core/Utilities.h>
#include <map>

namespace Nt {
	class Menu {
	public:
		enum Flag : uLong {
			NONE = 0x00000000L,

			BITMAP = 0x00000004L,
			CHECKED = 0x00000008L,
			DISABLED = 0x00000002L,
			GRAYED = 0x00000001L,
			MENUBARBREAK = 0x00000020L,
			MENUBREAK = 0x00000040L,
			OWNERDRAW = 0x00000100L,
			POPUP = 0x00000010L,
			SEPARATOR = 0x00000800L,

			ENABLED = 0x00000000L,
			STRING = 0x00000000L,
			UNCHECKED = 0x00000000L,
		};

		struct Item {
			Flag Flags = NONE;
			uInt ID = 0;
			std::wstring Text;
		};

		using ItemContainer = std::vector<Item>;
		using SubMenuContainer = std::vector<Menu>;

	public:
		Menu() noexcept = default;
#ifdef _WINDEF_
		NT_API Menu(const HMENU& hMenu);
#endif
		NT_API Menu(const Menu& menu) noexcept;
		NT_API Menu(Menu&& menu) noexcept;
		NT_API ~Menu();

		NT_API void Add(const Flag& flags, const uInt& id, const String& text);
		NT_API void Add(const Item& newItem);
		NT_API void AddSubMenu(const Menu& subMenu, const Flag& flags, const String& text);

		NT_API void Create();
		NT_API void Destroy() noexcept;

		NT_API void ChangeItemText(const Menu& menu, const String& newText);
		NT_API void ChangeItemFlags(const Menu& menu, const Flag& flags);
		NT_API void ChangeItem(const Menu& menu, const Flag& flags, const String& newText);
		
		NT_API void ChangeItemText(const uInt& id, const String& newText);
		NT_API void ChangeItemFlags(const uInt& id, const Flag& flags);
		NT_API void ChangeItem(const uInt& id, const Flag& flags, const String& newText);

		NT_API void CheckItem(const Menu& menu, const Bool& isChecked) const;
		NT_API void CheckItem(const uInt& id, const Bool& isChecked) const;

#ifdef _WINDEF_
		NT_API _NODISCARD HMENU GetHandle() const noexcept;
#endif
		NT_API _NODISCARD const Menu& GetSubMenu(const uInt& index) const;
		NT_API _NODISCARD uInt GetSubMenuCount() const noexcept;
		NT_API _NODISCARD const Item* GetItemPtr(const Menu& menu) const noexcept;
		NT_API _NODISCARD const Item* GetItemPtr(const uInt& id) const noexcept;
		NT_API _NODISCARD uInt GetItemCount() const noexcept;

		NT_API Menu& operator = (const Menu& menu) noexcept;
		NT_API Menu& operator = (Menu&& menu) noexcept;

	private:
#ifdef _WINDEF_
		HMENU m_Handle = nullptr;
#endif
		ItemContainer m_Items = { };
		SubMenuContainer m_SubMenu = { };

	private:
		NT_API _NODISCARD Item* _GetItemPtrFromID(const uInt& id);
		NT_API void _Modify(const uInt& flags, const uInt& id, const std::wstring& text) const;
	};
}