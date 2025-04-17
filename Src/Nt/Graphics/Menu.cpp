// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#undef NOUSER

#include <Windows.h>

#include <Nt/Graphics/Menu.h>

namespace Nt {
	std::map<HMENU, Int> g_MenuHandleMap;

	Menu::Menu(const HMENU& hMenu) :
		m_Handle(hMenu)
	{
		if (m_Handle == nullptr)
			return;

		++g_MenuHandleMap[m_Handle];

		const uInt itemCount = GetMenuItemCount(m_Handle);
		for (uInt i = 0; i < itemCount; ++i) {
			Item item;
			item.ID = GetMenuItemID(m_Handle, i);
			item.Flags = Flag(GetMenuState(m_Handle, i, MF_BYPOSITION));

			wChar text[MAX_PATH] = { };
			GetMenuString(m_Handle, i, text, MAX_PATH, MF_BYPOSITION);
			item.Text = text;
			m_Items.push_back(item);

			HMENU hSubmenu = ::GetSubMenu(m_Handle, i);
			if (hSubmenu)
				m_SubMenu.push_back(hSubmenu);
		}
	}
	Menu::Menu(const Menu& menu) noexcept :
		m_Handle(menu.m_Handle),
		m_Items(menu.m_Items),
		m_SubMenu(menu.m_SubMenu)
	{
		if (m_Handle != nullptr)
			++g_MenuHandleMap[m_Handle];
	}
	Menu::Menu(Menu&& menu) noexcept :
		m_Handle(menu.m_Handle),
		m_Items(std::move(menu.m_Items)),
		m_SubMenu(std::move(menu.m_SubMenu))
	{
		menu.m_Handle = nullptr;
	}

	Menu::~Menu() {
		Destroy();
	}

	void Menu::Add(const Flag& flags, const uInt& id, const String& text) {
		Item newItem;
		newItem.Flags = flags;
		newItem.ID = id;
		newItem.Text = text;
		Add(newItem);
	}
	void Menu::Add(const Item& newItem) {
		RequireNotNull(m_Handle, "Menu not created.");

		m_Items.push_back(newItem);
		if (!AppendMenu(m_Handle, newItem.Flags, newItem.ID, newItem.Text.c_str()))
			Raise("Failed to add item");
	}
	void Menu::AddSubMenu(const Menu& subMenu, const Flag& flags, const String& text) {
		RequireNotNull(m_Handle, "Menu not created.");

		m_SubMenu.push_back(RequireNotNull(subMenu.m_Handle));
		Add(flags, (uInt)subMenu.m_Handle, text);

		for (const Item& item : subMenu.m_Items)
			m_Items.push_back(item);
	}

	void Menu::Create() {
		m_Handle = RequireNotNull(CreateMenu(), "Failed to created.");
		g_MenuHandleMap[m_Handle] = 1;
	}
	void Menu::Destroy() noexcept {
		if (m_Handle == nullptr)
			return;

		if ((--g_MenuHandleMap[m_Handle]) <= 0) {
			g_MenuHandleMap.erase(m_Handle);
			DestroyMenu(m_Handle);
		}

		m_Handle = nullptr;
		m_Items.clear();
		m_SubMenu.clear();
	}

	void Menu::ChangeItemText(const Menu& menu, const String& newText) {
		ChangeItemText(uInt(menu.m_Handle), newText);
	}
	void Menu::ChangeItemFlags(const Menu& menu, const Flag& flags) {
		ChangeItemFlags(uInt(menu.m_Handle), flags);
	}
	void Menu::ChangeItem(const Menu& menu, const Flag& flags, const String& newText) {
		ChangeItem(uInt(menu.m_Handle), flags, newText);
	}

	void Menu::ChangeItemText(const uInt& id, const String& newText) {
		RequireNotNull(m_Handle, "Menu not created.");

		Item* pItem = _GetItemPtrFromID(id);
		pItem->Text = newText;

		_Modify(pItem->Flags, pItem->ID, pItem->Text);
	}
	void Menu::ChangeItemFlags(const uInt& id, const Flag& flags) {
		RequireNotNull(m_Handle, "Menu not created.");

		Item* pItem = _GetItemPtrFromID(id);
		pItem->Flags = flags;

		_Modify(pItem->Flags, pItem->ID, pItem->Text);
	}
	void Menu::ChangeItem(const uInt& id, const Flag& flags, const String& newText) {
		RequireNotNull(m_Handle, "Menu not created.");

		Item* pItem = _GetItemPtrFromID(id);
		pItem->Flags = flags;
		pItem->Text = newText;

		_Modify(uInt(pItem->Flags), pItem->ID, pItem->Text);
	}

	void Menu::CheckItem(const Menu& menu, const Bool& isChecked) const {
		CheckItem((uInt)menu.GetHandle(), isChecked);
	}
	void Menu::CheckItem(const uInt& id, const Bool& isChecked) const {
		RequireNotNull(m_Handle, "Menu not created.");
		CheckMenuItem(m_Handle, id, (isChecked) ? MF_CHECKED : MF_UNCHECKED);
	}

	HMENU Menu::GetHandle() const noexcept {
		return m_Handle;
	}

	const Menu& Menu::GetSubMenu(const uInt& index) const {
		if (index >= m_SubMenu.size())
			Raise("Out of range");

		return m_SubMenu[index];
	}
	uInt Menu::GetSubMenuCount() const noexcept {
		return m_SubMenu.size();
	}
	const Menu::Item* Menu::GetItemPtr(const Menu& menu) const noexcept {
		return GetItemPtr(uInt(menu.m_Handle));
	}
	const Menu::Item* Menu::GetItemPtr(const uInt& id) const noexcept {
		for (const Item& item : m_Items) {
			if (item.ID == id)
				return (&item);
		}

		return nullptr;
	}
	uInt Menu::GetItemCount() const noexcept {
		return m_Items.size();
	}

	Menu& Menu::operator = (const Menu& menu) noexcept {
		if (this == &menu)
			return (*this);

		Destroy();

		m_Handle = menu.m_Handle;
		m_Items = menu.m_Items;
		m_SubMenu = menu.m_SubMenu;

		if (m_Handle != nullptr)
			++g_MenuHandleMap[m_Handle];

		return (*this);
	}

	Menu& Menu::operator = (Menu&& menu) noexcept {
		if (this == &menu)
			return (*this);

		Destroy();

		m_Handle = menu.m_Handle;
		m_Items = std::move(menu.m_Items);
		m_SubMenu = std::move(menu.m_SubMenu);
	
		menu.m_Handle = nullptr;

		return (*this);
	}

	Menu::Item* Menu::_GetItemPtrFromID(const uInt& id) {
		return const_cast<Item*>(GetItemPtr(id));
	}
	void Menu::_Modify(const uInt& flags, const uInt& id, const std::wstring& text) const {
		if (!ModifyMenu(m_Handle, id, flags | MF_BYCOMMAND, id, text.c_str()))
			Raise(String("Failed to modify menu.\nError code: ") + String(GetLastError()));
	}
}