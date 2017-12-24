#include "attacktable.h"
#include "defs.h"
#include "move.h"
#include "bitutils.h"
#include <iostream>
#include <cstring>
#include "raytable.h"

U64 AttackTable::ATTACKS[2][6][64];

void AttackTable::init() {
  _zeroTables();
  _calcPawn();
  _calcKnight();
  _calcBishop();    
  _calcRook();
  _calcQueen();
  _calcKing();
}

void AttackTable::_zeroTables() {
  std::memset(ATTACKS, ZERO, sizeof(ATTACKS));
}

U64 AttackTable::getAttacks(PieceType pieceType, int square, Color color) {
  return ATTACKS[color][pieceType][square];
}

void AttackTable::_calcPawn() {
  for(int i=0;i<64;i++) {
    U64 start = ONE << i;

    U64 whiteAttackBb = ((start << 9) & ~FILE_A) | ((start << 7) & ~FILE_H);
    U64 blackAttackBb = ((start >> 9) & ~FILE_H) | ((start >> 7) & ~FILE_A);

    ATTACKS[WHITE][PAWN][i] = whiteAttackBb;
    ATTACKS[BLACK][PAWN][i] = blackAttackBb;
  }
}

void AttackTable::_calcKnight() {
  for (int i=0;i<64;i++) {
    U64 start = ONE << i;

    U64 attackBb = (((start << 15) | (start >> 17)) & ~FILE_H) | // Left 1
    (((start >> 15) | (start << 17)) & ~FILE_A) | // Right 1
    (((start << 6) | (start >> 10)) & ~(FILE_G | FILE_H)) | // Left 2
    (((start >> 6) | (start << 10)) & ~(FILE_A | FILE_B)); // Right 2

    ATTACKS[WHITE][KNIGHT][i] = attackBb;
    ATTACKS[BLACK][KNIGHT][i] = attackBb;
  }
}

void AttackTable::_calcBishop() {
  for(int i=0;i<64;i++) {
    U64 attackBb = RayTable::getRay(RayTable::NORTH_EAST, i) | RayTable::getRay(RayTable::NORTH_WEST, i)
        | RayTable::getRay(RayTable::SOUTH_EAST, i) | RayTable::getRay(RayTable::SOUTH_WEST, i);

    ATTACKS[WHITE][BISHOP][i] = attackBb;
    ATTACKS[BLACK][BISHOP][i] = attackBb;
  }
}

void AttackTable::_calcRook() {
  for(int i=0;i<64;i++) {
    U64 attackBb = RayTable::getRay(RayTable::NORTH, i) | RayTable::getRay(RayTable::SOUTH, i)
        | RayTable::getRay(RayTable::EAST, i) | RayTable::getRay(RayTable::WEST, i);

    ATTACKS[WHITE][ROOK][i] = attackBb;
    ATTACKS[BLACK][ROOK][i] = attackBb;
  }
}

void AttackTable::_calcQueen() {
  for(int i=0;i<64;i++) {
    U64 attackBb = RayTable::getRay(RayTable::NORTH_EAST, i) | RayTable::getRay(RayTable::NORTH_WEST, i)
        | RayTable::getRay(RayTable::SOUTH_EAST, i) | RayTable::getRay(RayTable::SOUTH_WEST, i)
        | RayTable::getRay(RayTable::NORTH, i) | RayTable::getRay(RayTable::SOUTH, i)
        | RayTable::getRay(RayTable::EAST, i) | RayTable::getRay(RayTable::WEST, i);

    ATTACKS[WHITE][QUEEN][i] = attackBb;
    ATTACKS[BLACK][QUEEN][i] = attackBb;
  }
}

void AttackTable::_calcKing() {
  for(int i=0;i<64;i++) {
    U64 start = ONE << i;
    
    U64 attackBb = (((start << 7) | (start >> 9) | (start >> 1)) & (~FILE_H)) |
    (((start << 9) | (start >> 7) | (start << 1)) & (~FILE_A)) |
    ((start >> 8) | (start << 8));

    ATTACKS[WHITE][KING][i] = attackBb;
    ATTACKS[BLACK][KING][i] = attackBb;
  }
}