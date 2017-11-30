#ifndef CAPTUREMOVEPICKER_H
#define CAPTUREMOVEPICKER_H

#include "movepicker.h"

class CaptureMovePicker : MovePicker {
public:
  CaptureMovePicker(MoveList* moves);
  
  bool hasNext() const override;
  Move getNext() override;

private:
  void _scoreMoves();
  
  size_t _currHead;
  size_t _numCaptures;
};

#endif