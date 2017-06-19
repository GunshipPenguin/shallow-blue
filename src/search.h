#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "defs.h"
#include "cmove.h"
#include "transptable.h"
#include <limits>

class Search {
public:
  Search(const Board&);
  CMove getBestMove();

private:
  static const int INF = std::numeric_limits<int>::max();
  CMove rootMax(const Board&, int);

  int negaMax(const Board&, Color, int, int, int);

  TranspTable _tt;
  CMove _bestMove;
};

#endif
