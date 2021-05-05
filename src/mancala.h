#include <array>

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

		/**
		* Move the pieces in a loop.
		* 
		* @param n The pocket to move from.
		* @return The pocket into which the last piece was deposited
		* or -1 if the move failed.
		*/
		int move_pieces (int n);
	public:
		enum class Ruleset: int { Capture = 0, Avalanche = 1 } rules{Ruleset::Capture};

		/**
		* The active player. 0 or 1.
		*/
		int player{0};

		Board(Ruleset r = Ruleset::Capture);

		int operator[] (int n) const { return pockets[n]; }
		int& operator[] (int n) { return pockets[n]; }

		void move (int n);
	};
}  // namespace mancala
