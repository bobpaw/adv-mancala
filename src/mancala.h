#include <array>
#include <forward_list>
#include <ostream>

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
	int reverse_move_pieces(int final, int hand);

	int my_mancala() const noexcept { return player == 0 ? 7 : 0; }
	int their_mancala() const noexcept { return player == 0 ? 0 : 7; }
	bool on_my_side(int n) const noexcept {
		if (player == 0) return n > their_mancala() && n < my_mancala();
		return n > their_mancala() && n < static_cast<int>(size());
	}

public:
	/**
	 * Force \c pocket to be in range.
	 *
	 * @param pocket An integer
	 * @return \c pocket in the range [0, pockets.size()).
	 */
	constexpr int mod_pocket(int pocket) const noexcept;

	enum class Ruleset : int { Capture = 0, Avalanche = 1 };
	
	Ruleset rules{Ruleset::Capture};

	/**
	 * The active player. 0 or 1.
	 */
	int player{0};

	Board(Ruleset r = Ruleset::Capture):
			pockets{}, rules(r), player(0), last_move_() {}

	int operator[](std::size_t n) const noexcept { return pockets[n]; }
	int& operator[](std::size_t n) noexcept { return pockets[n]; }

	constexpr decltype(pockets)::size_type size() const noexcept {
		return pockets.size();
	}

	/**
	 * Make a move.
	 *
	 * @param n The pocket to move.
	 * @returns -1 if the pocket was empty or \c !on_my_side(n).
	 */
	int move(int n);

	/**
	 * Holds info about a move.
	 *
	 * If the current ruleset is \c Capture, \c data holds the capture amount
	 * followed by the initial hand value. If the current ruleset is \c Avalanche,
	 * \c data holds the hand values in reverse order (the last jump is first).
	 */
	struct MoveInfo {
		unsigned int pocket : 4;      // The pocket number of the move. [0,14)
		unsigned int final : 4;       // The location of the final drop.
		std::forward_list<int> data;  // See MoveInfo.

		MoveInfo(): pocket(14), final(14), data() {}

		template <typename ...Ts>
		MoveInfo(int p, int f, Ts... args): pocket(p), final(f) {
			data = {args...};
		}

		constexpr bool operator==(const MoveInfo& other) const noexcept {
			return pocket == other.pocket && final == other.final &&
						 data == other.data;
		}
	};

	// A nonexistent MoveInfo struct.
	static const MoveInfo NullMove;

	const MoveInfo& last_move() const noexcept { return last_move_; }

	/**
	 * Check if a MoveInfo struct would apply to the current board. Invokes
	 * preview.
	 *
	 * @param move A move info struct to attempt to do on the current board.
	 */
	bool move_applies(const MoveInfo& info) const {
		if (info == NullMove) return false;
		Board other = preview(info.pocket);
		return other.last_move_ == info;
	}

	void apply_move(const MoveInfo& info) {
		// FIXME: Maybe lenient (pocket/hand) check first?
		move(info.pocket);
	}
	void unapply_move(const MoveInfo& info);

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

private:
	// Holds info about the last move() call. Only move() modifies last_move_.
	MoveInfo last_move_;
};
}  // namespace mancala
