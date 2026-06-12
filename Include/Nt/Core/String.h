#pragma once

#include <string>
#include <vector>

#include <Nt/Core/NtDLL.h>

namespace Nt {
	enum class CharCodePage : uInt {
		ACP = 0,
		OEMCP = 1,
		MACCP = 2,
		THREAD_ACP = 3,
		SYMBOL = 42,
		UTF7 = 65000,
		UTF8 = 65001
	};

	template <typename _Ty>
	inline constexpr Bool Is_digit_v = std::_Is_any_of_v<_Ty, Bool, Byte,
		Short, Int, Long, LLong, uShort, uInt, uLong, uLLong>;

	template <class _Ty>
	inline constexpr Bool Is_u_symbol_v = std::_Is_any_of_v<std::remove_cv<_Ty>, 
#ifdef __cpp_char8_t
		Char8,
#endif
		Char16, Char32>;
	
	NT_API NT_NODISCARD std::string wStringToString(const  std::wstring& wStr, const CharCodePage& codePage = CharCodePage::ACP);
	NT_API NT_NODISCARD std::wstring StringToWString(const std::string& mStr, const CharCodePage& codePage = CharCodePage::ACP);

	class String : public std::string {
	public:
		template <typename _Ty> requires (std::is_arithmetic_v<_Ty> && (!Is_u_symbol_v<_Ty>))
		String(const _Ty& value) :
			String(_ToString(value)) 
		{
		}
		String(const Char& value) :
			String(_ToString(value)) 
		{
		}
		String(wChar* pBuffer, const uInt& size) :
			String(std::wstring(pBuffer, size))
		{
		}
		String(Char* pBuffer, const uInt& size) :
			std::string(pBuffer, size)
		{
		}
		String(cString value) :
			std::string((value) ? value : "") 
		{
		}
		String(cwString value) :
			String(std::wstring((value) ? value : L"")) 
		{
		}
		String(const std::wstring& wStr) {
			std::string::assign(wStringToString(wStr));
		}
		String(const std::wstring_view& wStr) {
			std::string::assign(wStringToString(wStr.data()));
		}
		String(const std::string& str) :
			std::string(str) {
		}
		String(const std::string_view& str) :
			std::string(str.data()) 
		{
		}
		String() = default;

		template <typename _Ty> requires (std::is_arithmetic_v<_Ty> && (!Is_u_symbol_v<_Ty>))
		void Assign(const _Ty& value) {
			std::string::assign(_ToString(value));
		}
		NT_API void Assign(const wChar& value);

		NT_API NT_NODISCARD std::vector<String> Split(const Char& separator) const;

		NT_API NT_NODISCARD String ToLower() const noexcept;
		NT_API NT_NODISCARD String ToUpper() const noexcept;

		NT_API NT_NODISCARD Bool IsLowers() const noexcept;
		NT_API NT_NODISCARD Bool IsUppers() const noexcept;
		NT_API NT_NODISCARD Bool IsAlphas() const noexcept;
		NT_API NT_NODISCARD Bool IsAlnums() const noexcept;
		NT_API NT_NODISCARD Bool IsPuncts() const noexcept;
		NT_API NT_NODISCARD Bool IsGraphs() const noexcept;
		NT_API NT_NODISCARD Bool IsBlanks() const noexcept;
		NT_API NT_NODISCARD Bool IsCntrls() const noexcept;
		NT_API NT_NODISCARD Bool IsPrints() const noexcept;
		NT_API NT_NODISCARD Bool IsSpaces() const noexcept;
		NT_API NT_NODISCARD Bool IsDigits() const noexcept;
		NT_API NT_NODISCARD Bool IsxDigits() const noexcept;
		NT_API NT_NODISCARD Bool IsIntegral() const noexcept;
		NT_API NT_NODISCARD Bool IsFloat() const noexcept;

		template <class _Ty> requires (std::is_arithmetic_v<_Ty> &&
			(!std::is_same_v<_Ty, wChar>))
		NT_NODISCARD Bool operator == (const _Ty& value) const {
			return (_Ty(*this) == value);
		}
		NT_API NT_NODISCARD Bool operator == (const wChar& wSymbol) const;
		NT_API NT_NODISCARD Bool operator == (cwString wStr) const;
		NT_API NT_NODISCARD Bool operator == (const std::wstring& str) const;
		NT_API NT_NODISCARD Bool operator == (const std::wstring_view& str) const;
		NT_API NT_NODISCARD Bool operator == (const std::string& str) const noexcept;
		NT_API NT_NODISCARD Bool operator == (const Char& symbol) const noexcept;
		NT_API NT_NODISCARD Bool operator == (cString Str) const noexcept;
		NT_API NT_NODISCARD Bool operator == (const String& str) const noexcept;

		//NT_API NT_NODISCARD String operator + (const Char& symbol) const;
		//NT_API NT_NODISCARD String operator + (cString str) const;
		NT_API NT_NODISCARD String operator + (const wChar& symbol) const;
		NT_API NT_NODISCARD String operator + (cwString str) const;

		//NT_API NT_NODISCARD String operator + (const std::string& str) const;
		//NT_API NT_NODISCARD String operator + (const String& str) const;

		NT_API String& operator += (const String& str);
		NT_API String& operator << (const String& str);
		NT_API String& operator << (std::ios_base& (__cdecl* _Pfn)(std::ios_base&));

		NT_API NT_NODISCARD Char& operator [] (const uInt& index);

		NT_API NT_NODISCARD Float ToFloat() const;

		NT_API NT_NODISCARD operator std::wstring() const;
		NT_API NT_NODISCARD operator cString() const;
		NT_API NT_NODISCARD operator Char() const;
		NT_API NT_NODISCARD operator Bool() const;
		NT_API NT_NODISCARD operator Short() const;
		NT_API NT_NODISCARD operator Int() const;
		NT_API NT_NODISCARD operator Long() const;
		NT_API NT_NODISCARD operator LLong() const;
		NT_API NT_NODISCARD operator Byte() const;
		NT_API NT_NODISCARD operator uShort() const;
		NT_API NT_NODISCARD operator uInt() const;
		NT_API NT_NODISCARD operator uLong() const;
		NT_API NT_NODISCARD operator uLLong() const;
		NT_API NT_NODISCARD operator Float() const;
		NT_API NT_NODISCARD operator Double() const;
		NT_API NT_NODISCARD operator LDouble() const;
		NT_API NT_NODISCARD operator sChar() const;

#ifdef __cpp_char8_t
		NT_API NT_NODISCARD operator Char8() const;
#endif
		NT_API NT_NODISCARD operator Char16() const;
		NT_API NT_NODISCARD operator Char32() const;

	private:
		NT_NODISCARD Bool _AllMatch(Int(predicate)(Int)) const noexcept;

		NT_NODISCARD std::wstring _ToString(const wChar& value) const {
			return std::wstring(1, value);
		}
		NT_NODISCARD std::string _ToString(const Char& value) const {
			return std::string(1, value);
		}

		template <typename _Ty> requires Is_digit_v<_Ty>
		NT_NODISCARD std::string _ToString(const _Ty& value) const {
			return std::to_string(value);
		}
		template <typename _Ty> requires std::is_floating_point_v<_Ty>
		NT_NODISCARD std::string _ToString(const _Ty& value) const {
			std::string result = std::to_string(value);
			std::string::iterator iterator = result.end() - 1;

			while (result.length() > 1 && *(iterator - 1) != '.' && (*iterator) == '0')
				iterator = result.erase(iterator) - 1;

			return result;
		}
	};
}