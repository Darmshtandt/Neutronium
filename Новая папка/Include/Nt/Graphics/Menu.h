#pragma once

namespace Nt {
	enum MenuFlag {
		MENU_BITMAP = MF_BITMAP,
		MENU_CHECKED = MF_CHECKED,
		MENU_DISABLED = MF_DISABLED,
		MENU_ENABLED = MF_ENABLED,
		MENU_GRAYED = MF_GRAYED,
		MENU_MENUBARBREAK = MF_MENUBARBREAK,
		MENU_MENUBREAK = MF_MENUBREAK,
		MENU_OWNERDRAW = MF_OWNERDRAW,
		MENU_POPUP = MF_POPUP,
		MENU_SEPARATOR = MF_SEPARATOR,
		MENU_STRING = MF_STRING,
		MENU_UNCHECKED = MF_UNCHECKED,
	};

	class Menu {
		struct Item {
			MenuFlag MenuFlags;
			uInt ID;
			std::wstring Text;
		};

		using ItemContainer = std::vector<Item>;
		using SubMenuContainer = std::vector<Menu>;

	public:
		explicit Menu() {
			Create();
		}

		void Add(const MenuFlag& flags, const uInt& id, const String& text) {
			Item newItem;
			newItem.MenuFlags = flags;
			newItem.ID = id;
			newItem.Text = text;
			Add(newItem);
		}
		void Add(const Item& newItem) {
			m_Items.push_back(newItem);
			if (m_hMenu == nullptr)
				Raise("Menu is not created.");
			AppendMenu(m_hMenu, newItem.MenuFlags, newItem.ID, newItem.Text.c_str());
		}
		void AddSubMenu(const Menu& subMenu, const MenuFlag& flags, const String& text) {
			if (m_hMenu == nullptr)
				Raise("Menu is not created.");
			else if (subMenu.m_hMenu == nullptr)
				Raise("Sub menu is not created.");

			m_SubMenus.push_back(subMenu);
			Add(flags, (uInt)subMenu.m_hMenu, text);

			for (const Item& item : subMenu.m_Items)
				m_Items.push_back(item);
		}

		void Create() {
			m_hMenu = CreateMenu();
			if (!m_hMenu)
				Raise("Failed to create menu.");
		}
		void Destroy() {
			if (m_hMenu)
				DestroyMenu(m_hMenu);
		}

		void ChangeItemText(const Menu& menu, const String& newText) {
			ChangeItemText((uInt)menu.m_hMenu, newText);
		}
		void ChangeItemFlags(const Menu& menu, const MenuFlag& flags) {
			ChangeItemFlags((uInt)menu.m_hMenu, flags);
		}
		void ChangeItem(const Menu& menu, const MenuFlag& flags, const String& newText) {
			ChangeItem((uInt)menu.m_hMenu, flags, newText);
		}

		void ChangeItemText(const uInt& id, const String& newText) {
			if (m_hMenu == nullptr)
				Raise("Menu is not created.");

			Item& item = _GetItemFromID(id);
			item.Text = newText;
			_Modify(item.MenuFlags, item.ID, item.Text.c_str());
		}
		void ChangeItemFlags(const uInt& id, const MenuFlag& flags) {
			if (m_hMenu == nullptr)
				Raise("Menu is not created.");

			Item& item = _GetItemFromID(id);
			item.MenuFlags = flags;
			_Modify(item.MenuFlags, item.ID, item.Text.c_str());
		}
		void ChangeItem(const uInt& id, const MenuFlag& flags, const String& newText) {
			if (m_hMenu == nullptr)
				Raise("Menu is not created.");

			Item& item = _GetItemFromID(id);
			item.MenuFlags = flags;
			item.Text = newText;
			_Modify(item.MenuFlags, item.ID, item.Text.c_str());
		}

		void CheckItem(const Menu& menu, const Bool& isChecked) {
			CheckItem((uInt)menu.GetHandle(), isChecked);
		}
		void CheckItem(const uInt& id, const Bool& isChecked) {
			CheckMenuItem(m_hMenu, id, (isChecked) ? MF_CHECKED : MF_UNCHECKED);
		}

#ifdef _WINDEF_
		HMENU GetHandle() const noexcept {
			return m_hMenu;
		}
#endif
		const Menu& GetSubMenu(const uInt& index) const {
			if (index >= m_SubMenus.size())
				Raise("Out of range.");
			return m_SubMenus[index];
		}
		uInt GetSubMenuCount() const {
			return m_SubMenus.size();
		}
		const Item& GetItem(const Menu& menu) const {
			return GetItem((uInt)menu.m_hMenu);
		}
		const Item& GetItem(const uInt& id) const {
			for (const Item& item : m_Items)
				if (item.ID == id)
					return item;
			Raise("Menu ID not find");
			return Item();
		}
		uInt GetItemCount() const {
			return m_Items.size();
		}

	private:
#ifdef _WINDEF_
		HMENU m_hMenu;
#endif
		ItemContainer m_Items;
		SubMenuContainer m_SubMenus;

	private:
		Item& _GetItemFromID(const uInt& id) {
			return const_cast<Item&>(GetItem(id));
		}
		void _Modify(const uInt& flags, const uInt& id, const std::wstring& text) {
			if (!ModifyMenu(m_hMenu, id, flags | MF_BYCOMMAND, id, text.c_str())) {
				String errorMsg = "Failed to modify menu.\nError code: ";
				errorMsg += GetLastError();
				Raise(errorMsg);
			}
		}
	};
}