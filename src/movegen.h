#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "defs.h"

typedef std::vector<Move> MoveList;

typedef std::pair<Move, Board> MoveBoard;
typedef std::vector<MoveBoard> MoveBoardList;

class MoveGen {
public:
  MoveGen(const Board& board);
  MoveGen();

  void setBoard(const Board& board);

  MoveList getMoves();
  MoveBoardList getLegalMoves();

private:
  MoveList _moves;
  MoveBoardList _legalMoves;

  void _genMoves(const Board& board);
  void _genLegalMoves(const Board& board);

  void _genWhiteMoves(const Board&);
  void _genBlackMoves(const Board&);

  void _genWhitePawnMoves(const Board&);
  void _genWhitePawnSingleMoves(const Board&);
  void _genWhitePawnDoubleMoves(const Board&);
  void _genWhitePawnRightAttacks(const Board&);
  void _genWhitePawnLeftAttacks(const Board&);

  void _genBlackPawnMoves(const Board&);
  void _genBlackPawnSingleMoves(const Board&);
  void _genBlackPawnDoubleMoves(const Board&);
  void _genBlackPawnRightAttacks(const Board&);
  void _genBlackPawnLeftAttacks(const Board&);

  void _genPawnPromotions(unsigned int, unsigned int, unsigned int=0, PieceType=PAWN);

  void _genWhiteKingMoves(const Board&);
  void _genBlackKingMoves(const Board&);

  void _genWhiteKnightMoves(const Board&);
  void _genBlackKnightMoves(const Board&);

  void _genWhiteBishopMoves(const Board&);
  void _genBlackBishopMoves(const Board&);

  void _genWhiteRookMoves(const Board&);
  void _genBlackRookMoves(const Board&);

  void _genWhiteQueenMoves(const Board&);
  void _genBlackQueenMoves(const Board&);

  void _genKingMoves(const Board&, U64, U64, U64);
  void _genKnightMoves(const Board&, U64, U64, U64);
  void _genBishopMoves(const Board&, U64, U64, U64);
  void _genRookMoves(const Board&, U64, U64, U64);
  void _genQueenMoves(const Board&, U64, U64, U64);

  void _addMoves(const Board&, int, PieceType, U64, U64);
};

#endif
