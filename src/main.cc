#include "main.h"
#include <iostream>

int main() {
	// let the player pick the ruleset
	int ruleset = -1;
	while (!(ruleset == 0 /* || ruleset == 1 */)) {
		std::cout
				<< "Which ruleset would you like to use? \n  0: Capture\n  1: Avalanche\n> "
				<< std::flush;
		std::cin >> ruleset;
	}

	std::cout << "Starting Game..." << std::endl;

	mancala::Board game;  // im not sure how you would initialize the ruleset
	game.rules = mancala::Board::Ruleset(ruleset);

	for (unsigned i = 1; i < 7; ++i) {
		game[i] = 6;
		game[14 - i] = 6;
	}

	// game loop
	bool endGame = false;
	while (!endGame) {
		// ask player which # to move
		std::cout << "Please make your move: ";
		int moveNum;
		std::cin >> moveNum;

		game.move(moveNum);
	}

	return 0;
}
