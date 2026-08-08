#pragma once

#include <Nt/Graphics/System/HandleWindow.h>
#include <Nt/Graphics/System/WindowElements/CommonControls.h>

namespace Nt {
	class ListBox : public HandleWindow {
	public:
		ListBox() = default;
		NT_API ListBox(const IntRect& windowRect);
		~ListBox() noexcept override = default;

		NT_API void Create(const IntRect& windowRect);

		NT_API uInt AddItem(const String& text);
		NT_API void RemoveItem(const uInt& index);

		NT_API void SetItemData(const uInt& index, const void* pData);
		NT_API void SetItemHeight(const uInt& index, const uInt& height);

		NT_API String GetText(const uInt& index) const;
		NT_API Int GetSelectionIndex() const;

		template <typename _Ty>
		_Ty* GetItemData(const uInt& index) {
			return reinterpret_cast<_Ty*>(_GetItemData(index));
		}

	private:
		uInt m_ItemCount = 0;

	private:
		NT_API void Create(const IntRect& windowRect, [[maybe_unused]] const String& name) override;

		NT_API Long _GetItemData(const uInt& index);
	};
}