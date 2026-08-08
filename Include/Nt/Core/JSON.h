#pragma once

//#include <Nt/Core/JSON/JSONDict.h>

#include <Nt/Core/Utilities.h>

#include <sstream>
#include <fstream>
#include <map>
#include <variant>

namespace Nt {
	class JSON {
	public:
		using Dict = std::map<std::string, JSON>;
		using Array = std::vector<JSON>;
		using Value = std::variant<std::monostate, Dict, Array, std::string, Int, Double, Bool>;

	public:
		JSON() noexcept = default;
		JSON(const Char* string) :
			m_Value(std::string(string))
		{
		}
		
		template <typename _Ty, Requires(std::_Is_any_of_v<_Ty, Dict, Array, std::string>)>
		JSON(const _Ty& value) noexcept :
			m_Value(std::move(value))
		{
		}

		template <typename _Ty, Requires(std::is_arithmetic_v<_Ty>)>
		JSON(const _Ty& value) :
			m_Value(value)
		{
		}

		void Parse(const std::ifstream& file) {
			Assert(file.is_open(), "File not open");

			std::stringstream stream;
			stream << file.rdbuf();

			Parse(stream.str());
		}
		void Parse(const std::string& code) {
			Clear();

			if (code.empty())
				return;

			for (std::string::const_iterator iterator = code.cbegin(); iterator != code.cend(); ++iterator) {
				if (isspace(*iterator) || iscntrl(*iterator))
					continue;

				const std::string dataCode(iterator, code.cend());

				if (*iterator == '{') {
					m_Value = Dict();
					_ParseDict(dataCode);
				}
				else if (*iterator == '[') {
					m_Value = Array();
					_ParseArray(dataCode);
				}
				else {
					_ParseValue(dataCode);
				}
			}
		}

		void Clear() noexcept {
			if (!std::holds_alternative<std::monostate>(m_Value))
				m_Value = std::monostate();
		}

		template <typename _Ty>
		_NODISCARD Bool Is() const noexcept {
			return std::holds_alternative<_Ty>(m_Value);
		}

		_NODISCARD Dict operator [] (const std::string& key) {
			Assert(std::holds_alternative<Dict>(m_Value), "Type error");
			return std::get<Dict>(m_Value)[key];
		}
		_NODISCARD Array operator [] (const uInt& index) {
			Assert(std::holds_alternative<Array>(m_Value), "Type error");

			const Array& array = std::get<Array>(m_Value);
			Assert(array.size() < index, "Out of range");

			return array[index];
		}

		template <typename _Ty>
		_NODISCARD operator const _Ty () const {
			Assert(std::holds_alternative<_Ty>(m_Value), "Type error");
			return std::get<_Ty>(m_Value);
		}

		template <typename _Ty>
		_NODISCARD operator _Ty () {
			Assert(std::holds_alternative<_Ty>(m_Value), "Type error");
			return std::get<_Ty>(m_Value);
		}

	private:
		Value m_Value;

	private:
		void _ParseDict(const std::string& code) {

		}
		void _ParseArray(const std::string& code) {

		}
		void _ParseValue(const std::string& code) {
			if (std::holds_alternative<Dict>(m_Value))
				_ParseDictValue(code);
			else if (std::holds_alternative<Array>(m_Value))
				_ParseArrayValue(code);
			else
				Raise("The JSON is corrupted")
		}

		void _ParseDictValue(const std::string& code) {

		}
		void _ParseArrayValue(const std::string& code) {

		}
	};
}