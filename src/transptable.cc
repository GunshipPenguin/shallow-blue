#include "transptable.h"

void TranspTable::add(ZKey key, int value) {
  _table[key.getValue()] = value;
}

void TranspTable::clear() {
  _table.clear();
}

bool TranspTable::contains(ZKey key) {
  return _table.find(key.getValue()) != _table.end();
}

int TranspTable::get(ZKey key) {
  return _table[key.getValue()];
}
