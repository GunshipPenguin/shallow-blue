#include <iostream>
#include "raytable.h"
#include "defs.h"
#include "board.h"

int main() {
  Board board;
  board.setToFen("8/8/8/3Q4/8/8/8/8 w - -");

  MoveList moves = board.getWhiteQueenMoves();

  for(MoveList::iterator it = moves.begin(); it != moves.end(); ++it) {
    std::cout << (*it).getNotation() << std::endl;
  }

  return 0;
}
