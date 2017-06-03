#include <iostream>
#include <string>
#include "cmove.h"
#include "board.h"

int main() {
  Board board;
  board.setToStartPos();

  std::cout << board.getStringRep() << std::endl;

  // for(MoveList::iterator it = moves.begin(); it != moves.end(); ++it) {
  //   std::cout << (*it).getStringMove() << std::endl;
  // }

  return 0;
}
