#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "mancala.h"

namespace {
// Static functions, etc.
static int modulus(int a, int b) {
	assert(b != 0);
	int mod = a % b;
	if (mod < 0) return mod + b;
	return mod;
}
}  // namespace

namespace mancala {
// Function definitions, etc.

// Precondition: n is in the range [0, pockets.size()).
// Postcondition: n is in the range [0, pockets.size()) and n is not
// their_mancala().
int Board::move_pieces(int n) {
	assert(-1 < n);
	assert(n < static_cast<int>(pockets.size()));

	int hand = pockets[n];
	pockets[n] = 0;
	while (hand > 0) {
		n = modulus(n + 1, static_cast<int>(pockets.size()));
		if (n == their_mancala()) continue;
		--hand;
		++pockets[n];
	}

	assert(-1 < n);
	assert(n < static_cast<int>(pockets.size()));
	assert(n != their_mancala());
	return n;
}

int Board::move(int n) {
	n = modulus(n, static_cast<int>(size()));

	if (pockets[n] == 0) return -1;

	switch (rules) {
	case Ruleset::Capture:

		n = move_pieces(n);

		// If n is the player's mancala, don't swap the active player.
		if (n == my_mancala()) break;

		// If on player's side and an empty pocket, perform capture.
		if (on_my_side(n) && pockets[n] == 1) {
			pockets[my_mancala()] += pockets[pockets.size() - n];
			pockets[pockets.size() - n] = 0;
		}

		swap_player();

		break;
	case Ruleset::Avalanche:

		while (n != my_mancala() && pockets[n] != 1) n = move_pieces(n);

		// If last piece didn't land in active player's mancala swap active player.
		if (n != my_mancala()) swap_player();
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
	} catch (const std::invalid_argument&) {
		std::cerr << "Invalid player value." << std::endl;
		return;  // Possibly recover
	} catch (const std::out_of_range&) {
		std::cerr << "Invalid player value." << std::endl;
		return;  // Possibly recover
	}

	std::getline(fin, line);
	try {
		rules = Ruleset(std::stoi(line));
	} catch (const std::invalid_argument&) {
		std::cerr << "Invalid ruleset value." << std::endl;
		return;  // Possibly recover here.
	} catch (const std::out_of_range&) {
		std::cerr << "Invalid ruleset value." << std::endl;
		return;  // Possibly recover here.
	}

	unsigned i = 0;
	while (std::getline(fin, line) && i < pockets.size()) {
		try {
			pockets[i] = std::stoi(line);
		} catch (const std::invalid_argument&) {
			break;
		} catch (const std::out_of_range&) { break; }
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
