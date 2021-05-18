#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "mancala.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
	sf::RenderWindow graphics_window(sf::VideoMode(640, 400), "Adv Mancala",
																	 sf::Style::Default,
																	 sf::ContextSettings(0, 0, 0));
	graphics_window.setVerticalSyncEnabled(true);

	sf::Font freesans;
	if (!freesans.loadFromFile(
					(fs::path(argv[0]).parent_path() / "FreeSans.ttf").string())) {
		std::cerr << "Failed to load font FreeSans.ttf." << std::endl;
		return -1;
	}

	sf::Event event;

	sf::Text rule_choice("Capture or Avalanche?", freesans);

	auto bounds = rule_choice.getLocalBounds();
	rule_choice.setOrigin(bounds.left + bounds.width / 2,
												bounds.top + bounds.height / 2);
	rule_choice.setPosition(sf::Vector2f(graphics_window.getSize()) / 2.f);

	while (graphics_window.isOpen()) {
		while (graphics_window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed: graphics_window.close(); break;
			case sf::Event::MouseButtonReleased:
				// Process
				break;
			default:
				// Do nothing
			}
		}

		// Yada yada nobody cares
	}

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
		unsigned moveNum = 0;
		if (line[0] == 'p') {
			try {
				moveNum = std::stoi(line.substr(1));
			} catch (std::invalid_argument) {
				std::cerr << "Invalid argument." << std::endl;
				moveNum = -1u;
			} catch (std::out_of_range) {
				std::cerr << "Input out of range." << std::endl;
				moveNum = -1u;
			}

			if (moveNum >= 1 && moveNum <= 7) {
				auto preview = game.preview(game.player * 7 + moveNum);

				std::cout << "Preview:\n" << preview << std::endl;
			}
		} else {
			try {
				moveNum = std::stoi(line);
			} catch (std::invalid_argument) {
				std::cerr << "Invalid argument." << std::endl;
				moveNum = -1u;
			} catch (std::out_of_range) {
				std::cerr << "Input out of range." << std::endl;
				moveNum = -1u;
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
