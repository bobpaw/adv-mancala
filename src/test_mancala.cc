#include <cassert>
#include <iostream>
#include <string>

#include "mancala.h"

constexpr int BOARD_SIZE = 14;

/**
 * Quickly and concisely test board contents.
 *
 * A match string consists of multiple fields of the same width each containing
 * a single value. Numerical values represent themselves. Asterisks (`*`) and
 * question marks (`?`) are interpreted as wild cards and match any value. Open
 * square braces (`[`) hungry match zeroes. For special wildcard values, only
 * the first character in the field must be the special value, but it must still
 * use the entire width. "[0011" is not okay and not equal to "[[0011"
 *
 * @param board The mancala board to check.
 * @param match A match string.
 * @param width The width of values.
 * @return Whether the board matched the match-string.
 */
bool test_board(const mancala::Board& board, std::string match, int width = 1) {
	int board_pos = 0;
	bool equal = true;
	for (std::string::size_type pos = 0;
			 equal && board_pos < BOARD_SIZE && pos < match.size(); pos += width) {
		switch (match[pos]) {
		case '[':
			while (board_pos < BOARD_SIZE && board[board_pos] == 0) ++board_pos;
			break;
		case '?':
		case '*': ++board_pos; continue;
		default:
			equal = board[board_pos] == std::stoi(match.substr(pos, width));
			++board_pos;
		}
	}

	return equal;
}

mancala::Board create_board(std::string init, std::size_t width = 1) {
	mancala::Board ret;
	for (std::size_t i = 0; i < ret.size() && i * width < init.size(); ++i) {
		ret[i] = std::stoi(init.substr(i * width, width));
	}

	return ret;
}

mancala::Board rot_board(mancala::Board board, int rot = 1) {
	for (std::size_t i = 0; i < board.size() - rot; ++i) {
		std::swap(board[i], board[(i + board.size() - rot) % board.size()]);
	}

	return board;
}

int main() {
	mancala::Board b;

	b[1] = 2;
	b[2] = 2;
	b[3] = 2;
	b[4] = 2;

	assert(test_board(b, "02222["));

	b.move(2);
	assert(test_board(b, "02033["));

	b.move(1);
	assert(test_board(b, "00143["));

	b[1] = 4;
	b.move(1);
	assert(test_board(b, "002541["));

	// Test preview
	b.player = 0;
	auto b2 = b.preview(3);
	b.move(3);  // 002052111[
	assert(b == b2);

	// Test avalanche
	mancala::Board ava_b(mancala::Board::Ruleset::Avalanche);
	ava_b[1] = 2;
	ava_b[2] = 3;
	ava_b[3] = 2;
	ava_b[4] = 2;

	ava_b.move(1);
	// Expected moves:
	/*
	* 0: 00 >2<3 2 2 0 0 00 000000 (initial state)
	* 1: 00  0 4>3<2 0 0 ...
	* 2: 00  0 4 0 3 1>1<...
	*/

	assert(test_board(ava_b, "0040311"));

	// Test extra turn
	b.player = 0;
	b[6] = 1;
	b.move(6);
	assert(b.player == 0);

	// Test move fail condition
	assert(b.move(1) == -1);

	// Test initialization
	mancala::Board b3;

	for (unsigned i = 1; i < 7; ++i) {
		b3[i] = 6;
		b3[7 + i] = 6;
	}

	assert(test_board(b3, "06666660666666"));

	// Test output function
	mancala::Board b4 = create_board("0001020304050607080910111213", 2);
	std::cout << b4 << std::endl;
	return 0;
}
