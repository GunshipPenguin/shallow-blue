#ifndef ZKEY_H
#define ZKEY_H

#include "defs.h"

class Board;

class ZKey {
public:
  ZKey();
  ZKey(Board);

  static void init();

  U64 getValue();

  void movePiece(Color, PieceType, unsigned int, unsigned int);
  void flipPiece(Color, PieceType, unsigned int);

  void flipActivePlayer();

  void clearEnPassant();
  void setEnPassantFile(unsigned int);

  void updateCastlingRights(bool, bool, bool, bool);

  void flipToMove();

private:
  U64 _key;

  void _flipKsCastle(Color);
  void _flipQsCastle(Color);

  int _enPassantFile;
  bool _whiteKs;
  bool _whiteQs;
  bool _blackKs;
  bool _blackQs;

  static U64 PIECE_KEYS[2][6][64];

  // One en passant key for each file
  static U64 EN_PASSANT_KEYS[8];

  static U64 KS_CASTLE_KEYS[2];
  static U64 QS_CASTLE_KEYS[2];

  static U64 WHITE_TO_MOVE_KEY;

  static const unsigned int PRNG_KEY;
};

#endif
