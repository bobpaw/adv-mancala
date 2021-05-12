#include <iostream>
#include "main.h"

int main() {

  //let the player pick the ruleset
  std::cout << "Which ruleset would you like to use? \n  0: Capture\n  1: Avalanche"<<std::endl;
  int ruleset;
  std::cin >> ruleset;

  std::cout << "Starting Game..."<<std::endl;

  mancala::Board game; // im not sure how you would initialize the ruleset

  //game loop
  bool endGame = false;
  while(endGame == false) {
    //ask player which # to move
    std::cout<<"Please make your move: ";
    int moveNum;
    std::cin >> moveNum;

    game.move(moveNum);
  }
  
}
