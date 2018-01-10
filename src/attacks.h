#ifndef ATTACKS_H
#define ATTACKS_H

#include "defs.h"

/**
 * @brief Namespace containing attack bitboard generation utilities
 */
namespace Attacks {
  namespace detail {
    /**
     * @enum Dir
     * @brief Represents the 4 cardinal and intercardinal directions
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
     * @name Non sliding attack initialization functions
     * @brief These functions precalculate pawn, knight and king moves for 
     * each square
     * 
     * @{
     */
    void _initPawnAttacks();
    void _initKnightAttacks();
    void _initKingAttacks();
    /**@}*/

    /**
     * @brief Returns a bitboard containing all possible bishop moves
     * 
     * @param square Square to calculate bishop moves from
     * @param blockers A bitboard containing any blocking pieces
     * @return A bitboard containing all possible bishop moves 
     */
    U64 _getBishopAttacks(int, U64);

    /**
     * @brief Returns a bitboard containing all possible rook moves
     * 
     * @param square Square to calculate rook moves from
     * @param blockers A bitboard containing any blocking pieces
     * @return A bitboard containing all possible rook moves 
     */
    U64 _getRookAttacks(int, U64);

    /**
     * @brief Gets a ray in the specified positive direction from the specified
     * square
     * 
     * Rays are returned up to and including any blockers.
     * 
     * @param dir Direction to get positive ray attack in
     * @param square Square to get positive ray attack from 
     * @param blockers Bitboard containing blocking pieces 
     * @return A bitboard containing the ray up to and including the first
     * blocking piece
     */
    U64 _getPositiveRayAttack(Dir, int, U64);

    /**
     * @brief Gets a ray in the specified negative direction from the specified
     * square
     * 
     * Rays are returned up to and including any blockers.
     * 
     * @param dir Direction to get negative ray attacks in
     * @param square Square to get negative ray attacks from 
     * @param blockers Bitboard containing blocking pieces 
     * @return A bitboard containing the ray up to and including the first
     * blocking piece
     */
    U64 _getNegativeRayAttack(Dir, int, U64);
    /**@}*/

    /**
     * @name Ray precalculation functions
     * @brief These functions precalculate rays for a each direction
     * that are used for generation of sliding attacks
     * @{
     */
    void _initNorthRays();
    void _initNorthEastRays();
    void _initEastRays();
    void _initNorthWestRays();
    void _initSouthRays();
    void _initSouthWestRays();
    void _initWestRays();
    void _initSouthEastRays();
    /**@}*/

    /**
     * @brief 3D array indexed by [color][pieceType][square] of non sliding
     * piece attacks
     */
    extern U64 NON_SLIDING_ATTACKS[2][6][64];

    /**
     * @brief 2D array indexed by [direction][square] of all possible rays for
     * all squares
     */
    extern U64 RAYS[8][64];
  }

  /**
   * @brief Initializes all internal values used to generate attacks
   */
  void init();

  /**
   * @brief Gets a bitboard containing all possible squares that the given
   * non sliding piece can move to
   * 
   * @param pieceType Type of non sliding piece to calculate attacks for 
   * @param square Square to get moves from
   * @param color Color of piece, only applicable for pawns, which are unique
   * as they move in different directions
   * @return U64 A bitboard containing all possible square that the non
   * sliding piece can move to
   */
  U64 getNonSlidingAttacks(PieceType, int, Color=WHITE);

  /**
   * @brief Gets a bitboard containg all possible squares that the given
   * piece type can move to from the given square, given a bitboard
   * containing pieces that block the piece from moving
   * 
   * The returned bitboard will contain all possible attacks, up to and
   * including any blocking pieces.
   * 
   * @param pieceType Type of sliding piece to calculate attacks for 
   * @param square Square to get moves from
   * @param blockers A bitboard containing pieces that block the movement
   * of the piece 
   * @return A bitboard containing all possible squares that the sliding
   * piece can move to
   */
  U64 getSlidingAttacks(PieceType, int, U64);
};

#endif