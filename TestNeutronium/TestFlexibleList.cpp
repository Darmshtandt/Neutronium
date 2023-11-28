#include "pch.h"
#include "CppUnitTest.h"
#include <Nt/Core.h>

#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <set>
#include <unordered_set>
#include <concurrent_unordered_set.h>

#pragma warning(disable : 4996)
#pragma comment(lib, "NeutroniumCore32d.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Nt;

namespace TestNeutronium {
	TEST_CLASS(TestFlexibleList) {
	public:
		TEST_METHOD(TestConstructors) {
			try {
				FlexibleList<Int> flexibleList1 = { 7, 8, 9 };
				Assert::IsTrue(flexibleList1.Size() == 3);
				Assert::IsTrue(flexibleList1[0] == 7);
				Assert::IsTrue(flexibleList1[1] == 8);
				Assert::IsTrue(flexibleList1[2] == 9);

				FlexibleList<Int> flexibleList2 = flexibleList1;
				Assert::IsTrue(flexibleList2.Size() == 3);
				Assert::IsTrue(flexibleList2[0] == 7);
				Assert::IsTrue(flexibleList2[1] == 8);
				Assert::IsTrue(flexibleList2[2] == 9);

				//std::list<Int> list = { 1, 2, 3 };
				//std::vector<Int> vector = { 4, 5, 6 };
				//std::stack<Int> stack = { 7, 8, 9 };
				//std::set<Int> set = { 1, 2, 3 };
				//std::unordered_set<Int> un_set = { 4, 5, 6 };

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

		}
		TEST_METHOD(TestOperatorEqual) {

		}
		TEST_METHOD(TestOtherOperators) {

		}
	};
}