#include "eval.h"
#include "defs.h"
#include "movegen.h"
#include "bitutils.h"

Eval::Eval(const Board& board, Color color) {
  _doEval(board, color);
}

int Eval::getScore() {
  return _score;
}

int Eval::getMaterialValue(PieceType pieceType) {
  int value = 0;
  switch (pieceType) {
    case PAWN: value = PAWN_VALUE;
      break;
    case KNIGHT: value = KNIGHT_VALUE;
      break;
    case BISHOP: value = BISHOP_VALUE;
      break;
    case ROOK: value = ROOK_VALUE;
      break;
    case QUEEN: value = QUEEN_VALUE;
      break;
    default: value = 0;
      break;
  }
  return value;
}

void Eval::_doEval(const Board& board, Color color) {
  _score = 0;

  _score += PAWN_VALUE * _popCount(board.getPieces(color, PAWN));
  _score += KNIGHT_VALUE * _popCount(board.getPieces(color, KNIGHT));
  _score += BISHOP_VALUE *_popCount(board.getPieces(color, BISHOP));
  _score += ROOK_VALUE * _popCount(board.getPieces(color, ROOK));
  _score += QUEEN_VALUE * _popCount(board.getPieces(color, QUEEN));

  Color otherColor = getOppositeColor(color);

  _score -= PAWN_VALUE * _popCount(board.getPieces(otherColor, PAWN));
  _score -= KNIGHT_VALUE * _popCount(board.getPieces(otherColor, KNIGHT));
  _score -= BISHOP_VALUE *_popCount(board.getPieces(otherColor, BISHOP));
  _score -= ROOK_VALUE * _popCount(board.getPieces(otherColor, ROOK));
  _score -= QUEEN_VALUE * _popCount(board.getPieces(otherColor, QUEEN));

  PSquareTable pst = board.getPSquareTable();
  _score += (pst.getScore(color) - pst.getScore(otherColor));

  _score += MOBILITY_VALUE * (_calcMobility(board, color) - _calcMobility(board, otherColor));
}

int Eval::_calcMobility(const Board& board, Color color) {
  int moves = 0;

  // Special case for pawn moves
  U64 pawns = board.getPieces(color, PAWN);
  U64 movedPawns1 = (color == WHITE ? pawns << 8 : pawns >> 8) & board.getNotOccupied();
  U64 movedPawns2 = (color == WHITE ? (movedPawns1 & RANK_3) << 8 : (movedPawns1 & RANK_6) >> 8) & board.getNotOccupied();
  moves += _popCount(movedPawns1 | movedPawns2);

  // Special case for pawn attacks
  while (pawns) {
    int square = _popLsb(pawns);

    U64 attacks = board.getAttacksForSquare(PAWN, color, square);
    attacks &= board.getAllPieces(getOppositeColor(color));

    moves += _popCount(attacks);
  }

  for (auto pieceType : {ROOK, KNIGHT, BISHOP, QUEEN, KING}) {
    U64 pieces = board.getPieces(color, pieceType);

    while (pieces) {
      int square = _popLsb(pieces);

      U64 attackBitBoard = board.getAttacksForSquare(pieceType, color, square);

      moves += _popCount(attackBitBoard);
    }
  }

  return moves;
}
