#pragma once

#include <Nt/Core/Utilities.h>

namespace Nt {
	template <typename _Ty>
	class SafePtr {
	public:
		SafePtr(_Ty* ptr) :
			m_Ptr(ptr)
		{
		}
		~SafePtr() {
			if (m_Ptr != nullptr)
				delete(m_Ptr);
		}

		SafePtr<_Ty>& operator = (_Ty* ptr) {
			m_Ptr = ptr;
			return (*this);
		}

		_Ty* operator -> () const {
			return RequireNotNull(m_Ptr);
		}
		_Ty operator * () const {
			return *RequireNotNull(m_Ptr);
		}
		operator _Ty* () const {
			return RequireNotNull(m_Ptr);
		}
		operator const _Ty* () const {
			return RequireNotNull(m_Ptr);
		}

	private:
		mutable _Ty* m_Ptr;
	};
}