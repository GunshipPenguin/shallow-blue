#include "cmove.h"
#include "defs.h"
#include <string>
#include <iostream>
#include <algorithm>

const char CMove::RANKS[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
const char CMove::FILES[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

CMove::CMove() {
  _move = ((NULL_MOVE & 0x7f) << 21);
}

CMove::CMove(unsigned int from, unsigned int to, PieceType piece, unsigned int flags) {
  _move = ((flags & 0x7f) << 21) | ((to & 0x3f) << 15) | ((from & 0x3f) << 9) | (piece & 0x7);
}

PieceType CMove::getPieceType() {
  return static_cast<PieceType>(_move & 0x7);
}

PieceType CMove::getCapturedPieceType() {
  return static_cast<PieceType>((_move >> 6) & 0x7);
}

void CMove::setCapturedPieceType(PieceType pieceType) {
  unsigned int mask = 0x7 << 6;
  _move = (_move & ~mask) | ((pieceType << 6) & mask);
}

PieceType CMove::getPromotionPieceType() {
  return static_cast<PieceType>((_move >> 3) & 0x7);
}

void CMove::setPromotionPieceType(PieceType pieceType) {
  unsigned int mask = 0x7 << 3;
  _move = (_move & ~mask) | ((pieceType << 3) & mask);
}

unsigned int CMove::getFrom() {
  return ((_move >> 9) & 0x3f);
}

unsigned int CMove::getTo() {
  return ((_move >> 15) & 0x3f);
}

unsigned int CMove::getFlags() {
  return ((_move >> 21) & 0x7f);
}

void CMove::setFlag(Flag flag) {
  _move = _move | (flag << 21);
}

std::string CMove::getNotation() {
  int fromIndex = getFrom();
  int toIndex = getTo();

  std::string moveNotation = indexToNotation(fromIndex) + indexToNotation(toIndex);

  if (getFlags() & PROMOTION) {
    switch (getPromotionPieceType()) {
      case QUEEN: moveNotation += 'q';
        break;
      case ROOK: moveNotation += 'r';
        break;
      case KNIGHT: moveNotation += 'n';
        break;
      case BISHOP: moveNotation += 'b';
        break;
      default:
        break;
    }
  }

  return moveNotation;
}

bool CMove::operator==(const CMove other) const {
  return other._move == _move;
}

std::string CMove::indexToNotation (int index) {
  return std::string({FILES[index%8], RANKS[index/8]});
}

unsigned int CMove::notationToIndex (std::string notation) {
  int file = std::find(FILES, FILES+8, notation[0]) - std::begin(FILES);
  int rank = std::find(RANKS, RANKS+8, notation[1]) - std::begin(RANKS);

  return rank * 8 + file;
}
