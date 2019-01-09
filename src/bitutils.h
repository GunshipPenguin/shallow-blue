/**
 * @file
 *
 * Contains utility functions for working with bitboards.
 */

#ifndef BITUTILS_H
#define BITUTILS_H

/**
 * @brief Sets the LSB of the given bitboard to 0 and returns its index.
 *
 * @param  board Value to reset LSB of
 * @return Index of reset LSB
 */
inline int _popLsb(U64 &board) {
  int lsbIndex = __builtin_ffsll(board) - 1;
  board &= board - 1;
  return lsbIndex;
}

/**
 * @brief Returns the number of set bits in the given bitboard.
 *
 * @param  board Value to return number of set bits for
 * @return Number of set bits in value
 */
inline int _popCount(U64 board) {
  return __builtin_popcountll(board);
}

/**
 * @brief Returns the index of the LSB in the given bitboard or -1 if
 * the bitboard is empty.
 *
 * @param  board Bitboard to get LSB of
 * @return The index of the LSB in the given bitboard.
 */
inline int _bitscanForward(U64 board) {
  if (board == ZERO) {
    return -1;
  }
  return __builtin_ffsll(board) - 1;
}

/**
 * @brief Returns the index of the MSB in the given bitboard or -1 if
 * the bitboard is empty.
 *
 * @param  board Bitboard to get MSB of
 * @return The index of the MSB in the given bitboard.
 */
inline int _bitscanReverse(U64 board) {
  if (board == ZERO) {
    return -1;
  }
  return 63 - __builtin_clzll(board);
}

/**
* @brief Moves all set bits in the given bitboard n squares east and returns
* the new bitboard, discarding those that fall off the edge.
*
* @param board Board to move bits east on
* @param n Number of squares to move east
* @return A bitboard with all set bits moved one square east, bits falling off the edge discarded
*/
inline U64 _eastN(U64 board, int n) {
  U64 newBoard = board;
  for (int i = 0; i < n; i++) {
    newBoard = ((newBoard << 1) & (~FILE_A));
  }

  return newBoard;
}

/**
 * @brief Moves all set bits in the given bitboard n squares west and returns the new
 * bitboard, discarding those that fall off the edge.
 *
 * @param board Board to move bits west on
 * @param n Number of squares to move west
 * @return A bitboard with all set bits moved one square west, bits falling off the edge discarded
 */
inline U64 _westN(U64 board, int n) {
  U64 newBoard = board;
  for (int i = 0; i < n; i++) {
    newBoard = ((newBoard >> 1) & (~FILE_H));
  }

  return newBoard;
}

/**
 * @brief Returns the zero indexed row of the given square
 *
 * @param square A square in little endian rank file mapping form
 * @return The zero indexed row of the square
 */
inline int _row(int square) {
  return square / 8;
}

/**
 * @brief Returns the zero indexed column of the given square.
 *
 * @param square A square in little endian rank file mapping form
 * @return The zero indexed row of the squares
 */
inline int _col(int square) {
  return square % 8;
}

#endif
