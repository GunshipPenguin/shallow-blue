#include "eval.h"
#include "defs.h"
#include "movegen.h"
#include <iostream>

Eval::Eval(const Board& board, Color color) {
  _doEval(board, color);
}

int Eval::getScore() {
  return _score;
}

void Eval::_doEval(const Board& board, Color color) {
  _score = 0;

  _score += PAWN_VALUE * _popCount(board.getPieces(color, PAWN));
  _score += KNIGHT_VALUE * _popCount(board.getPieces(color, KNIGHT));
  _score += BISHOP_VALUE *_popCount(board.getPieces(color, BISHOP));
  _score += ROOK_VALUE * _popCount(board.getPieces(color, ROOK));
  _score += QUEEN_VALUE * _popCount(board.getPieces(color, QUEEN));
  _score += MOBILITY_WEIGHT * _popCount(board.getAttacks(color));

  Color otherColor = color == WHITE ? BLACK : WHITE;

  _score -= PAWN_VALUE * _popCount(board.getPieces(otherColor, PAWN));
  _score -= KNIGHT_VALUE * _popCount(board.getPieces(otherColor, KNIGHT));
  _score -= BISHOP_VALUE *_popCount(board.getPieces(otherColor, BISHOP));
  _score -= ROOK_VALUE * _popCount(board.getPieces(otherColor, ROOK));
  _score -= QUEEN_VALUE * _popCount(board.getPieces(otherColor, QUEEN));
  _score -= MOBILITY_WEIGHT * _popCount(board.getAttacks(otherColor));
}

int Eval::_popCount(U64 value) {
  return __builtin_popcountll(value);
}
