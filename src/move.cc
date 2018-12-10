#include "move.h"
#include <iostream>
#include <algorithm>

const char Move::RANKS[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
const char Move::FILES[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const std::string Move::NULL_MOVE_NOTATION = "(none)";

Move::Move() {
  _move = ((NULL_MOVE & 0x7f) << 21);
  _value = 0;
}

Move::Move(unsigned int from, unsigned int to, PieceType piece, unsigned int flags) {
  _move = ((flags & 0x7f) << 21) | ((to & 0x3f) << 15) | ((from & 0x3f) << 9) | (piece & 0x7);
  _value = 0;
}

PieceType Move::getPieceType() const {
  return static_cast<PieceType>(_move & 0x7);
}

PieceType Move::getCapturedPieceType() const {
  return static_cast<PieceType>((_move >> 6) & 0x7);
}

void Move::setCapturedPieceType(PieceType pieceType) {
  unsigned int mask = 0x7 << 6;
  _move = (_move & ~mask) | ((pieceType << 6) & mask);
}

PieceType Move::getPromotionPieceType() const {
  return static_cast<PieceType>((_move >> 3) & 0x7);
}

void Move::setPromotionPieceType(PieceType pieceType) {
  unsigned int mask = 0x7 << 3;
  _move = (_move & ~mask) | ((pieceType << 3) & mask);
}

int Move::getValue() {
  return _value;
}

void Move::setValue(int value) {
  _value = value;
}

unsigned int Move::getFrom() const {
  return ((_move >> 9) & 0x3f);
}

unsigned int Move::getTo() const {
  return ((_move >> 15) & 0x3f);
}

unsigned int Move::getFlags() const {
  return ((_move >> 21) & 0x7f);
}

void Move::setFlag(Flag flag) {
  _move = _move | (flag << 21);
}

std::string Move::getNotation() const {
  // Special case for null moves
  if (getFlags() & NULL_MOVE) {
    return NULL_MOVE_NOTATION;
  }

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
      default:break;
    }
  }

  return moveNotation;
}

bool Move::operator==(const Move other) const {
  return other._move == _move;
}

std::string Move::indexToNotation(int index) {
  return std::string({FILES[index % 8], RANKS[index / 8]});
}

unsigned int Move::notationToIndex(std::string notation) {
  int file = std::find(FILES, FILES + 8, notation[0]) - std::begin(FILES);
  int rank = std::find(RANKS, RANKS + 8, notation[1]) - std::begin(RANKS);

  return rank * 8 + file;
}
