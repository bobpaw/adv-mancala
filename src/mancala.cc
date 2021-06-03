#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "mancala.h"

// Static functions, etc.
namespace {

/**
 * Get a in the range [0, b).
 *
 * modulus(5, 14) => 5
 * modulus(16, 14) => 2
 * modulus(-4, 14) => 10
 */
constexpr int modulus(int a, int b) noexcept {
	int mod = a % b;
	if (mod < 0) return mod + b;
	return mod;
}
}  // namespace

namespace mancala {
const Board::MoveInfo Board::NullMove = {14, 14, std::forward_list<int>()};

constexpr int Board::mod_pocket(int pocket) const noexcept {
	return modulus(pocket, static_cast<int>(pockets.size()));
}

// Precondition: n is in the range [0, pockets.size()).
// Postcondition: n is in the range [0, pockets.size()) and n is not
// their_mancala().
int Board::move_pieces(int n) {
	assert(-1 < n);
	assert(n < int(pockets.size()));

	int hand = pockets[n];
	pockets[n] = 0;
	while (hand > 0) {
		n = mod_pocket(n + 1);
		if (n == their_mancala()) continue;
		--hand;
		++pockets[n];
	}

	assert(-1 < n);
	assert(n < int(pockets.size()));
	assert(n != their_mancala());
	return n;
}

int Board::move(int n) {
	n = mod_pocket(n);

	if (!on_my_side(n) || pockets[n] == 0) return -1;
	last_move_.pocket = n;
	last_move_.data.clear();

	switch (rules) {
	case Ruleset::Capture:

		last_move_.data.push_front(pockets[n]);

		n = move_pieces(n);

		last_move_.final = n;

		// If n is the player's mancala, don't swap the active player.
		if (n == my_mancala()) break;

		// If on player's side and an empty pocket, perform capture.
		if (on_my_side(n) && pockets[n] == 1) {
			last_move_.data.push_front(pockets[pockets.size() - n]);  // std::swap?
			pockets[pockets.size() - n] = 0;
			pockets[my_mancala()] += last_move_.data.front();
		} else {
			last_move_.data.push_front(0);
		}

		swap_player();

		break;
	case Ruleset::Avalanche:

		while (n != my_mancala() && pockets[n] != 1) {
			last_move_.data.push_front(pockets[n]);
			n = move_pieces(n);
		}

		last_move_.final = n;

		// If last piece didn't land in active player's mancala swap active player.
		if (n != my_mancala()) swap_player();
		break;
	}
	return 0;
}

int Board::reverse_move_pieces(int final, int hand) {
	assert(-1 < final);
	assert(final < int(pockets.size()));
	assert(final != their_mancala());

	int n = final + 1;
	for (int h = hand; h > 0; --hand) {
		n = mod_pocket(n - 1);
		if (n == their_mancala()) continue;
		--pockets[n];
	}

	pockets[n] = hand;

	assert(-1 < n);
	assert(n < int(pockets.size()));
	assert(n != their_mancala());
	return n;
}


int Board::reverse_move(int final, int hand, std::forward_list<int> data) {
	// FIXME: Decide how reverse_move affects last_move_
	int n = mod_pocket(final);

	// Move to end.
	if (!on_my_side(n) || pockets[n] == 0) return -1;
	last_move_.pocket = n;
	last_move_.hand = pockets[n];
	// End move to end section.

	switch (rules) {
	case Ruleset::Capture:

		if (n != my_mancala()) swap_player();

		if (data.front() != 0) {
			pockets[my_mancala()] -= data.front();
			pockets[pockets.size() - n] = data.front();
			pockets[n] = 1;
		}

		int hand = *(++data.begin());

		n = reverse_move_pieces(n, hand);

		// <<>>
		n = reverse_move_pieces(n, hand);

		last_move_.final = n;

		// If n is the player's mancala, don't swap the active player.
		if (n == my_mancala()) break;

		// If on player's side and an empty pocket, perform capture.
		if (on_my_side(n) && pockets[n] == 1) {
			last_move_.data = pockets[pockets.size() - n];
			pockets[pockets.size() - n] = 0;
			pockets[my_mancala()] += last_move_.data;
		}

		swap_player();

		break;
	case Ruleset::Avalanche:
		if (n != my_mancala()) swap_player();

		last_move_.final = final;
		for (auto hand : data) { n = reverse_move_pieces(n, hand); }

		// <<>>
		last_move_.data = 0;
		while (n != my_mancala() && pockets[n] != 1) {
			n = move_pieces(n);
			++last_move_.data;
		}

		last_move_.final = n;

		// If last piece didn't land in active player's mancala swap active player.
		if (n != my_mancala()) swap_player();
		break;
	}
	return 0;
}

void Board::unapply_move(const MoveInfo& info) { reverse_move_pieces() }

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
