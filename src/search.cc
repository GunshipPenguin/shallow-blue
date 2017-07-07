#include "defs.h"
#include "search.h"
#include "eval.h"
#include "movegen.h"
#include "transptable.h"
#include <string>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <functional>

Search::Search(const Board& board, bool logUci) {
  _logUci = logUci;
  _board = board;
}

void Search::perform(int depth) {
  _rootMax(_board, depth);

  MoveBoardList pv = _getPv(_board);
  if (_logUci) {
    _logUciInfo(pv, depth, _bestMove, _bestScore);
  }
}

void Search::_logUciInfo(const MoveBoardList& pv, int depth, CMove bestMove, int bestScore) {
  std::string pvString;
  for(auto moveBoard : pv) {
    pvString += moveBoard.first.getNotation() + " ";
  }

  std::string scoreString;
  if (bestScore == INF) {
    scoreString = "mate " + std::to_string(pv.size());
  } else if (_bestScore == -INF) {
    scoreString = "mate -" + std::to_string(pv.size());
  } else {
    scoreString = "cp " + std::to_string(bestScore);
  }

  std::cout << "info depth " + std::to_string(depth) + " ";
  std::cout << "score " + scoreString + " ";
  std::cout << "pv " + pvString;
  std::cout << std::endl;
}

MoveBoardList Search::_getPv(const Board& board) {
  int bestScore = INF;
  MoveBoard best;
  bool foundBest = false;
  for (auto moveBoard : MoveGen(board).getLegalMoves()) {
    Board movedBoard = moveBoard.second;

    ZKey movedZKey = movedBoard.getZKey();

    if (_tt.contains(movedZKey) && _tt.getFlag(movedZKey) == TranspTable::EXACT && _tt.getScore(movedZKey) < bestScore) {
      foundBest = true;
      bestScore = _tt.getScore(movedZKey);
      best = moveBoard;
    }
  }

  if (!foundBest) {
    return MoveBoardList();
  } else {
    MoveBoardList pvList = _getPv(best.second);
    pvList.insert(pvList.begin(), best);
    return pvList;
  }
}

CMove Search::getBestMove() {
  return _bestMove;
}

void Search::_rootMax(const Board& board, int depth) {
  MoveGen movegen(board);
  MoveBoardList legalMoves = movegen.getLegalMoves();
  _orderMoves(legalMoves);

  int bestScore = -INF;
  int currScore;
  CMove bestMove;
  for (auto moveBoard : legalMoves) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;

    currScore = -_negaMax(movedBoard, depth-1, -INF, -bestScore);

    if (currScore > bestScore) {
      bestMove = move;
      bestScore = currScore;
    }
  }

  // If we couldn't find a path other than checkmate, just pick the first legal move
  if (bestMove.getFlags() & CMove::NULL_MOVE) {
    bestMove = legalMoves.at(0).first;
  }

  _tt.set(board.getZKey(), bestScore, depth, TranspTable::EXACT);

  _bestMove = bestMove;
  _bestScore = bestScore;
}

void Search::_orderMoves(MoveBoardList& moveBoardList) {
  // Order moves from tt score
  std::sort(moveBoardList.begin(), moveBoardList.end(), std::bind(&Search::_compareMovesTt, this, std::placeholders::_1, std::placeholders::_2));

  // Ending index of sorted section of moveBoardList
  unsigned int i;

  // Order captures by mvv/lva
  for (i=0;i<moveBoardList.size();i++) {
    if (!_tt.contains(moveBoardList.at(i).second.getZKey())) {
      break;
    }
  }
  std::sort(moveBoardList.begin() + i, moveBoardList.end(), std::bind(&Search::_compareMovesMvvLva, this, std::placeholders::_1, std::placeholders::_2));

  // Order promotions by promotion value
  for(;i<moveBoardList.size();i++) {
    if (!(moveBoardList.at(i).first.getFlags() & CMove::CAPTURE)) {
      break;
    }
  }
  std::sort(moveBoardList.begin() + i, moveBoardList.end(), std::bind(&Search::_compareMovesPromotionValue, this, std::placeholders::_1, std::placeholders::_2));
}

void Search::_orderMovesQSearch(MoveBoardList & moveBoardList) {
  std::sort(moveBoardList.begin(), moveBoardList.end(), std::bind(&Search::_compareMovesMvvLva, this, std::placeholders::_1, std::placeholders::_2));
}

bool Search::_compareMovesTt(MoveBoard a, MoveBoard b) {
  ZKey aKey = a.second.getZKey();
  ZKey bKey = b.second.getZKey();
  int aScore = _tt.contains(aKey) ? _tt.getScore(aKey) : -INF;
  int bScore = _tt.contains(bKey) ? _tt.getScore(bKey) : -INF;

  return aScore < bScore;
}

bool Search::_compareMovesMvvLva(MoveBoard a, MoveBoard b) {
  bool aIsCapture = a.first.getFlags() & CMove::CAPTURE;
  bool bIsCapture = b.first.getFlags() & CMove::CAPTURE;

  if (aIsCapture && !bIsCapture) {
    return true;
  } else if (bIsCapture && !aIsCapture) {
    return false;
  } else { // Both captures
    int aPieceValue = _getPieceValue(a.first.getPieceType());
    int bPieceValue = _getPieceValue(b.first.getPieceType());
    int aCaptureValue = _getPieceValue(a.first.getCapturedPieceType());
    int bCaptureValue = _getPieceValue(b.first.getCapturedPieceType());

    return (aCaptureValue - aPieceValue) > (bCaptureValue - bPieceValue);
  }
}

bool Search::_compareMovesPromotionValue(MoveBoard a, MoveBoard b) {
  bool aIsPromotion = a.first.getFlags() & CMove::PROMOTION;
  bool bIsPromotion = b.first.getFlags() & CMove::PROMOTION;

  if (aIsPromotion && !bIsPromotion) {
    return true;
  } else if (bIsPromotion && !aIsPromotion) {
    return false;
  } else { // Both promotions
    int aPromotionValue = _getPieceValue(a.first.getPromotionPieceType());
    int bPromotionValue = _getPieceValue(b.first.getPromotionPieceType());

    return aPromotionValue < bPromotionValue;
  }
}

int Search::_getPieceValue(PieceType pieceType) {
  int score = 0;

  switch(pieceType) {
    case PAWN: score = 1;
      break;
    case KNIGHT: score = 3;
      break;
    case BISHOP: score = 3;
      break;
    case ROOK: score = 5;
      break;
    case QUEEN: score = 9;
      break;
    default: break;
  }

  return score;
}

int Search::_negaMax(const Board& board, int depth, int alpha, int beta) {
  int alphaOrig = alpha;

  ZKey zKey = board.getZKey();
  // Check transposition table cache
  if (_tt.contains(zKey) && (_tt.getDepth(zKey) >= depth)) {
    switch(_tt.getFlag(zKey)) {
      case TranspTable::EXACT:
        return _tt.getScore(zKey);
      case TranspTable::UPPER_BOUND:
        alpha = std::max(alpha, _tt.getScore(zKey));
        break;
      case TranspTable::LOWER_BOUND:
        beta = std::min(beta, _tt.getScore(zKey));
        break;
    }

    if (alpha > beta) {
      return _tt.getScore(zKey);
    }
  }

  // Transposition table lookups are inconclusive, generate moves and recurse
  MoveGen movegen(board);
  MoveBoardList legalMoves = movegen.getLegalMoves();

  // Check for checkmate
  if (legalMoves.size() == 0 && board.colorIsInCheck(board.getActivePlayer())) {
    _tt.set(board.getZKey(), -INF, depth, TranspTable::EXACT);
    return -INF;
  }

  // Eval if depth is 0
  if (depth == 0) {
    int score = _qSearch(board, alpha, beta);
    _tt.set(board.getZKey(), score, 0, TranspTable::EXACT);
    return score;
  }

  _orderMoves(legalMoves);

  int bestScore = -INF;
  for (auto moveBoard : legalMoves) {
    Board movedBoard = moveBoard.second;

    bestScore = std::max(bestScore, -_negaMax(movedBoard, depth-1, -beta, -alpha));

    alpha = std::max(alpha, bestScore);
    if (alpha > beta) {
      break;
    }
  }

  // Store bestScore in transposition table
  TranspTable::Flag flag;
  if (bestScore < alphaOrig) {
    flag = TranspTable::UPPER_BOUND;
  } else if (bestScore >= beta) {
    flag = TranspTable::LOWER_BOUND;
  } else {
    flag = TranspTable::EXACT;
  }
  _tt.set(zKey, bestScore, depth, flag);

  return bestScore;
}

int Search::_qSearch(const Board& board, int alpha, int beta) {
  MoveGen movegen(board);
  MoveBoardList legalMoves = movegen.getLegalMoves();

  // Check for checkmate / stalemate
  if (legalMoves.size() == 0) {
    if (board.colorIsInCheck(board.getActivePlayer())) { // Checkmate
      return -INF;
    } else { // Stalemate
      return 0;
    }
  }

  _orderMovesQSearch(legalMoves);
  int standPat = Eval(board, board.getActivePlayer()).getScore();

  // If node is quiet, just return eval
  if (!(legalMoves.at(0).first.getFlags() & CMove::CAPTURE)) {
    return standPat;
  }

  if (standPat >= beta) {
    return beta;
  }
  if (alpha < standPat) {
    alpha = standPat;
  }

  for (auto moveBoard : legalMoves) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;
    if ((move.getFlags() & CMove::CAPTURE) == 0) {
      break;
    }

    int score = -_qSearch(movedBoard, -beta, -alpha);

    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
    }
  }
  return alpha;
}
