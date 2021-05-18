#include "main.h"

int main() {
	// let the player pick the ruleset
	int ruleset = -1;
	while (!std::cin.fail() && !(ruleset == 0 || ruleset == 1)) {
		std::cout << "Which ruleset would you like to use? \n  0: Capture\n  1: "
								 "Avalanche\n> "
							<< std::flush;
		std::cin >> ruleset;
	}

	// Discard newline left in input by std::cin.operator>> call.
	std::cin.ignore();

	std::cout << "Starting Game..." << std::endl;

	mancala::Board game;
	game.rules = mancala::Board::Ruleset(ruleset);

	for (unsigned i = 1; i < 7; ++i) {
		game[i] = 6;
		game[7 + i] = 6;
	}

	// game loop
	bool endGame = false;
	std::string line;

	std::cout << game << "\nPlayer " << game.player + 1;
	std::cout << ", please make your move (1-6).";
	std::cout << " Prepend p to preview.\n > " << std::flush;

	while (!endGame && std::getline(std::cin, line)) {
		int moveNum = 0;
		if (line[0] == 'p') {
			try {
				moveNum = std::stoi(line.substr(1));
			} catch (const std::invalid_argument&) {
				std::cerr << "Invalid argument." << std::endl;
				moveNum = -1;
			} catch (const std::out_of_range&) {
				std::cerr << "Input out of range." << std::endl;
				moveNum = -1;
			}

			if (moveNum >= 1 && moveNum <= 7) {
				auto preview = game.preview(game.player * 7 + moveNum);

				std::cout << "Preview:\n" << preview << std::endl;
			}
		} else {
			try {
				moveNum = std::stoi(line);
			} catch (const std::invalid_argument&) {
				std::cerr << "Invalid argument." << std::endl;
				moveNum = -1;
			} catch (const std::out_of_range&) {
				std::cerr << "Input out of range." << std::endl;
				moveNum = -1;
			}

			if (moveNum >= 1 && moveNum <= 7) {
				game.move(game.player * 7 + moveNum);
				std::cout << game << std::endl;

				std::cout << "Player " << game.player + 1;
				std::cout << ", please make your move (1-6).";
				std::cout << " Prepend p to preview.\n";
			}
		}

		std::cout << "> " << std::flush;
	}

	return 0;
}
