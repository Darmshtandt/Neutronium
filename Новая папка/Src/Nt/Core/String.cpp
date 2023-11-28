#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>

namespace Nt {
	std::string wStringToString(std::wstring wStr, const uInt& codePage) {
		const uInt size = WideCharToMultiByte(codePage, 0, wStr.c_str(), -1,
			nullptr, 0, nullptr, nullptr) - 1;
		if (size == 0)
			return "";

		std::string result(size, '\0');
		WideCharToMultiByte(codePage, 0, wStr.c_str(), -1,
			result.data(), size, nullptr, nullptr);
		return result;
	}
	std::wstring StringTowString(std::string mStr, const uInt& codePage) {
		const uInt size = MultiByteToWideChar(codePage, 0, mStr.c_str(), -1,
			nullptr, 0) - 1;
		if (size == 0)
			return L"";

		std::wstring result(size, '\0');
		MultiByteToWideChar(codePage, 0, mStr.c_str(), -1,
			result.data(), size);
		return result;
	}

	void String::Assign(const wChar& Value) {
		Char Symbol;
		wctomb(&Symbol, Value);
		std::string::assign({ Symbol });
	}

	String String::ToLower() const noexcept {
		String str;
		for (Char Symbol : (*this))
			str += (Char)std::tolower(Symbol);
		return str;
	}
	String String::ToUpper() const noexcept {
		String str;
		for (Char Symbol : (*this))
			str += (Char)std::toupper(Symbol);
		return str;
	}

	Bool String::IsLowers() const noexcept {
		return _VerifySymbols(islower);
	}
	Bool String::IsUppers() const noexcept {
		return _VerifySymbols(isupper);
	}
	Bool String::IsAlphas() const noexcept {
		return _VerifySymbols(isalpha);
	}
	Bool String::IsAlnums() const noexcept {
		return _VerifySymbols(isalnum);
	}
	Bool String::IsPuncts() const noexcept {
		return _VerifySymbols(ispunct);
	}
	Bool String::IsGraphs() const noexcept {
		return _VerifySymbols(isgraph);
	}
	Bool String::IsBlanks() const noexcept {
		return _VerifySymbols(isblank);
	}
	Bool String::IsCntrls() const noexcept {
		return _VerifySymbols(iscntrl);
	}
	Bool String::IsPrints() const noexcept {
		return _VerifySymbols(isprint);
	}
	Bool String::IsSpaces() const noexcept {
		return _VerifySymbols(isspace);
	}
	Bool String::IsDigits() const noexcept {
		return _VerifySymbols(isdigit);
	}
	Bool String::IsxDigits() const noexcept {
		return _VerifySymbols(isxdigit);
	}
	Bool String::IsIntegral() const noexcept {
		Bool isSkipFirstSymbol = (*begin() == '-');
		for (Char Symbol : (*this))
			if (isSkipFirstSymbol) {
				isSkipFirstSymbol = false;
				continue;
			}
			else if (!isdigit(Symbol)) {
				return false;
			}
		return true;
	}
	Bool String::IsFloat() const noexcept {
		if (length() == 0)
			return false;

		Bool isSkipFirstSymbol = (*begin() == '-');
		uInt DotCount = 0;
		for (Char Symbol : (*this)) {
			if (isSkipFirstSymbol) {
				isSkipFirstSymbol = false;
				continue;
			}
			else if (Symbol == '.') {
				++DotCount;
			}
			else if (!isdigit(Symbol)) {
				return false;
			}

			if (DotCount > 1)
				return false;
		}
		return true;
	}

	std::wstring String::wstr() const noexcept {
		return StringTowString(*this);
	}


	Bool String::operator == (wChar wSymbol) const {
		Char Symbol;
		wctomb(&Symbol, wSymbol);
		return (Symbol == *begin());
	}
	Bool String::operator == (cwString wStr) const {
		const uInt Length = lstrlenW(wStr);
		if (Length != length())
			return false;

		std::unique_ptr<Char*> Str = std::make_unique<Char*>(new Char[Length + 1]);
		wcstombs(*Str.get(), wStr, Length + 1);
		return (*this == *Str);
	}
	Bool String::operator == (const std::wstring& Str) const {
		return ((*this) == Str.c_str());
	}
	Bool String::operator == (const std::wstring_view& Str) const {
		return (std::wstring(*this) == Str);
	}
	Bool String::operator == (const std::string& Str) const noexcept {
		return (std::string(*this) == Str);
	}
	Bool String::operator == (const Char& Str) const noexcept {
		return (*begin() == Str);
	}
	Bool String::operator == (cString Str) const noexcept {
		return (std::string(*this) == Str);
	}
	Bool String::operator == (const String& Str) const noexcept {
		return (*this == std::string(Str));
	}

	String String::operator + (const std::string& Str) const {
		return (std::string(*this) + Str);
	}
	String String::operator + (const String& Str) const {
		return (std::string(*this) + Str);
	}
	String& String::operator += (const String& Str) {
		std::string::operator += (Str);
		return (*this);
	}
	String& String::operator << (const String& Str) {
		std::string::operator += (Str);
		return (*this);
	}
	String& String::operator << (std::ios_base& (__cdecl* _Pfn)(std::ios_base&)) {
		std::stringstream stream;
		stream << c_str() << _Pfn;
		(*this) = stream.str();
		return (*this);
	}


	String::operator std::wstring() const {
		return wstr();
	}
	String::operator cString() const {
		return c_str();
	}
	String::operator Char() const {
		return *begin();
	}
	String::operator Bool() const {
		if (length() == 0)
			return false;

		const Char Symbol = *begin();
		if (Symbol < '0' || Symbol > '9')
			return static_cast<Bool>(Symbol);
		return static_cast<Bool>(Symbol - '0');
	}
	String::operator Short() const {
		return static_cast<Short>(std::stoi(c_str()));
	}
	String::operator Int() const {
		return std::stoi(c_str());
	}
	String::operator Long() const {
		return std::stol(c_str());
	}
	String::operator LLong() const {
		return std::stoll(c_str());
	}
	String::operator Byte() const {
		return static_cast<Byte>(std::stoi(c_str()));
	}
	String::operator uShort() const {
		return static_cast<uShort>(std::stoi(c_str()));
	}
	String::operator uInt() const {
		return static_cast<uInt>(std::stoi(c_str()));
	}
	String::operator uLong() const {
		return std::stoul(c_str());
	}
	String::operator uLLong() const {
		return std::stoull(c_str());
	}
	String::operator Float() const {
		return std::stof(c_str());
	}
	String::operator Double() const {
		return std::stod(c_str());
	}
	String::operator LDouble() const {
		return std::stold(c_str());
	}
	String::operator sChar() const {
		return (sChar)(*begin());
	}
#	ifdef __cpp_char8_t
	String::operator Char8() const {
		return (Char8)(*begin());
	}
#	endif
	String::operator Char16() const {
		return (Char16)(*begin());
	}
	String::operator Char32() const {
		return (Char32)(*begin());
	}
}