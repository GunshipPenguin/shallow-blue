#ifndef CMOVE_H
#define CMOVE_H

#include "defs.h"
#include "board.h"
#include <string>
#include <vector>

class CMove {
public:
  CMove();
  CMove(unsigned int, unsigned int, PieceType, unsigned int=0);

  enum FLAGS {
    CAPTURE = 1<<0,
    DOUBLE_PAWN_PUSH = 1<<1,
    KSIDE_CASTLE = 1<<2,
    QSIDE_CASTLE = 1<<3,
    EN_PASSANT = 1<<4,
    QUEEN_PROMOTION = 1<<5,
    KNIGHT_PROMOTION = 1<<6,
    ROOK_PROMOTION = 1<<7,
    BISHOP_PROMOTION = 1<<8,
    NULL_MOVE = 1<<9
  };

  unsigned int getFlags();

  PieceType getPieceType();
  unsigned int getCapturedPieceType();

  unsigned int getFrom();
  unsigned int getTo();

  std::string getNotation();

  static unsigned int notationToIndex(std::string);
  static std::string indexToNotation(int);
private:
  // Format
  // |----4-----|---3---|---2---|-1--|
  // 4 - Flags (9 bits)
  // 3 - To square (6 bits)
  // 2 - From square (6 bits)
  // 1 - Piece type (3 bits)
  unsigned int _move;

  const static char RANKS[];
  const static char FILES[];
};

#endif
