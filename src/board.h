#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include "raytable.h"
#include "psquaretable.h"
#include "zkey.h"
#include "cmove.h"
#include <string>

class CMove;

class Board {
  public:
    Board();

    std::string getStringRep() const ;

    void setToStartPos();
    void setToFen(std::string);

    void doMove(CMove);

    bool whiteCanCastleKs() const;
    bool whiteCanCastleQs() const;

    bool blackCanCastleKs() const;
    bool blackCanCastleQs() const;

    bool whiteKsCastlingRight() const;
    bool whiteQsCastlingRight() const;

    bool blackKsCastlingRight() const;
    bool blackQsCastlingRight() const;

    bool colorIsInCheck(Color) const;

    U64 getPieces(Color, PieceType) const;

    U64 getAllPieces(Color) const;

    U64 getAttackable(Color) const;

    U64 getAttacks(Color) const;

    U64 getOccupied() const;
    U64 getNotOccupied() const;

    U64 getEnPassant() const;

    ZKey getZKey() const;

    PSquareTable getPSquareTable() const;

    Color getActivePlayer() const;
    Color getInactivePlayer() const;

    // Methods returning attack bitboards
    U64 getWhitePawnAttacksForSquare(int) const;
    U64 getBlackPawnAttacksForSquare(int) const;
    U64 getKingAttacksForSquare(int, U64) const;
    U64 getKnightAttacksForSquare(int, U64) const;
    U64 getBishopAttacksForSquare(int, U64) const;
    U64 getRookAttacksForSquare(int, U64) const;
    U64 getQueenAttacksForSquare(int, U64) const;

  private:
    U64 _pieces[2][6];

    U64 _allPieces[2];

    U64 _attackable[2];

    U64 _attacks[2];

    U64 _occupied;
    U64 _notOccupied;

    U64 _enPassant;

    Color _activePlayer;

    ZKey _zKey;
    PSquareTable _pst;

    static RayTable _raytable;

    bool _whiteCanCastleKs;
    bool _whiteCanCastleQs;

    bool _blackCanCastleKs;
    bool _blackCanCastleQs;

    void _updateNonPieceBitBoards();

    void _doRegularMove(CMove);

    void _clearBitBoards();

    U64 _genWhiteAttacks() const;
    U64 _genBlackAttacks() const;

    PieceType _getPieceAtSquare(Color, int) const;
};

#endif
