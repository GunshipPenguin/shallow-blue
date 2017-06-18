#ifndef TRANSPTABLE_H
#define TRANSPTABLE_H

#include "board.h"
#include "zkey.h"
#include <unordered_map>

class TranspTable {
public:
  void add(ZKey, int);
  bool contains(ZKey);
  int get(ZKey);

  void clear();

private:
  std::unordered_map<U64, int> _table;
};

#endif
