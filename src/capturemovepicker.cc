#include "movepicker.h"
#include "capturemovepicker.h"

CaptureMovePicker::CaptureMovePicker(MoveList* moveList) : MovePicker(moveList) {
  _currHead = 0;

  // Count captures
  _numCaptures = 0;
  for (auto move : *moveList) {
    if (move.getFlags() & Move::CAPTURE) {
      _numCaptures++;
    }
  }

  _scoreMoves();
}

void CaptureMovePicker::_scoreMoves() {
  for (auto &move : *_moves) {
    if (move.getFlags() & Move::CAPTURE) {
      move.setValue(_mvvLvaTable[move.getCapturedPieceType()][move.getPieceType()]);
    }
  }
}

bool CaptureMovePicker::hasNext() const {
  return _currHead < _numCaptures;
}

Move CaptureMovePicker::getNext() {
  size_t bestIndex;
  int bestScore = -INF;

  for (size_t i=_currHead;i<_moves->size();i++) {
    Move currMove = _moves->at(i);
    int currScore = _moves->at(i).getValue();

    // Disregard non captures
    if ((currMove.getFlags() & Move::CAPTURE) && (currScore > bestScore)) {
      bestScore = currScore;
      bestIndex = i;
    }
  }

  std::swap(_moves->at(_currHead), _moves->at(bestIndex));
  return _moves->at(_currHead++);
}