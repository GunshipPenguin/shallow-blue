#include "attacks.h"
#include "bitutils.h"
#include "rays.h"
#include <cstring>
#include <stdexcept>

U64 Attacks::detail::_nonSlidingAttacks[2][6][64] = {{{0}}};

U64 Attacks::detail::_rookMasks[64] = {0};
U64 Attacks::detail::_bishopMasks[64] = {0};

U64 Attacks::detail::_rookTable[64][4096] = {{0}};
U64 Attacks::detail::_bishopTable[64][1024] = {{0}};

const U64 Attacks::detail::_rookMagics[64] = {
    0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
    0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
    0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
    0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
    0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
    0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
    0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
    0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
    0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
    0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
    0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
    0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
    0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

const U64 Attacks::detail::_bishopMagics[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
    0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
    0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
    0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
    0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
    0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
    0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
    0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
    0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
    0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
    0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
    0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
};

const int Attacks::detail::_rookIndexBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const int Attacks::detail::_bishopIndexBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

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
    default: throw std::logic_error("Not a sliding piece");
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
