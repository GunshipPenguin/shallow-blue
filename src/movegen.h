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

  void printMoves();

private:
  Board _board;
  MoveList _moves;

  void genMoves();
  void genWhiteMoves();
  void genBlackMoves();

  void genWhitePawnMoves();
  void genBlackPawnMoves();
  void genPawnPromotions(unsigned int, unsigned int, unsigned int=0);

  void genWhiteKingMoves();
  void genBlackKingMoves();

  void genWhiteKnightMoves();
  void genBlackKnightMoves();

  void genWhiteBishopMoves();
  void genBlackBishopMoves();

  void genWhiteRookMoves();
  void genBlackRookMoves();

  void genWhiteQueenMoves();
  void genBlackQueenMoves();

  void genKingMoves(U64, U64, U64);
  void genKnightMoves(U64, U64, U64);
  void genBishopMoves(U64, U64, U64);
  void genRookMoves(U64, U64, U64);
  void genQueenMoves(U64, U64, U64);

  void addMoves(int, PieceType, U64, U64);
};

#endif
