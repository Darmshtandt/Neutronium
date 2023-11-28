#pragma once

namespace Nt {
	NT_API Int2D GetMonitorSize() noexcept;
	NT_API IntRect GetClientRect(HWND hwnd) noexcept;
	NT_API IntRect GetWindowRect(HWND hwnd) noexcept;
	NT_API Int2D GetCursorPosition() noexcept;
	NT_API HBRUSH CreateSolidBrush(const uInt3D& color) noexcept;
	NT_API COLORREF ToColorRef(const Int3D& color);

	class HandleWindow {
	public:
		NT_API HandleWindow() noexcept;
		HandleWindow(const IntRect& rect, const String& name) {
			Create(rect, name);
		}
		NT_API HandleWindow(const HandleWindow& handle);
		NT_API ~HandleWindow() noexcept;

		virtual void Create(const IntRect& rect, const String& name) = 0;

		NT_API void Resize(const uInt2D& size) noexcept;

		NT_API void Hide() const noexcept;
		NT_API void Show() const noexcept;
		NT_API void ShowMaximized() const noexcept;

		NT_API Bool EnableWindow() noexcept;
		NT_API Bool DisableWindow() noexcept;

		NT_API Int2D ScreenToClient(const Int2D& cursorPosition) const noexcept;

		NT_API void AddStyles(const DWord& styles) noexcept;
		NT_API void RemoveStyles(const DWord& styles) noexcept;
		NT_API void AddExStyles(const DWord& styles) noexcept;
		NT_API void RemoveExStyles(const DWord& styles) noexcept;

		NT_API void EnableMenu() noexcept;
		NT_API void DisableMenu() noexcept;

		NT_API void Destroy() noexcept;

		NT_API virtual void SetBackgroundColor(const uInt3D& color);
		NT_API void SetIcon(const cwString& iconName);
		NT_API void SetIconSmall(const cwString& iconName);

#ifdef _WINDEF_
		NT_API void SetInstance(const HINSTANCE& hInstance) noexcept;
#endif
		NT_API void SetMenu(const Menu& menu) noexcept;
		NT_API void SetID(const Int& newID) noexcept;
		NT_API void SetParent(HandleWindow& parentWindow) noexcept;
		NT_API void SetPosition(const Int2D& newPosition) noexcept;
		NT_API void SetSize(const Int2D& newSize) noexcept;
		NT_API void SetRect(const IntRect& newRect) noexcept;
		NT_API void SetName(const String& name);

#ifdef _WINDEF_
		NT_API HWND GetHandle() const noexcept;
		NT_API HDC GetDC() const noexcept;
		NT_API HINSTANCE GetInstance() const noexcept;
#endif
		NT_API Nt::String GetName() const;
		NT_API Menu GetMenu() const noexcept;
		NT_API IntRect GetRect() const noexcept;
		NT_API HandleWindow& GetParent() const noexcept;
		NT_API Int GetID() const noexcept;
		NT_API void* GetParamPtr() const noexcept;
		NT_API uInt3D GetBackgroundColor() const noexcept;
		NT_API Bool IsShowed() const noexcept;

	protected:
		std::wstring m_Name;
		std::wstring m_ClassName;
		uInt3D m_BackgroundColor;
		HandleWindow* m_pParent;
		HINSTANCE m_hInstance;
		HWND m_hwnd;
		HDC m_hdc;
		Menu m_Menu;
		void* m_pParam;
		IntRect m_Rect;
		Int m_ID;
		uInt m_Styles;
		uInt m_ExStyles;
		Bool m_IsMenuEnabled;
		Bool m_IsWindowEnabled;

	protected:
		NT_API void _CreateWindow();
	};

	NT_API String OpenFileDialog(const String& startPath, const _FILEOPENDIALOGOPTIONS& options);
}