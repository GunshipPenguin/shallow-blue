#include "searchinfo.h"
#include "defs.h"
#include "eval.h"
#include <cstring>

SearchInfo::SearchInfo() {
  _tt = nullptr;
  _ply = 0;
  std::memset(_history, 0, sizeof(_history));
}

SearchInfo::SearchInfo(const TranspTable* tt) {
  _tt = tt;
  _ply = 0;
  std::memset(_history, 0, sizeof(_history));
}

void SearchInfo::incrementHistory(Color color, int from, int to, int depth) {
  _history[color][from][to] += depth^2;
}

int SearchInfo::getHistory(Color color, int from, int to) const {
  return _history[color][from][to];
}

void SearchInfo::incrementPly() {
  _ply++;
}

void SearchInfo::deincrementPly() {
  _ply--;
}

int SearchInfo::getPly() const {
  return _ply;
}

const TranspTable* SearchInfo::getTt() const {
  return _tt;
}

void SearchInfo::updateKillers(int ply, Move move) {
  _killer2[ply] = _killer1[ply];
  _killer1[ply] = move;
}

Move SearchInfo::getKiller1(int ply) const {
  return _killer1[ply];
}

Move SearchInfo::getKiller2(int ply) const {
  return _killer2[ply];
}