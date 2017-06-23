#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "defs.h"
#include "movegen.h"
#include "transptable.h"
#include <limits>

class Search {
public:
  Search(const Board&, int depth, int maxTime, bool=true);
  CMove getBestMove();

private:
  static const int INF = std::numeric_limits<int>::max();

  void _rootMax(const Board&, int, MoveBoardList);
  int _negaMax(const Board&, int, int, int, const MoveBoardList&);
  void _iterDeep(const Board&, int, int);

  void _orderMoves(MoveBoardList&, const MoveBoardList&, unsigned int);

  MoveBoardList _getPv(const Board&);

  bool _logUci;
  TranspTable _tt;
  CMove _bestMove;
  int _bestScore;
};

#endif
