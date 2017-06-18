#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "defs.h"

class MoveGen {
public:
  MoveGen();
  MoveGen(Board);

  void setBoard(Board);

  MoveList getMoves();
  MoveList getLegalMoves();

  void printMoves();

private:
  Board _board;
  MoveList _moves;
  MoveList _legalMoves;

  void _genMoves();
  void _genLegalMoves();

  void _genWhiteMoves();
  void _genBlackMoves();

  void _genWhitePawnMoves();
  void _genBlackPawnMoves();
  void _genPawnPromotions(unsigned int, unsigned int, unsigned int=0);

  void _genWhiteKingMoves();
  void _genBlackKingMoves();

  void _genWhiteKnightMoves();
  void _genBlackKnightMoves();

  void _genWhiteBishopMoves();
  void _genBlackBishopMoves();

  void _genWhiteRookMoves();
  void _genBlackRookMoves();

  void _genWhiteQueenMoves();
  void _genBlackQueenMoves();

  void _genKingMoves(U64, U64, U64);
  void _genKnightMoves(U64, U64, U64);
  void _genBishopMoves(U64, U64, U64);
  void _genRookMoves(U64, U64, U64);
  void _genQueenMoves(U64, U64, U64);

  void _addMoves(int, PieceType, U64, U64);
};

#endif
