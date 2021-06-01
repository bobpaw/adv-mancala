#include "mancala.h"
#include "game.h"

namespace mancala {
class MoveQueue {
public:
	MoveQueue();

	~MoveQueue();

	
private:
	mancala::Board board_;
	std::deque moves_;
};
}  // namespace mancala
