#include <array>
#include <ostream>

#include "pockets.h"

namespace mancala {
class Board {
	/**
	 * The pockets of the mancala board.
	 *
	 * Organized like so:
	 *   00
	 * 01  13
	 * 02  12
	 * 03  11
	 * 04  10
	 * 05  09
	 * 06  08
	 *   07
	 */
	std::array<int, 14> pockets;

	int move_pieces(int n);

	int my_mancala() const noexcept { return player == 0 ? 7 : 0; }
	int their_mancala() const noexcept { return player == 0 ? 0 : 7; }
	bool on_my_side(int n) const noexcept {
		if (player == 0) return n > their_mancala() && n < my_mancala();
		return n > their_mancala() && n < static_cast<int>(size());
	}

	Pockets pockets2;

public:
	enum class Ruleset : int {
		Capture = 0,
		Avalanche = 1
	} rules{Ruleset::Capture};

	/**
	 * The active player. 0 or 1.
	 */
	int player{0};

	Board(Ruleset r = Ruleset::Capture): pockets{}, pockets2(14), rules(r), player(0) {}

	int  operator[](std::size_t n) const noexcept { return pockets[n]; }
	int& operator[](std::size_t n) noexcept { return pockets[n]; }

	decltype(pockets)::size_type size() const noexcept { return pockets.size(); }

	/**
	 * Make a move.
	 *
	 * @param n The pocket to move.
	 * @returns -1 if the pocket was empty or 0 on success.
	 */
	int move(int n);

	/**
	 * Preview a move.
	 *
	 * @param n The pocket to move. Passed through to Board::move.
	 * @returns A copy of the current board after invoking move(n).
	 */
	Board preview(int n) const {
		Board ret = *this;
		ret.move(n);
		return ret;
	}

	// Check if pockets are equal
	bool operator==(Board other) const { return pockets == other.pockets; }
	friend std::ostream& operator<<(std::ostream& out, const Board& board);

	void swap_player() noexcept { player = (player == 0 ? 1 : 0); }

	void save() const;
	void load();
};
}  // namespace mancala
