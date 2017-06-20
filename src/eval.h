#ifndef EVAL_H
#define EVAL_H

#include "board.h"
#include "defs.h"

class Eval {
public:
  Eval(const Board&, Color);
  int getScore();

private:
  // Piece values in centipawns
  static const int PAWN_VALUE = 100;
  static const int KNIGHT_VALUE = 300;
  static const int BISHOP_VALUE = 300;
  static const int ROOK_VALUE = 500;
  static const int QUEEN_VALUE = 900;

  // Other weights
  static const int MOBILITY_WEIGHT = 15;

  int _score;
  void _doEval(const Board&, Color);
  int _popCount(U64);
};

#endif
