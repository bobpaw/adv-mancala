#include <array>

namespace mancala {
  class Board {
    std::array<int, 14> pockets;

    int operator[] (int n) {
      return pockets[n];
    }
  };
}  // namespace mancala
