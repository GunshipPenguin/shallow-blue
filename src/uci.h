#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "cmove.h"
#include <sstream>
#include <fstream>

class Uci {
public:
  void start();

private:
  Board _board;

  void uciNewGame();
  void setPosition(std::istringstream&);
  CMove pickBestMove();
};

#endif
