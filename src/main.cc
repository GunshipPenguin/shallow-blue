#include <iostream>
#include <string>
#include "cmove.h"
#include "board.h"

int main() {
  Board board;

  board.setToFen("8/3p4/1pP5/8/8/8/8/8 w - b7");

  MoveList moves = board.getWhitePawnAttacks();
  for(MoveList::iterator it = moves.begin(); it != moves.end(); ++it) {
    std::cout << (*it).getStringMove() << std::endl;
  }

  return 0;
}
