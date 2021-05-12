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

	mancala::Board game;
	game.rules = mancala::Board::Ruleset(ruleset);

	for (unsigned i = 1; i < 7; ++i) {
		game[i] = 6;
		game[7 + i] = 6;
	}

	// game loop
	bool endGame = false;
	while (!endGame) {
		// ask player which # to move
		std::cout << game << std::endl;
		std::cout << "Player " << game.player + 1 << ", please make your move (1-6): ";
		unsigned moveNum;
		std::cin >> moveNum;

		if (moveNum - 1 < 6) game.move(game.player * 7 + moveNum);
	}

	return 0;
}
