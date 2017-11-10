#ifndef MOVEORDERING_H
#define MOVEORDERING_H

#include "transptable.h"
#include "defs.h"
#include "movegen.h"
#include "move.h"

class SearchInfo {
public:
  SearchInfo();
  SearchInfo(const TranspTable*);

  void incrementPly();
  void deincrementPly();
  int getPly() const;

  void updateKillers(int, Move);

  const TranspTable* getTt() const;

  Move getKiller1(int) const;
  Move getKiller2(int) const;

  static void init();
private:
  const TranspTable* _tt;
  Move _killer1[50];
  Move _killer2[50];
  int _ply;

  void _scoreMoves(const Board&, int, MoveList&);
};

#endif