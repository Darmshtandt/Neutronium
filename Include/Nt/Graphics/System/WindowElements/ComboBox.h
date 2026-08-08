#pragma once

#include <Nt/Graphics/System/HandleWindow.h>

#ifdef ERROR
#	undef ERROR
#endif

namespace Nt {
	class ComboBox : public HandleWindow {
	public:
		ComboBox() noexcept = default;
		NT_API ComboBox(const IntRect& windowRect);
		~ComboBox() noexcept override = default;

		NT_API void Create(const IntRect& windowRect);
		NT_API void AddElement(const String& element);
		NT_API void RemoveElement(const String& element);
		NT_API void ClearElements();
		NT_API void SetCurrentElement(const uInt& index);

	private:
		std::vector<std::wstring> m_Elements;

	private:
		NT_API void Create(const IntRect& windowRect, [[maybe_unused]] const String& name) override;

#ifdef _WINDEF_
		NT_API static LRESULT CALLBACK _ClassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData);
#endif
	};
}