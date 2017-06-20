#include "transptable.h"
#include <iostream>

void TranspTable::set(ZKey key, int score, int depth) {
  _table[key.getValue()] = std::pair<int, int>(score, depth);
}

void TranspTable::clear() {
  _table.clear();
}

bool TranspTable::contains(ZKey key) {
  return _table.find(key.getValue()) != _table.end();
}

int TranspTable::getScore(ZKey key) {
  return _table[key.getValue()].first;
}

int TranspTable::getDepth(ZKey key) {
  return _table[key.getValue()].second;
}
