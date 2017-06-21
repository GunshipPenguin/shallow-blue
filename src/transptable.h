#ifndef TRANSPTABLE_H
#define TRANSPTABLE_H

#include "board.h"
#include "zkey.h"
#include <unordered_map>

class TranspTable {
public:
  enum Flag {
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
  };

  void set(ZKey, int, int, Flag);

  bool contains(ZKey);

  int getScore(ZKey);
  int getDepth(ZKey);
  Flag getFlag(ZKey);

  void clear();

private:
  std::unordered_map<U64, std::tuple<int, int, Flag> > _table;
};

#endif
