#ifndef MOVEPICKER_H
#define MOVEPICKER_H

#include "board.h"
#include "orderinginfo.h"
#include <cmath>

class MovePicker {
public:
  MovePicker(MoveList*);
  
  virtual Move getNext() = 0;
  virtual bool hasNext() const = 0;

  static void init();
protected:
  MoveList* _moves;

  static int _mvvLvaTable[5][6];
  static constexpr int CAPTURE_BONUS = std::pow(10, 5);
  static constexpr int PROMOTION_BONUS = std::pow(10, 4);
  static constexpr int KILLER1_BONUS = std::pow(10, 3);
  static constexpr int KILLER2_BONUS = std::pow(10, 2);
  static constexpr int QUIET_BONUS = std::pow(10, 1);
};

#endif