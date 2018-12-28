/**
 * @file
 *
 * Contains global constants and functions used in many classes / functions.
 *
 */

#ifndef DEFS_H
#define DEFS_H

#include <limits>
#include <iostream>
#include <cstdlib>

/**
 * An unsigned 64 bit integer (A bitboard).
 */
typedef unsigned long long U64;

/**
 * @name Rank bitboards
 *
 * @{
 */
const U64 RANK_1 = 0xffull;
const U64 RANK_2 = 0xff00ull;
const U64 RANK_3 = 0xff0000ull;
const U64 RANK_4 = 0xff000000ull;
const U64 RANK_5 = 0xff00000000ull;
const U64 RANK_6 = 0xff0000000000ull;
const U64 RANK_7 = 0xff000000000000ull;
const U64 RANK_8 = 0xff00000000000000ull;
/**@}*/

/**
 * @name File bitboards
 *
 * @{
 */
const U64 FILE_H = 0x8080808080808080ull;
const U64 FILE_G = 0x4040404040404040ull;
const U64 FILE_F = 0x2020202020202020ull;
const U64 FILE_E = 0x1010101010101010ull;
const U64 FILE_D = 0x808080808080808ull;
const U64 FILE_C = 0x404040404040404ull;
const U64 FILE_B = 0x202020202020202ull;
const U64 FILE_A = 0x101010101010101ull;
/**@}*/

/**
 * @name Black and white squares
 */
const U64 BLACK_SQUARES = 0xAA55AA55AA55AA55;
const U64 WHITE_SQUARES = 0x55AA55AA55AA55AA;
/**@}*/

/**
 * @brief An empty bitboard. (ie. the number 0)
 */
const U64 ZERO = U64(0);

/**
 * @brief A bitboard containing only the square a1. (ie. the number 1)
 */
const U64 ONE = U64(1);

/** @brief Positive infinity to be used during search (eg. as a return value for winning) */
const int INF = std::numeric_limits<int>::max();

/**
 * @enum Color
 * @brief Represents a color.
 */
enum Color {
  WHITE,
  BLACK
};

/**
 * @enum PieceType
 * @brief Represents a piece type.
 */
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
 *
 * @param  color Color to get the opposite of
 * @return WHITE if color == BLACK, BLACK otherwise
 */
inline Color getOppositeColor(Color color) {
  return color == WHITE ? BLACK : WHITE;
}

/**
 * @brief Print the given message to stderr and exit with code 1. Should be used in
 * unrecoverable situations.
 *
 * @param msg Message to print to stderr before exiting
 */
[[ noreturn ]]
inline void fatal(std::string msg) {
  std::cerr << msg << std::endl;
  std::exit(1);
}

/**
 * @enum SquareIndex
 * @brief Little endian rank file mapping of each square.
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

/**
 * @enum GamePhase
 * @brief Enum representing the game phase (opening/endgame)
 */
enum GamePhase {
  OPENING,
  ENDGAME
};
#endif
