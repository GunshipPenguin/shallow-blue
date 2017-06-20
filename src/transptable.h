#ifndef TRANSPTABLE_H
#define TRANSPTABLE_H

#include "board.h"
#include "zkey.h"
#include <unordered_map>

class TranspTable {
public:
  void set(ZKey, int, int);

  bool contains(ZKey);

  int getScore(ZKey);
  int getDepth(ZKey);

  void clear();

private:
  std::unordered_map<U64, std::pair<int, int> > _table;
};

#endif
