#include "attacks.h"
#include "bitutils.h"
#include "rays.h"
#include <cstring>

U64 Attacks::detail::_nonSlidingAttacks[2][6][64] = {{{0}}};

U64 Attacks::detail::_rookMasks[64] = {0};
U64 Attacks::detail::_bishopMasks[64] = {0};

U64 Attacks::detail::_rookTable[64][4096] = {{0}};
U64 Attacks::detail::_bishopTable[64][1024] = {{0}};

void Attacks::init() {
  detail::_initPawnAttacks();
  detail::_initKnightAttacks();
  detail::_initKingAttacks();

  detail::_initRookMasks();
  detail::_initBishopMasks();

  detail::_initRookMagicTable();
  detail::_initBishopMagicTable();
}

U64 Attacks::detail::_getBlockersFromIndex(int index, U64 mask) {
  U64 blockers = ZERO;
  int bits = _popCount(mask);
  for (int i = 0; i < bits; i++) {
    int bitPos = _popLsb(mask);
    if (index & (1 << i)) {
      blockers |= (ONE << bitPos);
    }
  }
  return blockers;
}

void Attacks::detail::_initRookMasks() {
  for (int square = 0; square < 64; square++) {
    _rookMasks[square] = (Rays::getRay(Rays::NORTH, square) & ~RANK_8) |
        (Rays::getRay(Rays::SOUTH, square) & ~RANK_1) |
        (Rays::getRay(Rays::EAST, square) & ~FILE_H) |
        (Rays::getRay(Rays::WEST, square) & ~FILE_A);
  }
}

void Attacks::detail::_initBishopMasks() {
  U64 edgeSquares = FILE_A | FILE_H | RANK_1 | RANK_8;
  for (int square = 0; square < 64; square++) {
    _bishopMasks[square] = (Rays::getRay(Rays::NORTH_EAST, square) | Rays::getRay(Rays::NORTH_WEST, square) |
        Rays::getRay(Rays::SOUTH_WEST, square) | Rays::getRay(Rays::SOUTH_EAST, square)) & ~(edgeSquares);
  }
}

void Attacks::detail::_initRookMagicTable() {
  // For all squares
  for (int square = 0; square < 64; square++) {
    // For all possible blockers for this square
    for (int blockerIndex = 0; blockerIndex < (1 << _rookIndexBits[square]); blockerIndex++) {
      U64 blockers = _getBlockersFromIndex(blockerIndex, _rookMasks[square]);
      _rookTable[square][(blockers * _rookMagics[square]) >> (64 - _rookIndexBits[square])] =
          _getRookAttacksSlow(square, blockers);
    }
  }
}

void Attacks::detail::_initBishopMagicTable() {
  // For all squares
  for (int square = 0; square < 64; square++) {
    // For all possible blockers for this square
    for (int blockerIndex = 0; blockerIndex < (1 << _bishopIndexBits[square]); blockerIndex++) {
      U64 blockers = _getBlockersFromIndex(blockerIndex, _bishopMasks[square]);
      _bishopTable[square][(blockers * _bishopMagics[square]) >> (64 - _bishopIndexBits[square])] =
          _getBishopAttacksSlow(square, blockers);
    }
  }
}

U64 Attacks::detail::_getBishopAttacks(int square, U64 blockers) {
  blockers &= _bishopMasks[square];
  return detail::_bishopTable[square][(blockers * detail::_bishopMagics[square]) >> (64 - detail::_bishopIndexBits[square])];
}

U64 Attacks::detail::_getRookAttacks(int square, U64 blockers) {
  blockers &= detail::_rookMasks[square];
  U64 key = (blockers * detail::_rookMagics[square]) >> (64 - detail::_rookIndexBits[square]);
  return detail::_rookTable[square][key];
}

U64 Attacks::getNonSlidingAttacks(PieceType pieceType, int square, Color color) {
  return detail::_nonSlidingAttacks[color][pieceType][square];
}

U64 Attacks::getSlidingAttacks(PieceType pieceType, int square, U64 blockers) {
  switch (pieceType) {
    case BISHOP: return detail::_getBishopAttacks(square, blockers);
    case ROOK: return detail::_getRookAttacks(square, blockers);
    case QUEEN:
      return detail::_getBishopAttacks(square, blockers) |
          detail::_getRookAttacks(square, blockers);
    default: fatal("Not a sliding piece");
  }
}

U64 Attacks::detail::_getBishopAttacksSlow(int square, U64 blockers) {
  U64 attacks = ZERO;

  // North West
  attacks |= Rays::getRay(Rays::NORTH_WEST, square);
  if (Rays::getRay(Rays::NORTH_WEST, square) & blockers) {
    attacks &= ~(Rays::getRay(Rays::NORTH_WEST, (_bitscanForward(Rays::getRay(Rays::NORTH_WEST, square) & blockers))));
  }

  // North East
  attacks |= Rays::getRay(Rays::NORTH_EAST, square);
  if (Rays::getRay(Rays::NORTH_EAST, square) & blockers) {
    attacks &= ~(Rays::getRay(Rays::NORTH_EAST, _bitscanForward(Rays::getRay(Rays::NORTH_EAST, square) & blockers)));
  }

  // South East
  attacks |= Rays::getRay(Rays::SOUTH_EAST, square);
  if (Rays::getRay(Rays::SOUTH_EAST, square) & blockers) {
    attacks &= ~(Rays::getRay(Rays::SOUTH_EAST, _bitscanReverse(Rays::getRay(Rays::SOUTH_EAST, square) & blockers)));
  }

  // South West
  attacks |= Rays::getRay(Rays::SOUTH_WEST, square);
  if (Rays::getRay(Rays::SOUTH_WEST, square) & blockers) {
    attacks &= ~(Rays::getRay(Rays::SOUTH_WEST, _bitscanReverse(Rays::getRay(Rays::SOUTH_WEST, square) & blockers)));
  }

  return attacks;
}

U64 Attacks::detail::_getRookAttacksSlow(int square, U64 blockers) {
  U64 attacks = ZERO;

  // North
  attacks |= Rays::getRay(Rays::NORTH, square);
  if (Rays::getRay(Rays::NORTH, square) & blockers) {
    attacks &= ~(Rays::getRay(Rays::NORTH, _bitscanForward(Rays::getRay(Rays::NORTH, square) & blockers)));
  }

  // South
  attacks |= Rays::getRay(Rays::SOUTH, square);
  if (Rays::getRay(Rays::SOUTH, square) & blockers) {
    attacks &= ~(Rays::getRay(Rays::SOUTH, _bitscanReverse(Rays::getRay(Rays::SOUTH, square) & blockers)));
  }

  // East
  attacks |= Rays::getRay(Rays::EAST, square);
  if (Rays::getRay(Rays::EAST, square) & blockers) {
    attacks &= ~(Rays::getRay(Rays::EAST, _bitscanForward(Rays::getRay(Rays::EAST, square) & blockers)));
  }

  // West
  attacks |= Rays::getRay(Rays::WEST, square);
  if (Rays::getRay(Rays::WEST, square) & blockers) {
    attacks &= ~(Rays::getRay(Rays::WEST, _bitscanReverse(Rays::getRay(Rays::WEST, square) & blockers)));
  }

  return attacks;
}

void Attacks::detail::_initPawnAttacks() {
  for (int i = 0; i < 64; i++) {
    U64 start = ONE << i;

    U64 whiteAttackBb = ((start << 9) & ~FILE_A) | ((start << 7) & ~FILE_H);
    U64 blackAttackBb = ((start >> 9) & ~FILE_H) | ((start >> 7) & ~FILE_A);

    _nonSlidingAttacks[WHITE][PAWN][i] = whiteAttackBb;
    _nonSlidingAttacks[BLACK][PAWN][i] = blackAttackBb;
  }
}

void Attacks::detail::_initKnightAttacks() {
  for (int i = 0; i < 64; i++) {
    U64 start = ONE << i;

    U64 attackBb = (((start << 15) | (start >> 17)) & ~FILE_H) | // Left 1
        (((start >> 15) | (start << 17)) & ~FILE_A) | // Right 1
        (((start << 6) | (start >> 10)) & ~(FILE_G | FILE_H)) | // Left 2
        (((start >> 6) | (start << 10)) & ~(FILE_A | FILE_B)); // Right 2

    _nonSlidingAttacks[WHITE][KNIGHT][i] = attackBb;
    _nonSlidingAttacks[BLACK][KNIGHT][i] = attackBb;
  }
}

void Attacks::detail::_initKingAttacks() {
  for (int i = 0; i < 64; i++) {
    U64 start = ONE << i;

    U64 attackBb = (((start << 7) | (start >> 9) | (start >> 1)) & (~FILE_H)) |
        (((start << 9) | (start >> 7) | (start << 1)) & (~FILE_A)) |
        ((start >> 8) | (start << 8));

    _nonSlidingAttacks[WHITE][KING][i] = attackBb;
    _nonSlidingAttacks[BLACK][KING][i] = attackBb;
  }
}
