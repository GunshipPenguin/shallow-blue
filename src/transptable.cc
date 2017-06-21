#include "transptable.h"
#include <iostream>

void TranspTable::set(ZKey key, int score, int depth, Flag flag) {
  _table[key.getValue()] = std::tuple<int, int, TranspTable::Flag>(score, depth, flag);
}

void TranspTable::clear() {
  _table.clear();
}

bool TranspTable::contains(ZKey key) {
//  std::cout << key.getValue() << std::endl;

  std::unordered_map<U64, std::tuple<int, int, TranspTable::Flag> >::const_iterator got = _table.find(key.getValue());

//  std::cout << "adsf" << std::endl;

  return got != _table.end();
}

int TranspTable::getScore(ZKey key) {
  std::tuple<int, int, Flag> entry = _table.at(key.getValue());
  return std::get<0>(entry);
}

int TranspTable::getDepth(ZKey key) {
  std::tuple<int, int, Flag> entry = _table.at(key.getValue());
  return std::get<1>(entry);
}

TranspTable::Flag TranspTable::getFlag(ZKey key) {
  std::tuple<int, int, Flag> entry = _table.at(key.getValue());
  return std::get<2>(entry);
}
