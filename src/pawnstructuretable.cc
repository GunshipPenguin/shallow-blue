#include "pawnstructuretable.h"
#include "defs.h"
#include "board.h"
#include <unordered_map>

std::unordered_map<U64, PawnStructureTable::PawnStructureEntry> _table;

PawnStructureTable::PawnStructureEntry *PawnStructureTable::get(ZKey key) {
  return &_table.at(key.getValue());
}

bool PawnStructureTable::exists(ZKey key) {
  return _table.find(key.getValue()) != _table.end();
}

void PawnStructureTable::set(ZKey key, PawnStructureTable::PawnStructureEntry entry) {
  _table.insert(std::make_pair(key.getValue(), entry));
}
