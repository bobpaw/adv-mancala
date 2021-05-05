#include <cassert>
#include <string>

#include "mancala.h"

/**
* Quickly and concisely test board contents.
* 
* @param board The mancala board to check.
* @param match A null-terminated string to check against. Width must be the
* same for every value and is inferred from string length. Asterisks (`*`) and
* question marks (`?`) are interpreted as single width wild cards and match any value.
* Opening square braces (`[`) hungry match zeroes. Square braces
* must take up the same width as other values.
* @param width The width of values.
* @return Whether the board matched the match-string.
*/
bool test_board (const mancala::Board& board, std::string match, int width = 1) {
	std::string chunk;
	int board_pos = 0;
	bool equal = true;
	for (std::size_type pos = 0; equal && pos < match.size; pos += width) {
		chunk = match.substr(pos, width);
		switch (chunk[0]) {
			case '[':
				while (board[board_pos] == 0) ++board_pos;
				break;
			case '?':
			case '*':
				continue;
			default:
				equal = board[board_pos] == std::stoi(chunk);
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

	b.move(2);

	return 0;
}
