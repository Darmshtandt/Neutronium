#pragma once

#include <Nt/Core/JSON/JSONData.h>
#include <Nt/Core/JSON/JSONList.h>
#include <map>

namespace Nt {
	class JSONDict : public std::map<std::string, JSONData>, public JSONContainer {
	public:
		JSONDict() = default;
		JSONDict(const std::ifstream& file) {
			JSONContainer::Parse(file);
		}
		JSONDict(const String& code) {
			Parse(code);
		}

		void Parse(String code) override {
			if (code.front() != '{' || code.back() != '}')
				Raise("Incorrect file");

			const std::vector<String> membersContainer = code.Split(',');
			for (const String& member : membersContainer) {
				const uInt separator = member.find(':');
				if (separator == uInt(-1))
					Raise("Incorrect file");

				std::string name = member.substr(0, separator);
				if (name.empty())
					Raise("Incorrect file");

				if (name.front() == '\"' || name.front() == '\'') {
					if (name.front() != name.back())
						Raise("Incorrect file");

					name.erase(name.begin());
					name.erase(name.end() - 1);
				}

				String value = member.substr(separator + 1, code.length() - separator);
				if (value.empty())
					Raise("Incorrect file");

				if (value.front() == '\"' || value.front() == '\'') {
					if (value.front() != value.back())
						Raise("Incorrect file");

					value.erase(value.begin());
					value.erase(value.end() - 1);

					if (value.ToLower() == "true" || value.ToLower() == "false") {
						if (value != "true" && value != "false")
							Raise("Incorrect file");

						(*this)[name] = JSONData(JSONType::BOOL, Bool(value));
					}
					else {
						(*this)[name] = JSONData(JSONType::STRING, std::string(value));
					}
				}
				else if (value.IsIntegral()) {
					(*this)[name] = JSONData(JSONType::INT, Int(value));
				}
				else if (value.IsFloat()) {
					(*this)[name] = JSONData(JSONType::FLOAT, Float(value));
				}
				else if (value.front() == '{') {
					(*this)[name] = JSONData(JSONType::DICT, JSONDict(value));
				}
				else if (value.front() == '[') {
					(*this)[name] = JSONData(JSONType::LIST, JSONList(value));
				}
			}
		}
	};
}