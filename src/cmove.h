#ifndef CMOVE_H
#define CMOVE_H

#include "defs.h"
#include <string>
#include <vector>

class CMove {
public:
  CMove(unsigned int, unsigned int, unsigned int=0);

  enum FLAGS {
    CAPTURE = 1<<0,
    KSIDE_CASTLE = 1<<1,
    QSIDE_CASTLE = 1<<2,
    QUEEN_PROMOTION = 1<<3,
    KNIGHT_PROMOTION = 1<<4,
    ROOK_PROMOTION = 1<<5,
    BISHOP_PROMOTION = 1<<6
  };

  int getFlags();

  int getFrom();
  int getTo();

  std::string getNotation();

  static int notationToIndex(std::string);
  static std::string indexToNotation(int);
private:
  int _move;

  const static char RANKS[];
  const static char FILES[];
};

typedef std::vector<CMove> MoveList;

#endif
