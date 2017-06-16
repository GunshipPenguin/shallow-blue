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

  void _calcNorth();
  void _calcNorthEast();
  void _calcEast();
  void _calcNorthWest();

  void _calcSouth();
  void _calcSouthWest();
  void _calcWest();
  void _calcSouthEast();

  U64 _eastOne(U64);
  U64 _westOne(U64);

  int _bitscanForward(U64);
  int _bitscanReverse(U64);
};

#endif
