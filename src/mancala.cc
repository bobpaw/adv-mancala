#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "mancala.h"

namespace {
// Static functions, etc.

}  // namespace

namespace mancala {
// Function definitions, etc.

int Board::move_pieces(int n) {
	int hand = pockets[n];
	pockets[n] = 0;
	while (hand > 0) {
		n = (n + 1) % pockets.size();
		if (n == player * 7) continue;
		--hand;
		++pockets[n];
	}

	return n;
}

int Board::move(int n) {
	if (pockets[n] == 0) return -1;

	switch (rules) {
	case Ruleset::Capture:

		n = move_pieces(n);

		// If n is the player's mancala, don't swap the active player.
		if (n == (player ^ 1) * 7) break;

		// If on player's side and an empty mancala, perform capture.
		if (((player == 0 && n < 7) || (player == 1 && n > 7)) && pockets[n] == 1) {
			pockets[(player ^ 1) * 7] += pockets[pockets.size() - n];
			pockets[pockets.size() - n] = 0;
		}

		// Swap active player
		player ^= 1;

		break;
	case Ruleset::Avalanche:
		
		while (n != (player ^ 1) * 7 && pockets[n] != 1)
			n = move_pieces(n);

		// If last piece didn't land in active player's mancala swap active player.
		if (n != (player ^ 1) * 7) player ^= 1;
		break;
	}
	return 0;
}

void Board::save() const {
	std::ofstream fout;
	fout.open("saveState.dat", std::ios::out | std::ios::trunc);
	fout << player << std::endl;
	fout << int(rules) << std::endl;
	for (unsigned i = 0; i < pockets.size(); ++i) {
		fout << pockets[i] << std::endl;
	}

	fout.close();
}

void Board::load() {
	std::ifstream fin;
	fin.open("saveState.dat");
	std::string line;

	std::getline(fin, line);

	try {
		player = std::stoi(line);
	} catch (std::invalid_argument) {
		std::cerr << "Invalid player value." << std::endl;
		return;  // Possibly recover
	} catch (std::out_of_range) {
		std::cerr << "Invalid player value." << std::endl;
		return;  // Possibly recover
	}

	std::getline(fin, line);
	try {
		rules = Ruleset(std::stoi(line));
	} catch (std::invalid_argument) {
		std::cerr << "Invalid ruleset value." << std::endl;
		return;  // Possibly recover here.
	} catch (std::out_of_range) {
		std::cerr << "Invalid ruleset value." << std::endl;
		return;  // Possibly recover here.
	}

	unsigned i = 0;
	while (std::getline(fin, line) && i < pockets.size()) {
		try {
			pockets[i] = std::stoi(line);
		} catch (std::invalid_argument) { break; } catch (std::out_of_range) {
			break;
		}
		++i;
	}
	if (i != 14)
		std::cerr << "An error has occured while loading your save. Please exit "
								 "and start a new game."
							<< std::endl;
}

std::ostream& operator<<(std::ostream& out, const Board& board) {
	auto old_width = out.width();
	auto old_fill = out.fill();

	int width = 2;

	out << std::setw(width) << std::setfill(' ') << " ";
	out << std::setw(width) << std::setfill('0') << board[0];
	out << std::endl;

	for (unsigned i = 1; i < 14 / 2; ++i) {
		out << std::setw(width) << std::setfill('0') << board[i];
		out << std::setw(width) << std::setfill(' ') << " ";
		out << std::setw(width) << std::setfill('0') << board[14 - i];
		out << std::endl;
	}
	out << std::setw(width) << std::setfill(' ') << " ";
	out << std::setw(width) << std::setfill('0') << board[7];

	// Reset fill and width
	out.width(old_width);
	out.fill(old_fill);

	return out;
}
}  // namespace mancala
