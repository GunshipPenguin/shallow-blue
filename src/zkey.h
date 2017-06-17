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

  void setWhitePiece(PieceType, unsigned int);
  void setBlackPiece(PieceType, unsigned int);

  void setEnPassant(unsigned int);

  void setWhiteCastleKs();
  void setWhiteCastleQs();
  void setBlackCastleKs();
  void setBlackCastleQs();

private:
  U64 _key;

  static U64 WHITE_PIECE_KEYS[6][64];
  static U64 BLACK_PIECE_KEYS[6][64];

  static U64 EN_PASSANT_KEYS[64];

  static U64 WHITE_KS_CASTLE_KEY;
  static U64 WHITE_QS_CASTLE_KEY;
  static U64 BLACK_KS_CASTLE_KEY;
  static U64 BLACK_QS_CASTLE_KEY;

  static const unsigned int PRNG_KEY;
};

#endif
