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

    std::string getStringRep();

    void setToStartPos();
    void setToFen(std::string);

    void doMove(CMove);

    bool whiteCanCastleKs();
    bool whiteCanCastleQs();

    bool blackCanCastleKs();
    bool blackCanCastleQs();

    bool whiteIsInCheck();
    bool blackIsInCheck();

    U64 getWhitePieces(PieceType);
    U64 getBlackPieces(PieceType);

    U64 getAllWhitePieces();
    U64 getAllBlackPieces();

    U64 getWhiteAttackable();
    U64 getBlackAttackable();

    U64 getWhiteAttacks();
    U64 getBlackAttacks();

    U64 getOccupied();
    U64 getNotOccupied();

    U64 getEnPassant();

    bool whiteToMove();

    // Methods returning attack bitboards
    U64 getWhitePawnAttacksForSquare(int);
    U64 getBlackPawnAttacksForSquare(int);
    U64 getKingAttacksForSquare(int, U64);
    U64 getKnightAttacksForSquare(int, U64);
    U64 getBishopAttacksForSquare(int, U64);
    U64 getRookAttacksForSquare(int, U64);
    U64 getQueenAttacksForSquare(int, U64);

  private:
    U64 _whitePieces[6];
    U64 _blackPieces[6];

    U64 _allWhitePieces;
    U64 _allBlackPieces;

    U64 _whiteAttackable;
    U64 _blackAttackable;

    U64 _whiteAttacks;
    U64 _blackAttacks;

    U64 _occupied;
    U64 _notOccupied;

    U64 _enPassant;

    bool _whiteToMove;

    static RayTable _raytable;

    bool _whiteCanCastleKs;
    bool _whiteCanCastleQs;

    bool _blackCanCastleKs;
    bool _blackCanCastleQs;

    void _updateNonPieceBitBoards();

    void _doRegularMove(CMove);

    void _clearBitBoards();

    U64 _genWhiteAttacks();
    U64 _genBlackAttacks();

    PieceType _getWhitePieceAtSquare(int);
    PieceType _getBlackPieceAtSquare(int);
};

#endif
