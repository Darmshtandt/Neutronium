#pragma once

#include <fstream>
#include <Nt/Core/Utilities.h>

namespace Nt {
	struct JSONContainer {
		void Parse(const std::ifstream& file) {
			if (!file.is_open())
				Raise("File not opened");

			String code = std::string(
				std::istreambuf_iterator<char>(file.rdbuf()),
				std::istreambuf_iterator<char>());

			_RemoveSpaces(code);

			Parse(code);
		}

		virtual void Parse(String code) = 0;

	protected:
		void _RemoveSpaces(String& code) const noexcept {
			for (std::string::const_iterator iterator = code.begin(); iterator != code.end();) {
				if (isspace(*iterator) || iscntrl(*iterator))
					iterator = code.erase(iterator);
				else
					++iterator;
			}

			if (code.size() < 2)
				Raise("Incorrect file");
		}
	};
}