#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include "cmove.h"
#include <string>

class Board {
  public:
    void setToStartPos();
    void setToFen(std::string);

    std::string getStringRep();

    MoveList getWhitePawnMoves();
    MoveList getBlackPawnMoves();
    MoveList getWhitePawnAttacks();
    MoveList getBlackPawnAttacks();

    MoveList getWhiteKingMoves();
    MoveList getBlackKingMoves();
    MoveList getKingMoves(U64, U64, U64);

  private:
    U64 WHITE_PAWNS = 0;
    U64 BLACK_PAWNS = 0;

    U64 WHITE_ROOKS = 0;
    U64 BLACK_ROOKS = 0;

    U64 WHITE_KNIGHTS = 0;
    U64 BLACK_KNIGHTS = 0;

    U64 WHITE_BISHOPS = 0;
    U64 BLACK_BISHOPS = 0;

    U64 WHITE_QUEENS = 0;
    U64 BLACK_QUEENS = 0;

    U64 WHITE_KING = 0;
    U64 BLACK_KING = 0;

    U64 WHITE_PIECES = 0;
    U64 BLACK_PIECES = 0;

    U64 WHITE_ATTACKABLE = 0;
    U64 BLACK_ATTACKABLE = 0;

    U64 EN_PASSANT = 0;

    U64 OCCUPIED = 0;
    U64 NOT_OCCUPIED = 0;

    MoveList getPawnPromotions(unsigned int, unsigned int, unsigned int=0);

    U64 getOccupied();
    U64 getBlackPieces();
    U64 getWhitePieces();
};

#endif
