#ifndef RAYTABLE_H
#define RAYTABLE_H

#include "defs.h"

/**
 * @brief Utility class for calculating sliding piece attacks.
 *
 */
class RayTable {
public:
  /**
   * @enum
   * Represents the 8 possible ray directions.
   */
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

  /**
   * @brief Initializes the _rays array
   *
   * This must be called once prior to using RayTable
   */
  static void init();

  /**
   * @brief Returns a bitboard containing a ray in the given direction from the given square on an empty board
   *
   * @param  Dir Direction of ray
   * @param  int Origin square of ray (little endian rank-file mapping)
   * @return A bitboard containing the ray
   */
  static U64 getRay(Dir, int);

  /**
   * @name Methods for getting positive/negative ray attacks
   *
   * Given a direction, origin square, and occupied bitboard, these functions
   * return the ray from the originating square, in the given direction, up to
   * and including the first occupied square (or the edge of the board, if no
   * intersection).
   * @{
   */
  static U64 getPositiveAttacks(Dir, int, U64);
  static U64 getNegativeAttacks(Dir, int, U64);
  /**@}*/

private:
  /**
   * @brief Array indexed by [Dir][Square] of all possible rays on an empty board
   */
  static U64 _rays[8][64];

  /**
   * @name Methods that fill the _rays array
   *
   * These are called from init().
   *
   * @{
   */
  static void _calcNorth();
  static void _calcNorthEast();
  static void _calcEast();
  static void _calcNorthWest();

  static void _calcSouth();
  static void _calcSouthWest();
  static void _calcWest();
  static void _calcSouthEast();
  /**@}*/
};

#endif
