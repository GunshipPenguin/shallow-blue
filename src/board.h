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

    U64 getPieces(Color, PieceType);

    U64 getAllPieces(Color);

    U64 getAttackable(Color);

    U64 getAttacks(Color);

    U64 getOccupied();
    U64 getNotOccupied();

    U64 getEnPassant();

    Color getActivePlayer();
    Color getInactivePlayer();

    // Methods returning attack bitboards
    U64 getWhitePawnAttacksForSquare(int);
    U64 getBlackPawnAttacksForSquare(int);
    U64 getKingAttacksForSquare(int, U64);
    U64 getKnightAttacksForSquare(int, U64);
    U64 getBishopAttacksForSquare(int, U64);
    U64 getRookAttacksForSquare(int, U64);
    U64 getQueenAttacksForSquare(int, U64);

  private:
    U64 _pieces[2][6];

    U64 _allPieces[2];

    U64 _attackable[2];

    U64 _attacks[2];

    U64 _occupied;
    U64 _notOccupied;

    U64 _enPassant;

    Color _activePlayer;

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

    PieceType _getPieceAtSquare(Color, int);
};

#endif
