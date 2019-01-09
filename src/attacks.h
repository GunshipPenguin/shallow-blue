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
 * @name Rook/Bishop attack precalculation functions
 * @brief These functions calculate rook and bishop attacks for the given
 * square and the given set of blockers
 *
 * Note that these functions should only be used for precalculating attack
 * bitboards to put in magic tables. The much faster _getRookAttacks and
 * _getBishopAttacks should be used in all other scenarios.
 *
 * @{
 */
U64 _getRookAttacksSlow(int, U64);
U64 _getBishopAttacksSlow(int, U64);
/**@}*/

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
 * @name Rook/Bishop attack bitboard generation functions
 * @brief Gets rook/bishop attacks on the given square with the given blocker
 * pieces
 *
 * Internally these functions use the fancy magic bitboard technique to lookup
 * attacks from a preinitialized attack table
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
 *
 * @{
 */
const U64 _rookMagics[64] = {
    0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
    0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
    0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
    0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
    0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
    0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
    0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
    0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
    0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
    0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
    0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
    0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
    0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

const U64 _bishopMagics[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
    0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
    0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
    0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
    0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
    0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
    0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
    0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
    0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
    0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
    0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
    0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
};
/**@}*/

/**
 * @name Number of bits in the magic bitboard table index for rooks
 * and bishops
 * @brief Indexed by [square] with each value being the number of
 * bits required for that square
 *
 * @{
 */
const int _rookIndexBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const int _bishopIndexBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};
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
U64 getNonSlidingAttacks(PieceType, int, Color=WHITE);

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