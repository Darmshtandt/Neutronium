#pragma once

#include <Nt/Core/Utilities.h>

namespace Nt {
	template <typename _Ty>
	class WeakSafePtr {
	public:
		WeakSafePtr(_Ty* ptr) :
			m_Ptr(ptr)
		{
		}

		void Delete() {
			if (m_Ptr != nullptr) {
				delete(m_Ptr);

				m_Ptr = nullptr;
			}
		}

		_Ty* Get() const {
			return RequireNotNull(m_Ptr);
		}

		WeakSafePtr<_Ty>& operator = (_Ty* ptr) {
			m_Ptr = ptr;
			return (*this);
		}

		_Ty* operator -> () const {
			return Get();
		}
		_Ty operator * () const {
			return *Get();
		}
		operator _Ty* () const {
			return Get();
		}
		operator const _Ty* () const {
			return Get();
		}

	private:
		mutable _Ty* m_Ptr;
	};
}