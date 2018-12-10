#include "orderinginfo.h"
#include <cstring>

OrderingInfo::OrderingInfo(const TranspTable *tt) {
  _tt = tt;
  _ply = 0;
  std::memset(_history, 0, sizeof(_history));
}

void OrderingInfo::incrementHistory(Color color, int from, int to, int depth) {
  _history[color][from][to] += depth * depth;
}

int OrderingInfo::getHistory(Color color, int from, int to) const {
  return _history[color][from][to];
}

void OrderingInfo::incrementPly() {
  _ply++;
}

void OrderingInfo::deincrementPly() {
  _ply--;
}

int OrderingInfo::getPly() const {
  return _ply;
}

const TranspTable *OrderingInfo::getTt() const {
  return _tt;
}

void OrderingInfo::updateKillers(int ply, Move move) {
  _killer2[ply] = _killer1[ply];
  _killer1[ply] = move;
}

Move OrderingInfo::getKiller1(int ply) const {
  return _killer1[ply];
}

Move OrderingInfo::getKiller2(int ply) const {
  return _killer2[ply];
}