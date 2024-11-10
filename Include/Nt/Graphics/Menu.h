#pragma once

namespace Nt {
	class Menu {
	public:
		enum Flags : uInt {
			FLAG_BITMAP = MF_BITMAP,
			FLAG_CHECKED = MF_CHECKED,
			FLAG_DISABLED = MF_DISABLED,
			FLAG_ENABLED = MF_ENABLED,
			FLAG_GRAYED = MF_GRAYED,
			FLAG_MENUBARBREAK = MF_MENUBARBREAK,
			FLAG_MENUBREAK = MF_MENUBREAK,
			FLAG_OWNERDRAW = MF_OWNERDRAW,
			FLAG_POPUP = MF_POPUP,
			FLAG_SEPARATOR = MF_SEPARATOR,
			FLAG_STRING = MF_STRING,
			FLAG_UNCHECKED = MF_UNCHECKED,
		};

		struct Item {
			Flags Flags;
			uInt ID;
			std::wstring Text;
		};

		using ItemContainer = std::vector<Item>;
		using SubMenuContainer = std::vector<Menu>;

	public:
		Menu() {
			Create();
		}
		Menu(const HMENU& hMenu) {
			m_Handle = hMenu;
			if (m_Handle) {
				const uInt itemCount = GetMenuItemCount(m_Handle);
				for (uInt i = 0; i < itemCount; ++i) {
					Item item;
					item.ID = GetMenuItemID(m_Handle, i);
					item.Flags = Flags(GetMenuState(m_Handle, i, MF_BYPOSITION));

					wChar text[MAX_PATH] = { };
					GetMenuString(m_Handle, i, text, MAX_PATH, MF_BYPOSITION);
					item.Text = text;
					m_Items.push_back(item);

					HMENU hSubmenu = ::GetSubMenu(m_Handle, i);
					if (hSubmenu)
						m_SubMenu.push_back(hSubmenu);
				}
			}
		}
		Menu(const Menu& menu) {
			Create();
			for (uInt i = 0; i < menu.m_Items.size(); ++i) {
				Item item = menu.m_Items[i];
				Add(item);
				if (i < menu.m_SubMenu.size())
					AddSubMenu(menu.m_SubMenu[i], item.Flags, item.Text);
			}
		}

		void Add(const Flags& flags, const uInt& id, const String& text) {
			Item newItem;
			newItem.Flags = flags;
			newItem.ID = id;
			newItem.Text = text;
			Add(newItem);
		}
		void Add(const Item& newItem) {
			m_Items.push_back(newItem);
			if (m_Handle == nullptr)
				Raise("Menu is not created.");
			if (!AppendMenu(m_Handle, newItem.Flags, newItem.ID, newItem.Text.c_str()))
				Raise("Failed to add item");
		}
		void AddSubMenu(const Menu& subMenu, const Flags& flags, const String& text) {
			if (m_Handle == nullptr)
				Raise("Menu is not created.");
			else if (subMenu.m_Handle == nullptr)
				Raise("Sub menu is not created.");

			m_SubMenu.push_back(subMenu);
			Add(flags, (uInt)subMenu.m_Handle, text);

			for (const Item& item : subMenu.m_Items)
				m_Items.push_back(item);
		}

		void Create() {
			m_Handle = CreateMenu();
			if (!m_Handle)
				Raise("Failed to create menu.");
		}
		void Destroy() {
			if (m_Handle)
				DestroyMenu(m_Handle);
		}

		void ChangeItemText(const Menu& menu, const String& newText) {
			ChangeItemText((uInt)menu.m_Handle, newText);
		}
		void ChangeItemFlags(const Menu& menu, const Flags& flags) {
			ChangeItemFlags((uInt)menu.m_Handle, flags);
		}
		void ChangeItem(const Menu& menu, const Flags& flags, const String& newText) {
			ChangeItem((uInt)menu.m_Handle, flags, newText);
		}

		void ChangeItemText(const uInt& id, const String& newText) {
			if (m_Handle == nullptr)
				Raise("Menu is not created.");

			Item* pItem = _GetItemPtrFromID(id);
			pItem->Text = newText;
			_Modify(pItem->Flags, pItem->ID, pItem->Text.c_str());
		}
		void ChangeItemFlags(const uInt& id, const Flags& flags) {
			if (m_Handle == nullptr)
				Raise("Menu is not created.");

			Item* pItem = _GetItemPtrFromID(id);
			pItem->Flags = flags;
			_Modify(pItem->Flags, pItem->ID, pItem->Text.c_str());
		}
		void ChangeItem(const uInt& id, const Flags& flags, const String& newText) {
			if (m_Handle == nullptr)
				Raise("Menu is not created.");

			Item* pItem = _GetItemPtrFromID(id);
			pItem->Flags = flags;
			pItem->Text = newText;
			_Modify(pItem->Flags, pItem->ID, pItem->Text.c_str());
		}

		void CheckItem(const Menu& menu, const Bool& isChecked) {
			CheckItem((uInt)menu.GetHandle(), isChecked);
		}
		void CheckItem(const uInt& id, const Bool& isChecked) {
			CheckMenuItem(m_Handle, id, (isChecked) ? MF_CHECKED : MF_UNCHECKED);
		}

#ifdef _WINDEF_
		HMENU GetHandle() const noexcept {
			return m_Handle;
		}
#endif
		const Menu& GetSubMenu(const uInt& index) const {
			if (index >= m_SubMenu.size())
				Raise("Out of range.");
			return m_SubMenu[index];
		}
		uInt GetSubMenuCount() const {
			return m_SubMenu.size();
		}
		const Item* GetItemPtr(const Menu& menu) const {
			return GetItemPtr((uInt)menu.m_Handle);
		}
		const Item* GetItemPtr(const uInt& id) const {
			for (const Item& item : m_Items)
				if (item.ID == id)
					return (&item);

			Raise("Menu ID not find");
			return nullptr;
		}
		uInt GetItemCount() const {
			return m_Items.size();
		}

	private:
#ifdef _WINDEF_
		HMENU m_Handle;
#endif
		ItemContainer m_Items;
		SubMenuContainer m_SubMenu;

	private:
		Item* _GetItemPtrFromID(const uInt& id) {
			return const_cast<Item*>(GetItemPtr(id));
		}
		void _Modify(const uInt& flags, const uInt& id, const std::wstring& text) {
			if (!ModifyMenu(m_Handle, id, flags | MF_BYCOMMAND, id, text.c_str())) {
				String errorMsg = "Failed to modify menu.\nError code: ";
				errorMsg += GetLastError();
				Raise(errorMsg);
			}
		}
	};
}