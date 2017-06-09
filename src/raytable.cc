#include "raytable.h"
#include "defs.h"

RayTable::RayTable() {
  calcNorth();
  calcNorthEast();
  calcEast();
  calcNorthWest();

  calcSouth();
  calcSouthWest();
  calcWest();
  calcSouthEast();
}

U64 RayTable::getRay(Dir direction, int square) {
  return rays[direction][square];
}

void RayTable::calcNorth() {
  U64 north = U64(0x0101010101010100);
  for (int square=0;square<64;square++,north<<=1) {
    rays[NORTH][square] = north;
  }
}

void RayTable::calcEast() {
  const U64 one = U64(1);
  for(int square=0;square<64;square++) {
    rays[EAST][square] = 2*((one << (square | 7)) - (one << square));
  }
}

void RayTable::calcNorthEast() {
  U64 startRay = U64(0x8040201008040200);
  for (int file=0;file<8;file++,startRay=eastOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8=0;rank8<64;rank8+=8,currRay<<=8ull) {
      rays[NORTH_EAST][rank8+file] = currRay;
    }
  }
}

void RayTable::calcNorthWest() {
  U64 startRay = U64(0x102040810204000);
  for (int file=7;file>=0;file--,startRay=westOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8=0;rank8<64;rank8+=8,currRay<<=8ull) {
      rays[NORTH_WEST][rank8+file] = currRay;
    }
  }
}

void RayTable::calcSouthEast() {
  U64 startRay = U64(0x2040810204080);
  for (int file=0;file<8;file++,startRay=eastOne(startRay)) {
    U64 currRay = startRay;
    for (int rank8=56;rank8>=0;rank8-=8,currRay>>=8ull) {
      rays[SOUTH_EAST][rank8+file] = currRay;
    }
  }
}

void RayTable::calcSouth() {
  U64 south = U64(0x0080808080808080);
  for (int square=63;square>=0;square--,south>>=1) {
    rays[SOUTH][square] = south;
  }
}

void RayTable::calcSouthWest() {
  U64 startRay = U64(0x40201008040201);
  for (int file=7;file>=0;file--,startRay=westOne(startRay)) {
     U64 currRay = startRay;
     for (int rank8=56;rank8>=0;rank8-=8,currRay>>=8ull) {
       rays[SOUTH_WEST][rank8+file] = currRay;
     }
  }
}

void RayTable::calcWest() {
  const U64 one = 1;
  for(int square=0;square<64;square++) {
    rays[WEST][square] = (one << square) - (one << (square&56));
  }
}

U64 RayTable::eastOne(U64 bb) {
  return ((bb << 1ull) & (~FILE_A));
}

U64 RayTable::westOne(U64 bb) {
  return ((bb >> 1ull) & (~FILE_H));
}
