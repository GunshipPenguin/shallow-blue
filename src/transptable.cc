#include "transptable.h"
#include <iostream>

void TranspTable::set(const ZKey &key, TranspTableEntry entry) {
  auto insertResult = _table.insert(std::make_pair(key.getValue(), entry));

  // If insertion was prevented due to the key already existing, overwrite the value
  if (!insertResult.second) {
    insertResult.first->second = entry;
  }
}

void TranspTable::clear() {
  _table.clear();
}

const TranspTableEntry *TranspTable::getEntry(const ZKey &key) const {
  auto got = _table.find(key.getValue());
  if (got != _table.end()) {
    return &_table.at(key.getValue());
  } else {
    return nullptr;
  }
}
