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

  void _uciNewGame();
  void _setPosition(std::istringstream&);
  void _go(std::istringstream&);
  void _pickBestMove(int);
};

#endif
