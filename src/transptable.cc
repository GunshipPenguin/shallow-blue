#include "transptable.h"
#include <iostream>

void TranspTable::set(const ZKey& key, int score, int depth, Flag flag) {
  _table[key.getValue()] = std::tuple<int, int, TranspTable::Flag>(score, depth, flag);
}

void TranspTable::clear() {
  _table.clear();
}

bool TranspTable::contains(const ZKey& key) const {
  std::unordered_map<U64, std::tuple<int, int, TranspTable::Flag> >::const_iterator got = _table.find(key.getValue());
  return got != _table.end();
}

int TranspTable::getScore(const ZKey& key) const {
  std::tuple<int, int, Flag> entry = _table.at(key.getValue());
  return std::get<0>(entry);
}

int TranspTable::getDepth(const ZKey& key) const {
  std::tuple<int, int, Flag> entry = _table.at(key.getValue());
  return std::get<1>(entry);
}

TranspTable::Flag TranspTable::getFlag(const ZKey& key) const {
  std::tuple<int, int, Flag> entry = _table.at(key.getValue());
  return std::get<2>(entry);
}
