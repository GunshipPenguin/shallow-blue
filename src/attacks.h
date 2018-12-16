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
U64 _getBishopAttacksSlow(int square, U64 blockers);

/**
 * @brief Returns a bitboard containing all possible rook moves
 *
 * @param square Square to calculate rook moves from
 * @param blockers A bitboard containing any blocking pieces
 * @return A bitboard containing all possible rook moves
 */
U64 _getRookAttacksSlow(int square, U64 blockers);

void _initBishopMagicTable();
void _initRookMagicTable();

U64 _getBishopAttacks(int, U64);
U64 _getRookAttacks(int, U64);

U64 _getBishopMask(int);
U64 _getRookMask(int);

U64 _getBlockersFromIndex(int, U64);

/**
 * @brief 3D array indexed by [color][pieceType][square] of non sliding
 * piece attacks
 */
extern U64 _nonSlidingAttacks[2][6][64];

extern U64 _rookTable[64][4096];
extern U64 _bishopTable[64][1024];

const extern U64 _rookMagics[64];
const extern U64 _bishopMagics[64];

const extern int _rookIndexBits[64];
const extern int _bishopIndexBits[64];
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
U64 getNonSlidingAttacks(PieceType, int, Color= WHITE);

/**
 * @brief Gets a bitboard containing all possible squares that the given
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