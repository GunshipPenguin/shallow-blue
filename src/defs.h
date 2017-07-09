/**
 * @file
 *
 * Contains global constants and functions used in many classes / functions.
 *
 */

#ifndef DEFS_H
#define DEFS_H

/**
 * An unsigned 64 bit integer (A bitboard).
 */
typedef unsigned long long U64;

/**
 * @name Rank bitboards
 *
 * @{
 */
const U64 RANK_1 = 255ull;
const U64 RANK_2 = 65280ull;
const U64 RANK_3 = 16711680ull;
const U64 RANK_4 = 4278190080ull;
const U64 RANK_5 = 1095216660480ull;
const U64 RANK_6 = 280375465082880ull;
const U64 RANK_7 = 71776119061217280ull;
const U64 RANK_8 = 18374686479671623680ull;
/**@}*/

/**
 * @name File bitboards
 *
 * @{
 */
const U64 FILE_H = 9259542123273814144ull;
const U64 FILE_G = 4629771061636907072ull;
const U64 FILE_F = 2314885530818453536ull;
const U64 FILE_E = 1157442765409226768ull;
const U64 FILE_D = 578721382704613384ull;
const U64 FILE_C = 289360691352306692ull;
const U64 FILE_B = 144680345676153346ull;
const U64 FILE_A = 72340172838076673ull;
/**@}*/

/**
 * @brief An empty bitboard. (ie. the number 0)
 */
const U64 ZERO = U64(0);

/**
 * @brief A bitboard containing only the square a1. (ie. the number 1)
 */
const U64 ONE = U64(1);

enum Color {
  WHITE,
  BLACK
};

enum PieceType {
  PAWN,
  ROOK,
  KNIGHT,
  BISHOP,
  QUEEN,
  KING
};

/**
 * @brief Returns the opposite of the given color
 * @param  color Color to get the opposite of
 * @return WHITE if color == BLACK, BLACK otherwise
 */
inline Color getOppositeColor(Color color) {
  return color == WHITE ? BLACK : WHITE;
}

/**
 * @enum SquareIndex
 *
 * Little endian rank file mapping of each square.
 */
enum SquareIndex {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

#endif
