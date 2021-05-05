#include <array>

namespace mancala {
	class Board {
		std::array<int, 14> pockets;

	public:
		int operator[] (int n) const {
			return pockets[n];
		}
	};
}  // namespace mancala
