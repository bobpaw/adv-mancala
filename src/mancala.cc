#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "mancala.h"
using namespace std;

namespace {
	// Static functions, etc.

}  // namespace

namespace mancala {
	// Function definitions, etc.

	Board::Board(Ruleset r): rules(r) {
		for (auto& v : pockets) v = 0;
	}

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

	void Board::save() { 
		fstream fout;
		fout.open("saveState.csv", ios::out | ios::trunc);
		for (int i = 0; i < sizeof(pockets); i++) { 
			fout << pockets[i] << ",";
		}

		fout.close();
	}

	void Board::restoreSave() { fstream fin;
		fin.open("saveState.csv", ios::in);
		string line, value;
		getline(fin, line);
		 
		stringstream s (line);

		int i = 0;
		while (getline(s, value, ',') && i < sizeof(pockets)) { 
			int marbleNum = 0;
			stringstream vs(value);
			vs >> marbleNum;
			pockets[i] = marbleNum;
			i++;
			
		}
		if (i != 14) 
			cout<< "An Error Has Occurred with the Save File, Please Start a New Game"<< endl;
		


	}
	}  // namespace mancala
