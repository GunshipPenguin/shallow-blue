#include "defs.h"
#include "search.h"
#include "eval.h"
#include "movegen.h"
#include "transptable.h"
#include <string>
#include <algorithm>
#include <time.h>
#include <iostream>

Search::Search(const Board& board, int depth, int maxTime, bool logUci) {
  _logUci = logUci;
  _iterDeep(board, depth, maxTime);
}

void Search::_iterDeep(const Board& board, int maxDepth, int maxTime) {
  _tt.clear();

  int timeRemaining = maxTime;
  clock_t startTime;
  MoveBoardList pv;
  for(int currDepth=1;currDepth<=maxDepth;currDepth++) {
    startTime = clock();

    _rootMax(board, currDepth, pv);

    clock_t timeTaken = clock() - startTime;
    timeRemaining -= (float(timeTaken) / CLOCKS_PER_SEC)*1000;

    pv = _getPv(board);

    if (_logUci) {
      _logUciInfo(pv, currDepth, _bestMove, _bestScore);
    }

    if (timeRemaining < 0) {
      return;
    }
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
  if (!_tt.contains(board.getZKey())) {
    return MoveBoardList();
  }

  int scoreToFind = -_tt.getScore(board.getZKey());
  ZKey movedZKey;

  for (auto moveBoard : MoveGen(board).getLegalMoves()) {
    Board movedBoard = moveBoard.second;

    movedZKey = movedBoard.getZKey();

    if (_tt.contains(movedZKey) && _tt.getScore(movedZKey) == scoreToFind) {
      MoveBoardList pvList = _getPv(moveBoard.second);
      pvList.insert(pvList.begin(), moveBoard);
      return pvList;
    }
  }

  return MoveBoardList();
}

CMove Search::getBestMove() {
  return _bestMove;
}

void Search::_rootMax(const Board& board, int depth, MoveBoardList pv) {
  MoveGen movegen(board);
  MoveBoardList legalMoves = movegen.getLegalMoves();
  _orderMoves(legalMoves, pv, depth);

  int bestScore = -INF;
  int currScore;
  CMove bestMove;
  for (auto moveBoard : legalMoves) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;

    currScore = -_negaMax(movedBoard, depth-1, bestScore, INF, pv);

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

void Search::_orderMoves(MoveBoardList& moveBoardList, const MoveBoardList& pv, unsigned int depth) {
  CMove pvMove = (pv.size() > depth) ? pv.at(depth).first : CMove();

  std::sort(moveBoardList.begin(), moveBoardList.end(), [this, pvMove](MoveBoard a, MoveBoard b) {
    if (pvMove == a.first) {
      return true;
    } else if (pvMove == b.first) {
      return false;
    } else {
      ZKey aKey = a.second.getZKey();
      ZKey bKey = b.second.getZKey();
      int aScore = _tt.contains(aKey) ? _tt.getScore(aKey) : -INF;
      int bScore = _tt.contains(bKey) ? _tt.getScore(bKey) : -INF;

      return aScore < bScore;
    }
  });
}

int Search::_negaMax(const Board& board, int depth, int alpha, int beta, const MoveBoardList& pv) {
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
  _orderMoves(legalMoves, pv, depth);

  // Check for checkmate
  if (legalMoves.size() == 0 && board.colorIsInCheck(board.getActivePlayer())) {
    _tt.set(board.getZKey(), -INF, depth, TranspTable::EXACT);
    return -INF;
  }

  // Eval if depth is 0
  if (depth == 0) {
    int score = Eval(board, board.getActivePlayer()).getScore();
    _tt.set(board.getZKey(), score, 0, TranspTable::EXACT);
    return score;
  }

  int bestScore = -INF;
  for (auto moveBoard : legalMoves) {
    Board movedBoard = moveBoard.second;

    bestScore = std::max(bestScore, -_negaMax(movedBoard, depth-1, -beta, -alpha, pv));

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
