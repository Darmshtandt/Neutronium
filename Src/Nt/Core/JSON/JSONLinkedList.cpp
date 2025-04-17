#include <Nt/Core/JSON/JSONDict.h>

namespace Nt {
	void JSONList::Parse(String code) {
		if (code.front() != '[' || code.back() != ']')
			Raise("Incorrect file");

		const std::vector<String> valuesContainer = code.Split(',');
		for (String value : valuesContainer) {
			if (value.front() == '\"' || value.front() == '\'') {
				if (value.front() != value.back())
					Raise("Incorrect file");

				value.erase(value.begin());
				value.erase(value.end() - 1);

				if (value.ToLower() == "true" || value.ToLower() == "false") {
					if (value != "true" && value != "false")
						Raise("Incorrect file");

					push_back(JSONData(JSONType::BOOL, Bool(value)));
				}
				else {
					push_back(JSONData(JSONType::STRING, std::string(value)));
				}
			}
			else if (value.IsIntegral()) {
				push_back(JSONData(JSONType::INT, Int(value)));
			}
			else if (value.IsFloat()) {
				push_back(JSONData(JSONType::FLOAT, Float(value)));
			}
			else if (value.front() == '{') {
				push_back(JSONData(JSONType::DICT, JSONDict(value)));
			}
			else if (value.front() == '[') {
				push_back(JSONData(JSONType::LIST, JSONList(value)));
			}
		}
	}
}