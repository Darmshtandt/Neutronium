#include "pch.h"
#include "CppUnitTest.h"
#include <Nt/Core.h>

#include <iostream>

#pragma warning(disable : 4996)
#pragma comment(lib, "Neutronium32d.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Nt;

namespace TestNeutronium {
	TEST_CLASS(TestString) {
	public:
		TEST_METHOD(TestConstructors) {
			try {
				String(Bool(0));
				String(wChar(L'0'));
				String(Char('0'));
				String(Byte(0));
				String(Short(0));
				String(uShort(0));
				String(Int(0));
				String(uInt(0));
				String(Long(0));
				String(uLong(0));
				String(LLong(0));
				String(uLLong(0));
				String(cString("123"));
				String(cwString(L"123"));
				String(std::string("123"));
				String(std::wstring(L"123"));
				String(std::string_view("123"));
				String(std::wstring_view(L"123"));
			}
			catch (const std::runtime_error Error) {
				std::cout 
					<< std::endl
					<< "========================================"
					<< Error.what() 
					<< "========================================"
					<< std::endl;
			}
		}
		TEST_METHOD(TestOperator_Ty) {
			Assert::IsTrue(uInt(String("123.123")) == std::stoi("123.123"));
			Assert::IsTrue(uLong(String("1d23.123")) == std::stoul("1d23.123"));
			Assert::IsTrue(uLLong(String("123d.123")) == std::stoull("123d.123"));
			Assert::IsTrue(uShort(String("123.d123")) == std::stoi("123.d123"));
			Assert::IsTrue(Byte(String("123.d123")) == std::stoi("123.d123"));

			Assert::IsTrue(Int(String("123.123")) == std::stoi("123.123"));
			Assert::IsTrue(Long(String("1d23.123")) == std::stol("1d23.123"));
			Assert::IsTrue(LLong(String("123d.123")) == std::stoll("123d.123"));
			Assert::IsTrue(Short(String("123.d123")) == std::stoi("123.d123"));
			Assert::IsTrue(Bool(String("123.1d23")));

			Assert::IsTrue(Float(String("123.123d")) == std::stof("123.123d"));
			Assert::IsTrue(Double(String("123.123d")) == std::stod("123.123d"));
			Assert::IsTrue(LDouble(String("123.123d")) == std::stold("123.123d"));

			String Str("123.123");
			Assert::IsTrue(uInt(Str) == std::stoi("123.123"));
			Assert::IsTrue(uLong(Str) == std::stoul("123.123"));
			Assert::IsTrue(uLLong(Str) == std::stoull("123.123"));
			Assert::IsTrue(uShort(Str) == std::stoi("123.123"));
			Assert::IsTrue(Byte(Str) == std::stoi("123.123"));

			Assert::IsTrue(Int(Str) == std::stoi("123.123"));
			Assert::IsTrue(Long(Str) == std::stol("123.123"));
			Assert::IsTrue(LLong(Str) == std::stoll("123.123"));
			Assert::IsTrue(Short(Str) == std::stoi("123.123"));
			Assert::IsTrue(Bool(Str));

			Assert::IsTrue(Float(Str) == std::stof("123.123"));
			Assert::IsTrue(Double(Str) == std::stod("123.123"));
			Assert::IsTrue(LDouble(Str) == std::stold("123.123"));
		}
		TEST_METHOD(TestOperatorEqual) {
			Assert::IsTrue(String(true) == true);
			Assert::IsTrue(String("123") == L'1');
			Assert::IsTrue(String(L"123") == '1');
			Assert::IsTrue(String(123) == 123);
			Assert::IsTrue(String(123.4) == 123.4);
			Assert::IsTrue(String(123.4) == 123.4f);
			Assert::IsTrue(String(123.4) == L"123.4");
			Assert::IsTrue(String(L"123.4") == "123.4");
			Assert::IsTrue(String(123.4) == std::string("123.4"));
			Assert::IsTrue(String(123.4) == std::wstring(L"123.4"));

			Assert::IsTrue(String(true) != false);
			//Assert::IsTrue(String("123") != L'2');
			Assert::IsTrue(String(L"123") != '2');
			Assert::IsTrue(String(123) != 122);
			Assert::IsTrue(String(123.4f) != 123.3);
			Assert::IsTrue(String(123.4) != 123.3f);
			Assert::IsTrue(String(123.4) != L"123.3");
			Assert::IsTrue(String(L"123.4") != "123.3");
			Assert::IsTrue(String(123.4) != std::string("123.3"));
			Assert::IsTrue(String(123.4) != std::wstring(L"123.3"));
		}
		TEST_METHOD(TestOtherOperators) {
			String Str1 = "123";
			String Str2 = 123;

			Assert::IsTrue((Str1 + Str2) == "123123");
			Assert::IsTrue((Str1 + Str2) == 123123);

			Str1 += 456;
			Str2 = "789";

			Assert::IsTrue(Str1 == "123456");
			Assert::IsTrue((Str1 + Str2) == 123456789);

			Str1.clear();
			Assert::IsTrue(Str1 == "");

			Str1 = L"123";
			Assert::IsTrue(Str1 != "");
			Str1 = "";
			Assert::IsTrue(Str1 == "");

			Str1 += 1;
			Str1 += '2';
			Str1 += "34";
			Str1 += 5.6;
			Assert::IsTrue(Str1 == "12345.6");
			Assert::IsTrue(Str1 == 12345.6);

			Str1 = 123;
			Str2 = 123;
			Str1 += Str2;
			Str2 += 123;
			Assert::IsTrue(Str1 == Str2);

			Char Chars[] = "123123";
			Char* CharsPtr = Chars;
			for (Char Symbol : Str1) {
				Assert::IsTrue(Symbol == *CharsPtr);
				++CharsPtr;
			}
			for (uInt i = 0; i < Str1.length(); ++i) {
				Assert::IsTrue(Str1[i] == Chars[i]);
			}

			Assert::IsTrue(Str1 == Chars[0]);
		}
	};
}