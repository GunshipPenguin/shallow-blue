#ifndef RAYTABLE_H
#define RAYTABLE_H

#include "defs.h"

class RayTable {
public:
  RayTable();

private:
  U64 northRays[64];
  U64 northEastRays[64];
  U64 eastRays[64];
  U64 northWestRays[64];

  U64 southRays[64];
  U64 southWestRays[64];
  U64 westRays[64];
  U64 southEastRays[64];

  U64 bishopAttacks[64];
  U64 rookAttacks[64];
  U64 queenAttacks[64];

  U64 eastOne(U64);
  U64 westOne(U64);

  void calcNorth();
  void calcNorthEast();
  void calcEast();
  void calcNorthWest();

  void calcSouth();
  void calcSouthWest();
  void calcWest();
  void calcSouthEast();

  void calcSliding();
};

#endif
