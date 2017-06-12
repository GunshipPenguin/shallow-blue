#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include "raytable.h"
#include "cmove.h"
#include <string>

class Board {
  public:
    Board();

    void setToStartPos();
    void setToFen(std::string);

    void clearBitBoards();

    void doMove(CMove);

    std::string getStringRep();

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

    bool WHITE_CAN_CASTLE_KS;
    bool WHITE_CAN_CASTLE_QS;

    bool BLACK_CAN_CASTLE_KS;
    bool BLACK_CAN_CASTLE_QS;

    U64 EN_PASSANT;

    U64 OCCUPIED;
    U64 NOT_OCCUPIED;

    // Methods returning attack bitboards
    U64 getWhitePawnAttacksForSquare(int);
    U64 getBlackPawnAttacksForSquare(int);
    U64 getKingAttacksForSquare(int, U64);
    U64 getKnightAttacksForSquare(int, U64);
    U64 getBishopAttacksForSquare(int, U64);
    U64 getRookAttacksForSquare(int, U64);
    U64 getQueenAttacksForSquare(int, U64);

    U64 getWhiteAttacks();
    U64 getBlackAttacks();

  private:
    RayTable raytable;

    void updateBitBoards();

    U64 getOccupied();
    U64 getBlackPieces();
    U64 getWhitePieces();

    U64* getWhiteBitBoard(int);
    U64* getBlackBitBoard(int);
};

#endif
