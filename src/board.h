#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include "raytable.h"
#include "cmove.h"
#include <string>

class CMove;

class Board {
  public:
    Board();

    void setToStartPos();
    void setToFen(std::string);

    void clearBitBoards();

    bool whiteCanCastleKs();
    bool whiteCanCastleQs();

    bool blackCanCastleKs();
    bool blackCanCastleQs();

    bool whiteIsInCheck();
    bool blackIsInCheck();

    U64 getWhiteBitBoard(PieceType);
    U64 getBlackBitBoard(PieceType);

    void doMove(CMove);

    std::string getStringRep();

    U64 WHITE_PIECES;
    U64 BLACK_PIECES;

    U64 WHITE_ATTACKABLE;
    U64 BLACK_ATTACKABLE;

    U64 WHITE_ATTACKS;
    U64 BLACK_ATTACKS;

    bool WHITE_TO_MOVE;

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
    U64 _whitePieces[6];
    U64 _blackPieces[6];

    static RayTable raytable;

    // From the fen string, override the above variables
    bool WHITE_CAN_CASTLE_KS;
    bool WHITE_CAN_CASTLE_QS;

    bool BLACK_CAN_CASTLE_KS;
    bool BLACK_CAN_CASTLE_QS;

    void updateNonPieceBitBoards();

    void doRegularMove(CMove);

    U64 getOccupied();
    U64 getBlackPieces();
    U64 getWhitePieces();

    U64* getWhiteBitBoard(int);
    U64* getBlackBitBoard(int);
};

#endif
