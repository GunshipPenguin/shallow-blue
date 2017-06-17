#include "cmove.h"
#include "defs.h"
#include <string>
#include <iostream>
#include <algorithm>

const char CMove::RANKS[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
const char CMove::FILES[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

CMove::CMove(unsigned int from, unsigned int to, PieceType piece, unsigned int flags) {
  _move = ((flags & 0x1ff) << 15) | ((to & 0x3f) << 9) | ((from & 0x3f) << 3) | (piece & 0x7);
}

PieceType CMove::getPieceType() {
  return static_cast<PieceType>(_move & 0x7);
}

unsigned int CMove::getFrom() {
  return ((_move >> 3) & 0x3f);
}

unsigned int CMove::getTo() {
  return ((_move >> 9) & 0x3f);
}

unsigned int CMove::getFlags() {
  return ((_move >> 15) & 0x1ff);
}

std::string CMove::getNotation() {
  int fromIndex = getFrom();
  int toIndex = getTo();

  std::string moveNotation = indexToNotation(fromIndex) + indexToNotation(toIndex);

  if (getFlags() & QUEEN_PROMOTION) {
    moveNotation += 'q';
  } else if (getFlags() & KNIGHT_PROMOTION) {
    moveNotation += 'n';
  } else if (getFlags() & BISHOP_PROMOTION) {
    moveNotation += 'b';
  } else if (getFlags() & ROOK_PROMOTION) {
    moveNotation += 'r';
  }

  return moveNotation;
}

std::string CMove::indexToNotation (int index) {
  return std::string({FILES[index%8], RANKS[index/8]});
}

unsigned int CMove::notationToIndex (std::string notation) {
  int file = std::find(FILES, FILES+8, notation[0]) - std::begin(FILES);
  int rank = std::find(RANKS, RANKS+8, notation[1]) - std::begin(RANKS);

  return rank * 8 + file;
}
