#ifndef RAYTABLE_H
#define RAYTABLE_H

#include "defs.h"

class RayTable {
public:
  RayTable();

  enum Dir {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST
  };

  U64 getRay(Dir, int);
  U64 getPositiveAttacks(Dir, int, U64);
  U64 getNegativeAttacks(Dir, int, U64);

private:
  U64 rays[8][64];

  void calcNorth();
  void calcNorthEast();
  void calcEast();
  void calcNorthWest();

  void calcSouth();
  void calcSouthWest();
  void calcWest();
  void calcSouthEast();

  U64 eastOne(U64);
  U64 westOne(U64);

  int bitscanForward(U64);
  int bitscanReverse(U64);
};

#endif
