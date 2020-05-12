#include "transptable.h"
#include <sys/mman.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

constexpr size_t TT_SLOTS = 1 << 22;
constexpr size_t TT_SIZE = sizeof(TranspTableEntry) * TT_SLOTS;
constexpr size_t TT_ALIGNMENT = 1024 * 1024 * 2;

TranspTable::TranspTable() {
  void **mem = reinterpret_cast<void**>(&_table);
  if (posix_memalign(mem, TT_ALIGNMENT, TT_SIZE) != 0) {
    perror("posix_memalign");
    fatal("posix_memalign failed");
  }

  madvise(_table, TT_SIZE, MADV_HUGEPAGE);
  clear();
}

TranspTable::~TranspTable() {
  free(_table);
}

void TranspTable::set(const ZKey &key, TranspTableEntry entry) {
  auto *slot = (TranspTableEntry *) (_table + (key.getValue() % TT_SLOTS));
  *slot = entry;
}

void TranspTable::clear() {
  memset(_table, 0, TT_SIZE);
}

const TranspTableEntry *TranspTable::probe(const ZKey &key) const {
  auto *slot = (TranspTableEntry *) (_table + (key.getValue() % TT_SLOTS));

  if (slot->getKey() == key) {
    return slot;
  }
  return nullptr;
}
