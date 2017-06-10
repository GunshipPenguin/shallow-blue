#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include "raytable.h"
#include "cmove.h"
#include <string>

class Board {
  public:
    void setToStartPos();
    void setToFen(std::string);

    void clearBitBoards();

    std::string getStringRep();

    MoveList getWhitePawnMoves();
    MoveList getBlackPawnMoves();
    MoveList getWhitePawnAttacks();
    MoveList getBlackPawnAttacks();

    MoveList getWhiteKingMoves();
    MoveList getBlackKingMoves();
    MoveList getKingMoves(U64, U64, U64);

    MoveList getWhiteKnightMoves();
    MoveList getBlackKnightMoves();
    MoveList getKnightMoves(U64, U64, U64);

    MoveList getWhiteBishopMoves();
    MoveList getBlackBishopMoves();
    MoveList getBishopMoves(U64, U64, U64);

    MoveList getWhiteRookMoves();
    MoveList getBlackRookMoves();
    MoveList getRookMoves(U64, U64, U64);

    MoveList getWhiteQueenMoves();
    MoveList getBlackQueenMoves();

  private:
    U64 WHITE_PAWNS;
    U64 BLACK_PAWNS;

    U64 WHITE_ROOKS;
    U64 BLACK_ROOKS;

    U64 WHITE_KNIGHTS;
    U64 BLACK_KNIGHTS;

    U64 WHITE_BISHOPS;
    U64 BLACK_BISHOPS;

    U64 WHITE_QUEENS;
    U64 BLACK_QUEENS;

    U64 WHITE_KING;
    U64 BLACK_KING;

    U64 WHITE_PIECES;
    U64 BLACK_PIECES;

    U64 WHITE_ATTACKABLE;
    U64 BLACK_ATTACKABLE;

    bool WHITE_TO_MOVE;

    U64 EN_PASSANT;

    U64 OCCUPIED;
    U64 NOT_OCCUPIED;

    RayTable raytable;

    // Methods returning attack bitboards
    U64 getKingMovesForSquare(int);
    U64 getKnightMovesForSquare(int);
    U64 getBishopMovesForSquare(int);
    U64 getRookMovesForSquare(int);
    U64 getQueenMovesForSquare(int);

    void addMoves(MoveList&, int, U64, U64);

    MoveList getPawnPromotions(unsigned int, unsigned int, unsigned int=0);

    U64 getOccupied();
    U64 getBlackPieces();
    U64 getWhitePieces();
};

#endif
