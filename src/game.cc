#include <iostream>

#include "game.h"

int main() {
	std::cout << "sizeof(GameFrame): " << sizeof(mancala::GameFrame) << std::endl;
	std::cout << "sizeof(MoveFrame): " << sizeof(mancala::GameFrame::MoveFrame) << std::endl;
	std::cout << "sizeof(Board): " << sizeof(mancala::Board) << std::endl;
}
