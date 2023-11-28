#pragma once

namespace Nt {
	template <class _Ty>
	class SelfDeletingPtr {
	public:
		constexpr SelfDeletingPtr(const _Ty* Ptr = nullptr) :
			m_Ptr(const_cast<_Ty*>(Ptr)) {
		}
		constexpr ~SelfDeletingPtr() {
			Delete();
		}

		constexpr void Delete() {
			SAFE_DELETE(&m_Ptr);
		}
		constexpr void MoveTo(SelfDeletingPtr<_Ty>& NewPointer) {
			if (NewPointer.m_Ptr != nullptr)
				Raise(L"NewPointer is already taken by another pointer");
			if (m_Ptr == nullptr)
				Raise(L"This pointer is nullptr");

			NewPointer.m_Ptr = m_Ptr;
			m_Ptr = nullptr;
		}
		
		constexpr SelfDeletingPtr<_Ty> operator = (_Ty* Ptr) {
			if (m_Ptr != nullptr)
				Raise(L"This pointer is already taken by another pointer");
			if (Ptr == nullptr)
				Raise(L"Ptr is nullptr");
			m_Ptr = Ptr;
		}

		constexpr _Ty* operator -> () const noexcept {
			return m_Ptr;
		}
		constexpr _Ty operator * () const noexcept {
			return *m_Ptr;
		}
		constexpr operator _Ty* () const noexcept {
			return m_Ptr;
		}
		constexpr operator const _Ty* () const noexcept {
			return m_Ptr;
		}

		constexpr _Ty* Get() const noexcept {
			return m_Ptr;
		}

	private:
		_Ty* m_Ptr;
	};
}