#pragma once

#include <Nt/Core/JSON/JSONDict.h>

namespace Nt {
	class JSON {
	public:
		JSON() noexcept = default;
		~JSON() {
			SAFE_DELETE(&m_pData);
		}

		void Parse(const std::ifstream& file) {
			SAFE_DELETE(&m_pData);


		}
		void Parse(const Nt::String& code) {
			SAFE_DELETE(&m_pData);


		}

		const JSONData* GetData() const {
			return m_pData;
		}

	private:
		JSONData* m_pData = nullptr;
	};
}