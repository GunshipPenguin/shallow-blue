#ifndef RAYTABLE_H
#define RAYTABLE_H

#include "defs.h"

class RayTable {
public:
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

  static void init();

  static U64 getRay(Dir, int);
  static U64 getPositiveAttacks(Dir, int, U64);
  static U64 getNegativeAttacks(Dir, int, U64);

private:
  static U64 _rays[8][64];

  static void _calcNorth();
  static void _calcNorthEast();
  static void _calcEast();
  static void _calcNorthWest();

  static void _calcSouth();
  static void _calcSouthWest();
  static void _calcWest();
  static void _calcSouthEast();
};

#endif
