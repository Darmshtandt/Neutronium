#include <NtStdH.h>
#include <Nt/Core.h>
#include <Nt/Graphics.h>
#include <iostream>
#include <cassert>

#pragma comment(lib, "NeutroniumCore32d")

using namespace Nt;

int main() {
	try {
		Window mainWindow({ 1000, 800 }, "Main window");

		mainWindow.Show();

		Event event;
		while (mainWindow.IsOpened()) {
			while (mainWindow.PeekEvent(&event)) {
				switch (event.Type) {
				case Event::Types::KEY_UP:
					switch (event.Value) {
					case Keyboard::KEY_ESCAPE:
						mainWindow.Close();
						break;
					}
					break;
				}
			}
		}

		//FlexibleList<Int> flexibleList1 = { 7, 8, 9 };
		//assert(flexibleList1.Size() == 3);
		//assert(flexibleList1[0] == 7);
		//assert(flexibleList1[1] == 8);
		//assert(flexibleList1[2] == 9);

		//FlexibleList<Int> flexibleList2 = flexibleList1;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 7);
		//assert(flexibleList2[1] == 8);
		//assert(flexibleList2[2] == 9);

		//flexibleList2[0] = 10;
		//flexibleList2[1] = 11;
		//flexibleList2[2] = 12;

		//assert(flexibleList2[0] == 10);
		//assert(flexibleList2[1] == 11);
		//assert(flexibleList2[2] == 12);

		//flexibleList2.Clear();
		//assert(flexibleList2.Size() == 0);


		//flexibleList2 = { 71, 81, 91 };
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 71);
		//assert(flexibleList2[1] == 81);
		//assert(flexibleList2[2] == 91);

		//std::list<Int> list = { 1, 2, 3 };
		//flexibleList2 = list;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 1);
		//assert(flexibleList2[1] == 2);
		//assert(flexibleList2[2] == 3);

		//std::forward_list<Int> forward_list = { 1, 2, 3 };
		//flexibleList2 = forward_list;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 1);
		//assert(flexibleList2[1] == 2);
		//assert(flexibleList2[2] == 3);

		//std::vector<Int> vector = { 4, 5, 6 };
		//flexibleList2 = vector;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 4);
		//assert(flexibleList2[1] == 5);
		//assert(flexibleList2[2] == 6);

		//std::stack<Int> stack;
		//stack.push(7);
		//stack.push(8);
		//stack.push(9);
		//flexibleList2 = stack;
		//flexibleList2.Assign(stack);
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 7);
		//assert(flexibleList2[1] == 8);
		//assert(flexibleList2[2] == 9);

		//std::set<Int> set = { 1, 2, 3 };
		//flexibleList2 = set;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 1);
		//assert(flexibleList2[1] == 2);
		//assert(flexibleList2[2] == 3);

		//std::multiset<Int> multiset = { 456, 789, 123 };
		//flexibleList2 = multiset;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 456);
		//assert(flexibleList2[1] == 789);
		//assert(flexibleList2[2] == 123);

		//std::unordered_set<Int> un_set = { 753, 861, 942 };
		//flexibleList2 = un_set;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 753);
		//assert(flexibleList2[1] == 861);
		//assert(flexibleList2[2] == 942);

		//std::unordered_multiset<Int> un_multiset = { 4, 5, 6 };
		//flexibleList2 = un_multiset;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 4);
		//assert(flexibleList2[1] == 5);
		//assert(flexibleList2[2] == 6);

		//std::queue<Int> queue;
		//multiset.insert(1);
		//multiset.insert(2);
		//multiset.insert(3);
		//multiset.insert(4);
		//flexibleList2 = queue;
		//assert(flexibleList2.Size() == 4);
		//assert(flexibleList2[0] == 1);
		//assert(flexibleList2[1] == 2);
		//assert(flexibleList2[2] == 3);
		//assert(flexibleList2[3] == 4);

		//std::priority_queue<Int> priority_queue;
		//multiset.insert(19);
		//multiset.insert(29);
		//multiset.insert(39);
		//multiset.insert(49);
		//flexibleList2 = priority_queue;
		//assert(flexibleList2.Size() == 4);
		//assert(flexibleList2[0] == 19);
		//assert(flexibleList2[1] == 29);
		//assert(flexibleList2[2] == 39);
		//assert(flexibleList2[3] == 49);

		//std::deque<Int> deque = { 4, 5, 6 };
		//flexibleList2 = deque;
		//assert(flexibleList2.Size() == 3);
		//assert(flexibleList2[0] == 4);
		//assert(flexibleList2[1] == 5);
		//assert(flexibleList2[2] == 6);

		//Int array[5] = { 1, 5, 9, 7, 3 };
		//flexibleList2 = array;
		//assert(flexibleList2.Size() == 5);
		//assert(flexibleList2[0] == 1);
		//assert(flexibleList2[1] == 5);
		//assert(flexibleList2[2] == 9);
		//assert(flexibleList2[3] == 7);
		//assert(flexibleList2[4] == 3);
	}
	catch (const std::runtime_error Error) {
		std::cout
			<< std::endl
			<< "========================================"
			<< Error.what()
			<< "========================================"
			<< std::endl;
	}
	return 0;
}