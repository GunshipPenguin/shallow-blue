#include "board.h"
#include <iostream>

int main() {
  Board board;
  board.setToStartPos();

  std::cout << board.getPawnAttacks();
}
