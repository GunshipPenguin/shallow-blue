#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include "raytable.h"
#include "psquaretable.h"
#include "zkey.h"
#include "move.h"
#include <string>

class Move;

class Board {
  public:
    Board();

    std::string getStringRep() const ;

    void setToStartPos();
    void setToFen(std::string);

    void doMove(Move);

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

    PieceType getPieceAtSquare(Color, int) const;

    U64 getOccupied() const;
    U64 getNotOccupied() const;

    U64 getEnPassant() const;

    ZKey getZKey() const;

    PSquareTable getPSquareTable() const;

    Color getActivePlayer() const;
    Color getInactivePlayer() const;

    U64 getAttacksForSquare(PieceType, Color, int) const;

  private:
    U64 _getWhitePawnAttacksForSquare(int) const;
    U64 _getBlackPawnAttacksForSquare(int) const;
    U64 _getKingAttacksForSquare(int, U64) const;
    U64 _getKnightAttacksForSquare(int, U64) const;
    U64 _getBishopAttacksForSquare(int, U64) const;
    U64 _getRookAttacksForSquare(int, U64) const;
    U64 _getQueenAttacksForSquare(int, U64) const;

    U64 _pieces[2][6];

    U64 _allPieces[2];

    U64 _attackable[2];

    // Calculating checks is expensive, so calculate on demand and cache the result
    mutable bool _checks[2];
    mutable bool _checksCalculated[2];

    void _resetChecks();

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

    bool _squareUnderAttack(Color, int) const;

    void _updateCastlingRightsForMove(Move);

    void _updateNonPieceBitBoards();

    void _movePiece(Color, PieceType, int, int);
    void _addPiece(Color, PieceType, int);
    void _removePiece(Color, PieceType, int);

    void _clearBitBoards();
};

#endif
