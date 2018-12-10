#include "attacks.h"
#include "bitutils.h"
#include <cstring>
#include <stdexcept>

U64 Attacks::detail::NON_SLIDING_ATTACKS[2][6][64] = {{{0}}};
U64 Attacks::detail::RAYS[8][64] = {{0}};

void Attacks::init() {
  detail::_initPawnAttacks();
  detail::_initKnightAttacks();
  detail::_initKingAttacks();

  detail::_initNorthRays();
  detail::_initNorthEastRays();
  detail::_initEastRays();
  detail::_initNorthWestRays();
  detail::_initSouthRays();
  detail::_initSouthWestRays();
  detail::_initWestRays();
  detail::_initSouthEastRays();
}

U64 Attacks::getNonSlidingAttacks(PieceType pieceType, int square, Color color) {
  return detail::NON_SLIDING_ATTACKS[color][pieceType][square];
}

U64 Attacks::getSlidingAttacks(PieceType pieceType, int square, U64 blockers) {
  switch (pieceType) {
    case BISHOP: return detail::_getBishopAttacks(square, blockers);
    case ROOK: return detail::_getRookAttacks(square, blockers);
    case QUEEN:
      return detail::_getBishopAttacks(square, blockers) |
          detail::_getRookAttacks(square, blockers);
    default: throw std::logic_error("Not a sliding piece");
  }
}

U64 Attacks::detail::_getBishopAttacks(int square, U64 blockers) {
  return _getPositiveRayAttack(detail::NORTH_WEST, square, blockers) |
      _getPositiveRayAttack(detail::NORTH_EAST, square, blockers) |
      _getNegativeRayAttack(detail::SOUTH_WEST, square, blockers) |
      _getNegativeRayAttack(detail::SOUTH_EAST, square, blockers);
}

U64 Attacks::detail::_getRookAttacks(int square, U64 blockers) {
  return _getPositiveRayAttack(detail::NORTH, square, blockers) |
      _getPositiveRayAttack(detail::EAST, square, blockers) |
      _getNegativeRayAttack(detail::SOUTH, square, blockers) |
      _getNegativeRayAttack(detail::WEST, square, blockers);
}

U64 Attacks::detail::_getPositiveRayAttack(Attacks::detail::Dir dir,
                                           int square, U64 blockers) {
  U64 attacks = RAYS[dir][square];
  U64 blocked = attacks & blockers;

  if (blocked) {
    int blockSquare = _bitscanForward(blocked);
    attacks ^= RAYS[dir][blockSquare];
  }

  return attacks;
}

U64 Attacks::detail::_getNegativeRayAttack(Attacks::detail::Dir dir,
                                           int square, U64 blockers) {
  U64 attacks = RAYS[dir][square];
  U64 blocked = attacks & blockers;

  if (blocked) {
    int blockSquare = 64 - _bitscanReverse(blocked);
    attacks ^= RAYS[dir][blockSquare];
  }

  return attacks;
}

void Attacks::detail::_initPawnAttacks() {
  for (int i = 0; i < 64; i++) {
    U64 start = ONE << i;

    U64 whiteAttackBb = ((start << 9) & ~FILE_A) | ((start << 7) & ~FILE_H);
    U64 blackAttackBb = ((start >> 9) & ~FILE_H) | ((start >> 7) & ~FILE_A);

    NON_SLIDING_ATTACKS[WHITE][PAWN][i] = whiteAttackBb;
    NON_SLIDING_ATTACKS[BLACK][PAWN][i] = blackAttackBb;
  }
}

void Attacks::detail::_initKnightAttacks() {
  for (int i = 0; i < 64; i++) {
    U64 start = ONE << i;

    U64 attackBb = (((start << 15) | (start >> 17)) & ~FILE_H) | // Left 1
        (((start >> 15) | (start << 17)) & ~FILE_A) | // Right 1
        (((start << 6) | (start >> 10)) & ~(FILE_G | FILE_H)) | // Left 2
        (((start >> 6) | (start << 10)) & ~(FILE_A | FILE_B)); // Right 2

    NON_SLIDING_ATTACKS[WHITE][KNIGHT][i] = attackBb;
    NON_SLIDING_ATTACKS[BLACK][KNIGHT][i] = attackBb;
  }
}

void Attacks::detail::_initKingAttacks() {
  for (int i = 0; i < 64; i++) {
    U64 start = ONE << i;

    U64 attackBb = (((start << 7) | (start >> 9) | (start >> 1)) & (~FILE_H)) |
        (((start << 9) | (start >> 7) | (start << 1)) & (~FILE_A)) |
        ((start >> 8) | (start << 8));

    NON_SLIDING_ATTACKS[WHITE][KING][i] = attackBb;
    NON_SLIDING_ATTACKS[BLACK][KING][i] = attackBb;
  }
}

void Attacks::detail::_initNorthRays() {
  U64 north = U64(0x0101010101010100);
  for (int square = 0; square < 64; square++, north <<= 1) {
    RAYS[NORTH][square] = north;
  }
}

void Attacks::detail::_initEastRays() {
  for (int square = 0; square < 64; square++) {
    RAYS[EAST][square] = 2 * ((ONE << (square | 7)) - (ONE << square));
  }
}

void Attacks::detail::_initNorthEastRays() {
  U64 startRay = U64(0x8040201008040200);
  for (int file = 0; file < 8; file++, startRay = _eastOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8 = 0; rank8 < 64; rank8 += 8, currRay <<= 8ull) {
      RAYS[NORTH_EAST][rank8 + file] = currRay;
    }
  }
}

void Attacks::detail::_initNorthWestRays() {
  U64 startRay = U64(0x102040810204000);
  for (int file = 7; file >= 0; file--, startRay = _westOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8 = 0; rank8 < 64; rank8 += 8, currRay <<= 8ull) {
      RAYS[NORTH_WEST][rank8 + file] = currRay;
    }
  }
}

void Attacks::detail::_initSouthEastRays() {
  U64 startRay = U64(0x2040810204080);
  for (int file = 0; file < 8; file++, startRay = _eastOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8 = 56; rank8 >= 0; rank8 -= 8, currRay >>= 8ull) {
      RAYS[SOUTH_EAST][rank8 + file] = currRay;
    }
  }
}

void Attacks::detail::_initSouthRays() {
  U64 south = U64(0x0080808080808080);
  for (int square = 63; square >= 0; square--, south >>= 1) {
    RAYS[SOUTH][square] = south;
  }
}

void Attacks::detail::_initSouthWestRays() {
  U64 startRay = U64(0x40201008040201);
  for (int file = 7; file >= 0; file--, startRay = _westOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8 = 56; rank8 >= 0; rank8 -= 8, currRay >>= 8ull) {
      RAYS[SOUTH_WEST][rank8 + file] = currRay;
    }
  }
}

void Attacks::detail::_initWestRays() {
  for (int square = 0; square < 64; square++) {
    RAYS[WEST][square] = (ONE << square) - (ONE << (square & 56));
  }
}
