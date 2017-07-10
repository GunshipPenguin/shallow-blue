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
inline int _popLsb(U64& board) {
  int lsbIndex = __builtin_ffsll(board) - 1;
  board &= board-1;
  return lsbIndex;
}

/**
 * @brief Returns the number of set bits in the given value.
 *
 * @param  board Value to return number of set bits for
 * @return Number of set bits in value
 */
inline int _popCount(U64 board) {
  return __builtin_popcountll(board);
}

/**
 * @brief Returns the index of the LSB in the given bitboard.
 *
 * @param  board Bitboard to get LSB of
 * @return The index of the LSB in the given bitboard.
 */
inline int _bitscanForward(U64 board) {
  return __builtin_ffsll(board) - 1;
}

/**
 * @brief Returns the index of the MSB in the given bitboard.
 *
 * @param  board Bitboard to get MSB of
 * @return The index of the MSB in the given bitboard.
 */
inline int _bitscanReverse(U64 board) {
  return __builtin_clzll(board) + 1;
}

/**
 * @brief Moves all set bits in the given bitboard one square east and returns the new bitboard, discarding those that fall off the edge.
 * @param  board Board to move bits east on.
 * @return A bitboard with all set bits moved one square east, bits falling off the edge discarded
 */
inline U64 _eastOne(U64 board) {
  return ((board << ONE) & (~FILE_A));
}

/**
 * @brief Moves all set bits in the given bitboard one square west and returns the new bitboard, discarding those that fall off the edge.
 * @param  board Board to move bits west on.
 * @return A bitboard with all set bits moved one square west, bits falling off the edge discarded
 */
inline U64 _westOne(U64 board) {
  return ((board >> ONE) & (~FILE_H));
}

#endif
