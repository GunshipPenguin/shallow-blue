#ifndef BITUTILS_H
#define BITUTILS_H

inline int _popLsb(U64& val) {
  int lsbIndex = __builtin_ffsll(val) - 1;
  val &= val-1;
  return lsbIndex;
}

inline int _popCount(U64 value) {
  return __builtin_popcountll(value);
}

inline int _bitscanForward(U64 board) {
  return __builtin_ffsll(board) - 1;
}

inline int _bitscanReverse(U64 board) {
  return __builtin_clzll(board) + 1;
}

#endif
