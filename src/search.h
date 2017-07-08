#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "defs.h"
#include "movegen.h"
#include "transptable.h"
#include <limits>

class Search {
public:
  Search(const Board&, bool=true);
  void perform(int);

  Move getBestMove();
  int getBestScore();

private:
  static const int INF = std::numeric_limits<int>::max();

  void _rootMax(const Board&, int);
  int _negaMax(const Board&, int, int, int);

  int _qSearch(const Board&, int=-INF, int=INF);

  void _orderMoves(MoveBoardList&);
  void _orderMovesQSearch(MoveBoardList&);

  bool _compareMovesTt(MoveBoard, MoveBoard);
  bool _compareMovesMvvLva(MoveBoard, MoveBoard);
  bool _compareMovesPromotionValue(MoveBoard, MoveBoard);

  void _logUciInfo(const MoveBoardList&, int, Move, int);

  int _getPieceValue(PieceType);

  MoveBoardList _getPv(const Board&, int);

  Board _board;

  bool _logUci;
  TranspTable _tt;
  Move _bestMove;
  int _bestScore;
};

#endif
