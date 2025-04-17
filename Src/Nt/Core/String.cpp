// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Utilities.h>
#include <Nt/Core/WinMinimal.h>

#include <Windows.h>
#include <sstream>

namespace Nt {
	std::string _NODISCARD wStringToString(const std::wstring& wStr, const CharCodePage& codePage) {
		const uInt size = WideCharToMultiByte(uInt(codePage), 0, wStr.c_str(), -1,
			nullptr, 0, nullptr, nullptr) - 1;

		if (size == 0)
			return "";

		std::string result(size, '\0');
		WideCharToMultiByte(uInt(codePage), 0, wStr.c_str(), -1,
			result.data(), size, nullptr, nullptr);

		return result;
	}
	std::wstring _NODISCARD StringTowString(const std::string& mStr, const CharCodePage& codePage) {
		const uInt size = MultiByteToWideChar(uInt(codePage), 0, mStr.c_str(), -1,
			nullptr, 0) - 1;

		if (size == 0)
			return L"";

		std::wstring result(size, '\0');
		MultiByteToWideChar(uInt(codePage), 0, mStr.c_str(), -1,
			result.data(), size);

		return result;
	}

	template <typename _Ty> requires std::is_arithmetic_v<_Ty>
	_Ty StringTo(const Char* string) {
		try {
			if constexpr (std::_Is_nonbool_integral<_Ty>) {
				using UnsignedType = std::make_unsigned_t<_Ty>;

				if constexpr (std::is_same_v<UnsignedType, uChar> || std::is_same_v<UnsignedType, uShort> || std::is_same_v<UnsignedType, uInt>)
					return _Ty(std::stoi(string));
				else if constexpr (std::is_same_v<_Ty, Long>)
					return std::stol(string);
				else if constexpr (std::is_same_v<_Ty, uLong>)
					return std::stoul(string);
				else if constexpr (std::is_same_v<_Ty, LLong>)
					return std::stoll(string);
				else if constexpr (std::is_same_v<_Ty, uLLong>)
					return std::stoull(string);
				else
					static_assert(std::is_same_v<UnsignedType, uChar>, "Unknown type");
			}
			else {
				if constexpr (std::is_same_v<_Ty, Float>)
					return std::stof(string);
				else if constexpr (std::is_same_v<_Ty, Double>)
					return std::stod(string);
				else if constexpr (std::is_same_v<_Ty, LDouble>)
					return std::stold(string);
				else
					static_assert(std::is_same_v<_Ty, Float>, "Unknown type");
			}
		}
		catch (const std::invalid_argument& error) {
			Raise(error.what());
		}

		return _Ty();
	}

	void String::Assign(const wChar& Value) {
		Char symbol = '\0';
		wctomb(&symbol, Value);

		std::string::assign({ symbol });
	}

	_NODISCARD std::vector<String> String::Split(const Char& separator) const {
		std::vector<String> strings;

		Int start = 0;
		Int end = find(separator);

		while (end != -1) {
			strings.emplace_back(substr(start, end - start));

			start = end + 1;
			end = find(separator, start);
		}

		if (uInt(start) != length())
			strings.emplace_back(substr(start, length() - start));

		return strings;
	}

	_NODISCARD String String::ToLower() const noexcept {
		String str;
		for (Char symbol : (*this))
			str += (Char)std::tolower(symbol);

		return str;
	}
	_NODISCARD String String::ToUpper() const noexcept {
		String str;
		for (Char symbol : (*this))
			str += (Char)std::toupper(symbol);

		return str;
	}

	_NODISCARD Bool String::IsLowers() const noexcept {
		return _AllMatch(islower);
	}
	_NODISCARD Bool String::IsUppers() const noexcept {
		return _AllMatch(isupper);
	}
	_NODISCARD Bool String::IsAlphas() const noexcept {
		return _AllMatch(isalpha);
	}
	_NODISCARD Bool String::IsAlnums() const noexcept {
		return _AllMatch(isalnum);
	}
	_NODISCARD Bool String::IsPuncts() const noexcept {
		return _AllMatch(ispunct);
	}
	_NODISCARD Bool String::IsGraphs() const noexcept {
		return _AllMatch(isgraph);
	}
	_NODISCARD Bool String::IsBlanks() const noexcept {
		return _AllMatch(isblank);
	}
	_NODISCARD Bool String::IsCntrls() const noexcept {
		return _AllMatch(iscntrl);
	}
	_NODISCARD Bool String::IsPrints() const noexcept {
		return _AllMatch(isprint);
	}
	_NODISCARD Bool String::IsSpaces() const noexcept {
		return _AllMatch(isspace);
	}
	_NODISCARD Bool String::IsDigits() const noexcept {
		return _AllMatch(isdigit);
	}
	_NODISCARD Bool String::IsxDigits() const noexcept {
		return _AllMatch(isxdigit);
	}
	_NODISCARD Bool String::IsIntegral() const noexcept {
		if (empty())
			return false;

		std::string::const_iterator iterator = begin();
		if ((*iterator) == '-')
			++iterator;

		if (iterator == end())
			return false;

		for (; iterator != end(); ++iterator) {
			if (!isdigit(*iterator))
				return false;
		}

		return true;
	}
	_NODISCARD Bool String::IsFloat() const noexcept {
		if (empty())
			return false;

		std::string::const_iterator iterator = begin();
		if ((*iterator) == '-')
			++iterator;

		if (iterator == end())
			return false;

		uInt dotCount = 0;
		for (; iterator != end(); ++iterator) {
			if ((*iterator) == '.') {
				++dotCount;
			}
			else if (!isdigit(*iterator)) {
				return false;
			}

			if (dotCount > 1)
				return false;
		}

		return true;
	}

	_NODISCARD std::wstring String::wstr() const noexcept {
		return StringTowString(*this);
	}


	_NODISCARD Bool String::operator == (const wChar& wSymbol) const {
		if (empty())
			return false;

		Char symbol;
		wctomb(&symbol, wSymbol);

		return (symbol == front());
	}
	_NODISCARD Bool String::operator == (cwString wStr) const {
		const uInt length = lstrlenW(wStr);
		if (length != std::string::length())
			return false;

		std::unique_ptr<Char*> other = std::make_unique<Char*>(new Char[length + 1]);
		wcstombs(*other.get(), wStr, length + 1);

		return (*this == *other);
	}
	_NODISCARD Bool String::operator == (const std::wstring& str) const {
		return ((*this) == str.c_str());
	}
	_NODISCARD Bool String::operator == (const std::wstring_view& str) const {
		return (std::wstring(*this) == str);
	}
	_NODISCARD Bool String::operator == (const std::string& str) const noexcept {
		return (std::string(*this) == str);
	}
	_NODISCARD Bool String::operator == (const Char& symbol) const noexcept {
		return (front() == symbol);
	}
	_NODISCARD Bool String::operator == (cString str) const noexcept {
		return (std::string(*this) == str);
	}
	_NODISCARD Bool String::operator == (const String& str) const noexcept {
		return (*this == std::string(str));
	}

	_NODISCARD String String::operator + (const Char& Symbol) const {
		return std::string(*this) + Symbol;
	}
	_NODISCARD String String::operator + (cString Str) const {
		return std::string(*this) + Str;
	}
	_NODISCARD String String::operator + (const wChar& symbol) const {
		return (*this) + String(symbol);
	}
	_NODISCARD String String::operator + (cwString str) const {
		return (*this) + String(str);
	}

	_NODISCARD String String::operator + (const std::string& str) const {
		return (std::string(*this) + str);
	}
	_NODISCARD String String::operator + (const String& str) const {
		return (std::string(*this) + str);
	}
	String& String::operator += (const String& str) {
		std::string::operator += (str);
		return (*this);
	}
	String& String::operator << (const String& str) {
		std::string::operator += (str);
		return (*this);
	}
	String& String::operator << (std::ios_base& (__cdecl* _Pfn)(std::ios_base&)) {
		std::stringstream stream;
		stream << c_str() << _Pfn;
		(*this) = stream.str();

		return (*this);
	}

	_NODISCARD Char& String::operator [] (const uInt& index) {
		Assert(index < length(), "Out of range");
		return const_cast<Char*>(c_str())[index];
	}

	_NODISCARD Float String::ToFloat() const {
		return operator Float();
	}

	_NODISCARD String::operator std::wstring() const {
		return wstr();
	}
	_NODISCARD String::operator cString() const {
		return c_str();
	}
	_NODISCARD String::operator Char() const {
		return front();
	}
	_NODISCARD String::operator Bool() const {
		if (empty())
			return false;

		const Char symbol = front();
		if (symbol < '0' || symbol > '9')
			return Bool(symbol);
		return Bool(symbol - '0');
	}
	_NODISCARD String::operator Short() const {
		return StringTo<Short>(c_str());
	}
	_NODISCARD String::operator Int() const {
		return StringTo<Int>(c_str());
	}
	_NODISCARD String::operator Long() const {
		return StringTo<Long>(c_str());
	}
	_NODISCARD String::operator LLong() const {
		return StringTo<LLong>(c_str());
	}
	_NODISCARD String::operator Byte() const {
		return StringTo<Byte>(c_str());
	}
	_NODISCARD String::operator uShort() const {
		return StringTo<uShort>(c_str());
	}
	_NODISCARD String::operator uInt() const {
		return StringTo<uInt>(c_str());
	}
	_NODISCARD String::operator uLong() const {
		return StringTo<uLong>(c_str());
	}
	_NODISCARD String::operator uLLong() const {
		return StringTo<uLLong>(c_str());
	}
	_NODISCARD String::operator Float() const {
		return StringTo<Float>(c_str());
	}
	_NODISCARD String::operator Double() const {
		return StringTo<Double>(c_str());
	}
	_NODISCARD String::operator LDouble() const {
		return StringTo<LDouble>(c_str());
	}
	_NODISCARD String::operator sChar() const {
		return sChar(front());
	}
#	ifdef __cpp_char8_t
	_NODISCARD String::operator Char8() const {
		return Char8(front());
	}
#	endif
	_NODISCARD String::operator Char16() const {
		return Char16(front());
	}
	_NODISCARD String::operator Char32() const {
		return Char32(front());
	}

	_NODISCARD Bool String::_AllMatch(Int(predicate)(Int)) const noexcept {
		if (empty())
			return false;

		for (Char symbol : (*this)) {
			if (!predicate(symbol))
				return false;
		}

		return true;
	}
}