#include "movepicker.h"
#include "qsearchmovepicker.h"
#include "eval.h"

QSearchMovePicker::QSearchMovePicker(MoveList *moveList) : MovePicker(moveList) {
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

void QSearchMovePicker::_scoreMoves() {
  for (auto &move : *_moves) {
    if (move.getFlags() & Move::CAPTURE) {
      move.setValue(CAPTURE_BONUS + _mvvLvaTable[move.getCapturedPieceType()][move.getPieceType()]);
    } else if (move.getFlags() & Move::PROMOTION) {
      move.setValue(PROMOTION_BONUS + Eval::getMaterialValue(move.getPromotionPieceType()));
    }
  }
}

bool QSearchMovePicker::hasNext() const {
  return _currHead < _numCaptures;
}

Move QSearchMovePicker::getNext() {
  size_t bestIndex;
  int bestScore = -INF;

  for (size_t i = _currHead; i < _moves->size(); i++) {
    Move currMove = _moves->at(i);
    int currScore = _moves->at(i).getValue();

    // Disregard non captures and promotions
    if ((currMove.getFlags() & (Move::CAPTURE | Move::PROMOTION)) && (currScore > bestScore)) {
      bestScore = currScore;
      bestIndex = i;
    }
  }

  std::swap(_moves->at(_currHead), _moves->at(bestIndex));
  return _moves->at(_currHead++);
}