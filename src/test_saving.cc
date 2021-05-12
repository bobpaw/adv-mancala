#include <cassert>

#include "mancala.h"

int main() {
	mancala::Board board;
	board[1] = 8;
	board.move(1);

	// State: 00111111110000
	board.save();

	mancala::Board board2;

	board2.load();
	assert(board.player == board2.player);
	assert(board.rules == board2.rules);
	for (int i = 0; i < 14; ++i) assert(board[i] == board2[i]);

	return 0;
}