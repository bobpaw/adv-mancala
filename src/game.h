#include "mancala.h"

#ifndef MANCALA_GAME_H_
#define MANCALA_GAME_H_

namespace mancala {
class Game {};

enum class FrameType : int { Move = 0, Capture };

class GameFrame {
public:
	struct MoveFrame {
		MoveFrame(int m, int h): move_(m), hand_(h) {}

		int move_;
		int hand_;
	};

	struct CaptureFrame {
		CaptureFrame(int m, int h, int c): move_(m), hand_(h), capture_(c) {}
		CaptureFrame(const Board& b, int m):
				move_(m), hand_(b[m]) { /* TODO: Write me */
		}

		int move_;
		int hand_;
		int capture_;
	};

	GameFrame() = delete;

	GameFrame(int m, int h): type(FrameType::Move), move(m, h) {}
	GameFrame(FrameType f): type(f) {}

	FrameType type;
	union {
		MoveFrame move;
		CaptureFrame capture;
	};
};

bool frame_applies(const Board& b, const GameFrame& gf) {
	switch (gf.type) {
	case FrameType::Move: auto mf = gf.move; return b[mf.move_] == mf.hand_;
	case FrameType::Capture: auto cf = gf.capture; return b[cf.move_] == cf.hand_;
	}
}

bool apply_frame(Board& b, const GameFrame& gf) {
	if (!frame_applies(b, gf)) return false;

	switch (gf.type) {
	case FrameType::Move:
		auto mf = gf.move;
		int ret = b.move(mf.move_);
		return ret == 0 || mf.hand_ == 0;
	case FrameType::Capture:
		// ???
	}
}

}  // namespace mancala

#endif  // MANCALA_GAME_H_
