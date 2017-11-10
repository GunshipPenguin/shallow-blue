#ifndef MOVEPICKER_H
#define MOVEPICKER_H

#include "board.h"
#include "searchinfo.h"

class MovePicker {
public:
  MovePicker(const SearchInfo* searchInfo, const Board* board, MoveList*);
  
  Move getNext();
  bool hasNext() const;

  static void init();
private:
  void _scoreMoves();

  size_t _currHead;

  const SearchInfo* _searchInfo;
  const Board* _board;
  MoveList* _moves;

  static int _mvvLvaTable[5][6];

  static const int CAPTURE_BONUS = 100000;
  static const int PROMOTION_BONUS = 10000;
  static const int KILLER1_BONUS = 1000;
  static const int KILLER2_BONUS = 100;
  static const int QUIET_BONUS = 10;
};

#endif