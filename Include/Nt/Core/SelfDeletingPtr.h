#pragma once

#include <Nt/Core/Utilities.h>

namespace Nt {
	template <class _Ty>
	class SelfDeletingPtr {
	public:
		_CONSTEXPR20 SelfDeletingPtr(const _Ty* Ptr = nullptr) :
			m_Ptr(const_cast<_Ty*>(Ptr)) {
		}
		_CONSTEXPR20 ~SelfDeletingPtr() {
			Delete();
		}

		_CONSTEXPR20 void Delete() {
			SAFE_DELETE(&m_Ptr);
		}
		_CONSTEXPR20 void MoveTo(SelfDeletingPtr<_Ty>& newPointer) {
			if (newPointer.m_Ptr != nullptr) {
				Raise("NewPointer is already taken by another pointer");
				return;
			}
			if (m_Ptr == nullptr) {
				Raise("This pointer is nullptr");
				return;
			}

			newPointer.m_Ptr = m_Ptr;
			m_Ptr = nullptr;
		}
		
		_CONSTEXPR20 SelfDeletingPtr<_Ty>& operator = (_Ty* ptr) {
			if (m_Ptr != nullptr) {
				Raise("This pointer is already taken by another pointer");
				return *this;
			}

			if (ptr == nullptr) {
				Raise("Pointer is null");
				return *this;
			}

			m_Ptr = ptr;
			return *this;
		}

		_CONSTEXPR20 _Ty* operator -> () const noexcept {
			return m_Ptr;
		}
		_CONSTEXPR20 _Ty& operator * () const noexcept {
			return *m_Ptr;
		}
		_CONSTEXPR20 operator _Ty* () const noexcept {
			return m_Ptr;
		}
		_CONSTEXPR20 operator const _Ty* () const noexcept {
			return m_Ptr;
		}

		_CONSTEXPR20 _Ty* Get() const noexcept {
			return m_Ptr;
		}

	private:
		_Ty* m_Ptr;
	};
}