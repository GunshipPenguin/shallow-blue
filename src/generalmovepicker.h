#ifndef GENERALMOVEPICKER_H
#define GENERALMOVEPICKER_H

#include "movepicker.h"
#include "orderinginfo.h"
#include "board.h"

class GeneralMovePicker : MovePicker {
public:
  GeneralMovePicker(const OrderingInfo* _orderingInfo, const Board*, MoveList*);

  bool hasNext() const override;
  Move getNext() override;

private:
  void _scoreMoves();

  size_t _currHead;

  const OrderingInfo* _orderingInfo;
  const Board* _board;
};

#endif