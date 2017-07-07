#include "raytable.h"
#include "defs.h"
#include "bitutils.h"
#include <iostream>

RayTable::RayTable() {
  _calcNorth();
  _calcNorthEast();
  _calcEast();
  _calcNorthWest();

  _calcSouth();
  _calcSouthWest();
  _calcWest();
  _calcSouthEast();
}

U64 RayTable::getPositiveAttacks(Dir dir, int square, U64 occupied) {
  U64 attacks = rays[dir][square];
  U64 blocked = attacks & occupied;

  if (blocked) {
    int blockSquare = _bitscanForward(blocked);
    attacks ^= rays[dir][blockSquare];
  }

  return attacks;
}

U64 RayTable::getNegativeAttacks(Dir dir, int square, U64 occupied) {
  U64 attacks = rays[dir][square];
  U64 blocked = attacks & occupied;

  if (blocked) {
    int blockSquare = 64 - _bitscanReverse(blocked);
    attacks ^= rays[dir][blockSquare];
  }

  return attacks;
}

U64 RayTable::getRay(Dir direction, int square) {
  return rays[direction][square];
}

void RayTable::_calcNorth() {
  U64 north = U64(0x0101010101010100);
  for (int square=0;square<64;square++,north<<=1) {
    rays[NORTH][square] = north;
  }
}

void RayTable::_calcEast() {
  for(int square=0;square<64;square++) {
    rays[EAST][square] = 2*((ONE << (square | 7)) - (ONE << square));
  }
}

void RayTable::_calcNorthEast() {
  U64 startRay = U64(0x8040201008040200);
  for (int file=0;file<8;file++,startRay=_eastOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8=0;rank8<64;rank8+=8,currRay<<=8ull) {
      rays[NORTH_EAST][rank8+file] = currRay;
    }
  }
}

void RayTable::_calcNorthWest() {
  U64 startRay = U64(0x102040810204000);
  for (int file=7;file>=0;file--,startRay=_westOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8=0;rank8<64;rank8+=8,currRay<<=8ull) {
      rays[NORTH_WEST][rank8+file] = currRay;
    }
  }
}

void RayTable::_calcSouthEast() {
  U64 startRay = U64(0x2040810204080);
  for (int file=0;file<8;file++,startRay=_eastOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8=56;rank8>=0;rank8-=8,currRay>>=8ull) {
      rays[SOUTH_EAST][rank8+file] = currRay;
    }
  }
}

void RayTable::_calcSouth() {
  U64 south = U64(0x0080808080808080);
  for (int square=63;square>=0;square--,south>>=1) {
    rays[SOUTH][square] = south;
  }
}

void RayTable::_calcSouthWest() {
  U64 startRay = U64(0x40201008040201);
  for (int file=7;file>=0;file--,startRay=_westOne(startRay)) {
     U64 currRay = startRay;
     for (int rank8=56;rank8>=0;rank8-=8,currRay>>=8ull) {
       rays[SOUTH_WEST][rank8+file] = currRay;
     }
  }
}

void RayTable::_calcWest() {
  for(int square=0;square<64;square++) {
    rays[WEST][square] = (ONE << square) - (ONE << (square&56));
  }
}

U64 RayTable::_eastOne(U64 bb) {
  return ((bb << 1ull) & (~FILE_A));
}

U64 RayTable::_westOne(U64 bb) {
  return ((bb >> 1ull) & (~FILE_H));
}
