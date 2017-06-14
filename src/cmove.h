#ifndef CMOVE_H
#define CMOVE_H

#include "defs.h"
#include <string>
#include <vector>

class CMove {
public:
  CMove(unsigned int, unsigned int, unsigned int=0);
  CMove(std::string);

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
  };

  int getFlags();

  unsigned int getFrom();
  unsigned int getTo();

  std::string getNotation();

  static unsigned int notationToIndex(std::string);
  static std::string indexToNotation(int);
private:
  int _move;

  const static char RANKS[];
  const static char FILES[];
};

typedef std::vector<CMove> MoveList;

#endif
