#include <cassert>

#include "mancala.h"

namespace {
	// Static functions, etc.

}  // namespace

namespace mancala {
	// Function definitions, etc.

	int Board::move_pieces (int n) {
		if (pockets[n] == 0) return -1;

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

	void Board::move (int n) {
		switch (rules) {
			case Ruleset::Capture:
				if (pockets[n] == 0) break;

				n = move_pieces(n);

				// If n is the player's mancala, don't swap the active player.
				if (n == (player ^ 1) * 7) break;

				// If on player's side and an empty mancala, perform capture.
				if ((player == 0 && n < 7 || player == 1 && n > 7)
					&& pockets[n] == 1) {
					pockets[(player ^ 1) * 7] += pockets[pockets.size() - n];
					pockets[pockets.size() - n] = 0;
				}

				// Swap active player
				player ^= 1;

				break;
			case Ruleset::Avalanche:
				// TODO: Implement Avalanche mode
				break;
		}
	}
}  // namespace mancala
