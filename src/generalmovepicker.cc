#include "movepicker.h"
#include "generalmovepicker.h"
#include "eval.h"

GeneralMovePicker::GeneralMovePicker(const OrderingInfo *orderingInfo, const Board *board, MoveList *moveList)
    : MovePicker(moveList) {
  _orderingInfo = orderingInfo;
  _moves = moveList;
  _board = board;
  _currHead = 0;
  _scoreMoves();
}

void GeneralMovePicker::_scoreMoves() {
  const TranspTableEntry *ttEntry = _orderingInfo->getTt()->getEntry(_board->getZKey());
  Move pvMove;
  if (ttEntry) {
    pvMove = ttEntry->getBestMove();
  }

  for (auto &move : *_moves) {
    if (move == pvMove) {
      move.setValue(INF);
    } else if (move.getFlags() & Move::CAPTURE) {
      move.setValue(CAPTURE_BONUS + _mvvLvaTable[move.getCapturedPieceType()][move.getPieceType()]);
    } else if (move.getFlags() & Move::PROMOTION) {
      move.setValue(PROMOTION_BONUS + Eval::getMaterialValue(move.getPromotionPieceType()));
    } else if (move == _orderingInfo->getKiller1(_orderingInfo->getPly())) {
      move.setValue(KILLER1_BONUS);
    } else if (move == _orderingInfo->getKiller2(_orderingInfo->getPly())) {
      move.setValue(KILLER2_BONUS);
    } else { // Quiet
      move.setValue(QUIET_BONUS + _orderingInfo->getHistory(_board->getActivePlayer(), move.getFrom(), move.getTo()));
    }
  }
}

bool GeneralMovePicker::hasNext() const {
  return _currHead < _moves->size();
}

Move GeneralMovePicker::getNext() {
  size_t bestIndex;
  int bestScore = -INF;

  for (size_t i = _currHead; i < _moves->size(); i++) {
    if (_moves->at(i).getValue() > bestScore) {
      bestScore = _moves->at(i).getValue();
      bestIndex = i;
    }
  }

  std::swap(_moves->at(_currHead), _moves->at(bestIndex));
  return _moves->at(_currHead++);
}