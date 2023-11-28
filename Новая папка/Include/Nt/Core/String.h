#pragma once

#include <windows.h>
#include <string>
#include <codecvt>

namespace Nt {
	template <typename _Ty>
	inline constexpr Bool Is_digit_v = std::_Is_any_of_v<_Ty, Bool, Byte,
		Short, Int, Long, LLong, uShort, uInt, uLong, uLLong>;

	template <class _Ty>
	inline constexpr Bool Is_u_symbol_v = std::_Is_any_of_v<std::remove_cv<_Ty>, 
#ifdef __cpp_char8_t
		Char8,
#endif
		Char16, Char32>;
	
	NT_API std::string wStringToString(std::wstring wStr, const uInt& codePage = CP_ACP);
	NT_API std::wstring StringTowString(std::string mStr, const uInt& codePage = CP_ACP);

	class String : public std::string {
	public:
		template <typename _Ty> requires (std::is_arithmetic_v<_Ty> && (!Is_u_symbol_v<_Ty>))
		String(const _Ty& Value) :
			String(_ToString(Value)) {
		}
		String(cString Value) :
			std::string((Value) ? Value : "") {
		}
		String(cwString Value) :
			String(std::wstring((Value) ? Value : L"")) {
		}
		template <class _Ty> requires (std::_Is_any_of_v<_Ty,
			std::wstring, std::wstring_view>)
		String(const _Ty& wStr) {
			//std::wstring_convert<std::codecvt_utf8_utf16<wChar>, wChar> Convert;
			//std::string::assign(Convert.to_bytes(wStr));
			std::string::assign(wStringToString(wStr));
		}
		template <class _Ty> requires (std::_Is_any_of_v<_Ty,
			std::string, std::string_view>)
		String(const _Ty& Str) :
			std::string(Str) {
		}
		String() = default;

		template <typename _Ty> requires (std::is_arithmetic_v<_Ty> && (!Is_u_symbol_v<_Ty>))
		void Assign(const _Ty& Value) {
			std::string::assign(_ToString(Value));
		}
		NT_API void Assign(const wChar& Value);

		NT_API String ToLower() const noexcept;
		NT_API String ToUpper() const noexcept;

		NT_API Bool IsLowers() const noexcept;
		NT_API Bool IsUppers() const noexcept;
		NT_API Bool IsAlphas() const noexcept;
		NT_API Bool IsAlnums() const noexcept;
		NT_API Bool IsPuncts() const noexcept;
		NT_API Bool IsGraphs() const noexcept;
		NT_API Bool IsBlanks() const noexcept;
		NT_API Bool IsCntrls() const noexcept;
		NT_API Bool IsPrints() const noexcept;
		NT_API Bool IsSpaces() const noexcept;
		NT_API Bool IsDigits() const noexcept;
		NT_API Bool IsxDigits() const noexcept;
		NT_API Bool IsIntegral() const noexcept;
		NT_API Bool IsFloat() const noexcept;

		NT_API std::wstring wstr() const noexcept;
#	ifdef __NT__EXPEREMENTAL
		cwString cw_str() const noexcept {
			return wstr().c_str();
		}
#	endif

		template <class _Ty> requires (std::is_arithmetic_v<_Ty> &&
			(!std::is_same_v<_Ty, wChar>))
		Bool operator == (const _Ty& Value) const {
			return (_Ty(*this) == Value);
		}
		NT_API Bool operator == (wChar wSymbol) const;
		NT_API Bool operator == (cwString wStr) const;
		NT_API Bool operator == (const std::wstring& Str) const;
		NT_API Bool operator == (const std::wstring_view& Str) const;
		NT_API Bool operator == (const std::string& Str) const noexcept;
		NT_API Bool operator == (const Char& Str) const noexcept;
		NT_API Bool operator == (cString Str) const noexcept;
		NT_API Bool operator == (const String& Str) const noexcept;

		String operator + (const Char& Symbol) const {
			return std::string(*this) + Symbol;
		}
		String operator + (cString Str) const {
			return std::string(*this) + Str;
		}
		String operator + (const wChar& Symbol) const {
			return (*this) + String(Symbol);
		}
		String operator + (cwString Str) const {
			return (*this) + String(Str);
		}
		NT_API String operator + (const std::string& Str) const;
		NT_API String operator + (const String& Str) const;
		NT_API String& operator += (const String& Str);
		NT_API String& operator << (const String& Str);
		NT_API String& operator << (std::ios_base& (__cdecl* _Pfn)(std::ios_base&));

		NT_API operator std::wstring() const;
		NT_API operator cString() const;
		NT_API operator Char() const;
		NT_API operator Bool() const;
		NT_API operator Short() const;
		NT_API operator Int() const;
		NT_API operator Long() const;
		NT_API operator LLong() const;
		NT_API operator Byte() const;
		NT_API operator uShort() const;
		NT_API operator uInt() const;
		NT_API operator uLong() const;
		NT_API operator uLLong() const;
		NT_API operator Float() const;
		NT_API operator Double() const;
		NT_API operator LDouble() const;
		NT_API operator sChar() const;
#ifdef __cpp_char8_t
		NT_API operator Char8() const;
#endif
		NT_API operator Char16() const;
		NT_API operator Char32() const;

	private:
		Bool _VerifySymbols(Int(Func)(Int)) const noexcept {
			for (Char Symbol : (*this))
				if (!Func(Symbol))
					return false;
			return true;
		}

		std::wstring _ToString(const wChar& Value) {
			return std::wstring(1, Value);
		}
		std::string _ToString(const Char& Value) {
			return std::string(1, Value);
		}
		template <typename _Ty> requires Is_digit_v<_Ty>
		std::string _ToString(const _Ty& Value) {
			return std::to_string(Value);
		}
		template <typename _Ty> requires std::is_floating_point_v<_Ty>
		std::string _ToString(const _Ty& Value) {
			std::string Result = std::to_string(Value);
			std::string::iterator It = Result.end() - 1;
			while (Result.length() > 0 && *(It - 1) != '.' && *It == '0')
				It = Result.erase(It) - 1;
			return Result;
		}
	};
}