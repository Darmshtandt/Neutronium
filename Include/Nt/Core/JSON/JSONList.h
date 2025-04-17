#pragma once

#include <Nt/Core/JSON/JSONData.h>
#include <Nt/Core/JSON/JSONContainer.h>
#include <list>

namespace Nt {
	class JSONList : public std::list<JSONData>, public JSONContainer {
	public:
		JSONList() = default;
		JSONList(const std::ifstream& file) {
			JSONContainer::Parse(file);
		}
		JSONList(const String& code) {
			Parse(code);
		}

		void Parse(String code) override;
	};
}