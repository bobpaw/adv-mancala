#include <cassert>
#include <iostream>

#include "pockets.h"

struct TestPocket: mancala::Pockets {
	using mancala::Pockets::Pockets;

	void printList() { std::cout << *this << std::endl; }
};

int main() {
	{
		TestPocket p;
		p.append(4);
		p.append(50);
		p.append(43);
		p.insert(1, 3);
		p.printList();
	}
	{
		// Test copy constructor
		mancala::Pockets p = {1, 3, 4, 6, 7};
		auto p_copy(p);
		assert(p == p);
	}
	{
		// Test move constructor
		mancala::Pockets p = {1, 3, 4, 6, 7};
		auto p_move(std::move(p));
		assert(p.size() == 0);
		assert(p_move == mancala::Pockets({1, 3, 4, 6, 7}));
	}
	{
		// Test copy assignment
		mancala::Pockets p2 = {1, 3, 4, 6, 7};
		auto p2_copy = p2;
		assert(p2 == p2);
	}
	{
		// Test move assignment
		mancala::Pockets p3 = {1, 3, 4, 6, 7};
		auto p3_move = std::move(p3);
		assert(p3_move[1] == 3);
		assert(p3.size() == 0);
	}
	{
		TestPocket p;
		p.append(4);
		p.append(50);
		p.append(43);
		p.insert(1, 3);
		p.remove(2);
		p.printList();
	}
	{
		mancala::Pockets p_less = {1, 2, 3, 4, 5}, p_more = {1, 2, 3, 4, 6};
		assert(p_less < p_more);
		assert(!(p_less == p_more));

		mancala::Pockets p_lesser = {1, 3, 4}, p_greater = {1, 3, 4, 5};
		assert(p_lesser < p_greater);
		assert(!(p_lesser == p_greater));
	}
	return 0;
}
