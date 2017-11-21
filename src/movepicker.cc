#include "movepicker.h"
#include "eval.h"
#include "defs.h"

// Indexed by [victimValue][attackerValue]
int MovePicker::_mvvLvaTable[5][6];

void MovePicker::init() {
  int currScore = 0;
  PieceType victimsLoToHi[] = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN};
  PieceType attackersHiToLo[] = {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};

  // Iterate over victims (low to high)
  for (auto victim : victimsLoToHi) {
    // Iterate over attackers (high to low)
    for (auto attacker : attackersHiToLo) {
      _mvvLvaTable[victim][attacker] = currScore++;
    }
  }
}

MovePicker::MovePicker(const OrderingInfo* orderingInfo, const Board* board, MoveList* moveList) {
  _orderingInfo = orderingInfo;
  _moves = moveList;
  _board = board;
  _currHead = 0;  
  _scoreMoves();
}

void MovePicker::_scoreMoves() {
  const TranspTableEntry* ttEntry = _orderingInfo->getTt()->getEntry(_board->getZKey());
  Move pvMove;
  if (ttEntry) {
    pvMove = ttEntry->getBestMove();
  }

  for (auto &move : *_moves) {
    Board tempBoard = *_board;
    tempBoard.doMove(move);

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

bool MovePicker::hasNext() const {
  return _currHead < _moves->size();
}

Move MovePicker::getNext() {
  size_t bestIndex;
  int bestScore = -INF;

  for (size_t i=_currHead;i<_moves->size();i++) {
    if (_moves->at(i).getValue() > bestScore) {
      bestScore = _moves->at(i).getValue();
      bestIndex = i;
    }
  }

  std::swap(_moves->at(_currHead), _moves->at(bestIndex));
  return _moves->at(_currHead++);
}