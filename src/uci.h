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
  static const int DEFAULT_DEPTH = 4;
  static const int DEFAULT_MAX_TIME = 5000;

  void uciNewGame();
  void setPosition(std::istringstream&);
  CMove pickBestMove();
};

#endif
