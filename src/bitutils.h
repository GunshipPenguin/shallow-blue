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
 * @brief Returns the number of set bits in the given value.
 *
 * @param  board Value to return number of set bits for
 * @return Number of set bits in value
 */
inline int _popCount(U64 board) {
  return __builtin_popcountll(board);
}

/**
 * @brief Returns the index of the LSB in the given bitboard. Returns -1 if the bitboard is empty.
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
 * @brief Returns the index of the MSB in the given bitboard.
 *
 * @param  board Bitboard to get MSB of
 * @return The index of the MSB in the given bitboard.
 */
inline int _bitscanReverse(U64 board) {
  return 63 - __builtin_clzll(board);
}

/**
 * @brief Moves all set bits in the given bitboard n squares east and returns the new bitboard, discarding those that fall off the edge.
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
 * @brief Moves all set bits in the given bitboard n squares west and returns the new bitboard, discarding those that fall off the edge.
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

inline int _row(int square) {
  return square / 8;
}

inline int _col(int square) {
  return square % 8;
}

inline U64 _northRay(int square) {
 return 0x0101010101010100ULL << square;
}

inline U64 _southRay(int square) {
  return 0x0080808080808080ULL >> (63 - square);
}

inline U64 _eastRay(int square) {
  return 2 * ((ONE << (square | 7)) - (ONE << square));
}

inline U64 _westRay(int square) {
  return (ONE << square) - (ONE << (square & 56));
}

inline U64 _northWestRay(int square) {
  return _westN(0x102040810204000ULL, 7 - _col(square)) << (_row(square) * 8);
}

inline U64 _northEastRay(int square) {
  return _eastN(0x8040201008040200ULL, _col(square)) << (_row(square) * 8);
}

inline U64 _southWestRay(int square) {
  return _westN(0x40201008040201ULL, 7 - _col(square)) >> ((7 - _row(square)) * 8);
}

inline U64 _southEastRay(int square) {
  return _eastN(0x2040810204080ULL, _col(square)) >> ((7 - _row(square)) * 8);
}

#endif
