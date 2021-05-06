#include <cassert>
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
bool test_board (const mancala::Board& board, std::string match, int width = 1) {
	int board_pos = 0;
	bool equal = true;
	for (std::string::size_type pos = 0; equal && board_pos < BOARD_SIZE
		&& pos < match.size(); pos += width) {
		switch (match[pos]) {
			case '[':
				while (board_pos < BOARD_SIZE && board[board_pos] == 0) ++board_pos;
				break;
			case '?':
			case '*':
				++board_pos;
				continue;
			default:
				equal = board[board_pos] == std::stoi(match.substr(pos, width));
				++board_pos;
		}
	}

	return equal;
}

int main () {
	mancala::Board b;

	b[1] = 2;
	b[2] = 2;
	b[3] = 2;
	b[4] = 2;

	assert(test_board(b, "02222"));

	b.move(2);
	assert(test_board(b, "02033"));

	b.move(1);
	assert(test_board(b, "00143"));

	b[1] = 4;
	b.move(1);
	assert(test_board(b, "002541"));

	return 0;
}
