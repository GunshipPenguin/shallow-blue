#include "cmove.h"
#include "defs.h"
#include <string>

const char CMove::RANKS[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
const char CMove::FILES[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

CMove::CMove(unsigned int from, unsigned int to, unsigned int flags) {
  // Moves are stored as packed integers with the first 8 bits representing the
  // from square, the middle 8 bits representing the to square and the last 8 bits
  // containing flags

  // Squares are numbered 0-63 with a1 being 0 and h8 being 63

  // 0x3f = 63 = 8 set bits
  _move = ((from & 0x3f) << 16) | ((to & 0x3f) << 8) | (flags & 0x3f);
}

int CMove::getFrom() {
  return ((_move >> 16) & 0x3f);
}

int CMove::getTo() {
  return ((_move >> 8) & 0x3f);
}

int CMove::getFlags() {
  return _move & 0x3f;
}

std::string CMove::getStringMove() {
  if (getFlags() & KSIDE_CASTLE) {
    return "O-O";
  } else if (getFlags() & QSIDE_CASTLE) {
    return "O-O-O";
  }

  int fromIndex = getFrom();
  int toIndex = getTo();

  std::string moveNotation;

  if (getFlags() & CAPTURE) {
    moveNotation = indexToNotation(fromIndex) + 'x' + indexToNotation(toIndex);
  } else {
    moveNotation = indexToNotation(fromIndex) + indexToNotation(toIndex);
  }

  if (getFlags() & QUEEN_PROMOTION) {
    moveNotation += 'Q';
  } else if (getFlags() & KNIGHT_PROMOTION) {
    moveNotation += 'N';
  } else if (getFlags() & BISHOP_PROMOTION) {
    moveNotation += 'B';
  } else if (getFlags() & ROOK_PROMOTION) {
    moveNotation += 'R';
  }

  return moveNotation;
}

std::string CMove::indexToNotation (int index) {
  return std::string({FILES[index%8], RANKS[index/8]});
}
