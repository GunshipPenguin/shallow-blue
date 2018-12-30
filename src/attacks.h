#ifndef ATTACKS_H
#define ATTACKS_H

#include "defs.h"

/**
 * @brief Namespace containing attack bitboard generation utilities
 */
namespace Attacks {
namespace detail {
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
 * @brief Returns a bitboard containing all possible rook moves
 *
 * @param square Square to calculate rook moves from
 * @param blockers A bitboard containing any blocking pieces
 * @return A bitboard containing all possible rook moves
 */
U64 _getRookAttacksSlow(int square, U64 blockers);

/**
 * @brief Returns a bitboard containing all possible bishop moves
 *
 * @param square Square to calculate bishop moves from
 * @param blockers A bitboard containing any blocking pieces
 * @return A bitboard containing all possible bishop moves
 */
U64 _getBishopAttacksSlow(int square, U64 blockers);

/**
 * @name Rook/bishop magic table precalculation functions
 *
 * Initializes the internal rook/bishop magic tables used for
 * fast calculation of sliding attacks
 *
 * @{
 */
void _initRookMagicTable();
void _initBishopMagicTable();
/**@}*/

/**
 * @name Rook/bishop mask precalculation functions
 *
 * Initializes the internal tables of rook/bishop masks used for
 * sliding attack calculations
 *
 * @{
 */
void _initRookMasks();
void _initBishopMasks();
/**@}*/

/**
 * @brief Gets rook/bishop attacks on the given square with the given blocker
 * pieces
 *
 * Internally this uses the fancy magic bitboard technique to lookup attacks
 * from a preinitialized attack table
 *
 * @{
 */
U64 _getRookAttacks(int, U64);
U64 _getBishopAttacks(int, U64);
/**@}*/

/**
 * @brief Given a blockers bitboard and an index value containing
 * no more set bits than exist in the blockers bitboard, return a new bitboard
 * with blocked squares bits set or unset in the same order as in the passed
 * index
 *
 * This is used to exhaustively generate blocker sets to create magic tables.
 *
 * @return A bitboard with the blocked bits set/unset in the same order as
 * the index
 */
U64 _getBlockersFromIndex(int, U64);

/**
 * @brief 3D array indexed by [color][pieceType][square] of non sliding
 * piece attacks
 */
extern U64 _nonSlidingAttacks[2][6][64];

/**
 * @name Rook and bishop magic bitboard tables
 *
 * @{
 */
extern U64 _rookTable[64][4096];
extern U64 _bishopTable[64][1024];
/**@}*/

/**
 * @name Rook and bishop sliding attack masks indexed by [square]
 *
 * These do not include edge squares
 *
 * @{
 */
extern U64 _rookMasks[64];
extern U64 _bishopMasks[64];
/**@}*/

/**
 * @name Rook and bishop magic values for magic table lookups
 * @{
 */
const extern U64 _rookMagics[64];
const extern U64 _bishopMagics[64];
/*@}*/

/**
 * @name Number of bits in the magic bitboard table index for rooks
 * and bishops
 *
 * @{
 */
const extern int _rookIndexBits[64];
const extern int _bishopIndexBits[64];
/**@}*/
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