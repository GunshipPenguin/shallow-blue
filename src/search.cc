#include "defs.h"
#include "search.h"
#include "eval.h"
#include "movegen.h"
#include "transptable.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

Search::Search(const Board& board) {
  _iterDeep(board, 4, 5000);
}

void Search::_iterDeep(const Board& board, int depth, int maxTime) {
  _tt.clear();

  int timeRemaining = maxTime;
  clock_t startTime;
  for(int i=1;i<=depth;i++) {
    startTime = clock();

    rootMax(board, i);

    clock_t timeTaken = clock() - startTime;
    timeRemaining -= (float(timeTaken) / CLOCKS_PER_SEC)*1000;

    if (timeRemaining < 0) {
      return;
    }
  }
}

CMove Search::getBestMove() {
  return _bestMove;
}

void Search::rootMax(const Board& board, int depth) {
  MoveGen movegen(board);
  MoveBoardList legalMoves = movegen.getLegalMoves();

  int bestScore = -INF;
  int currScore;
  CMove bestMove;
  for (auto moveBoard : legalMoves) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;

    currScore = -_negaMax(movedBoard, depth-1, -INF, INF);

    if (currScore > bestScore) {
      bestMove = move;
      bestScore = currScore;
    }
  }

  _bestMove = bestMove;
  _bestScore = bestScore;
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

  if (depth == 0) {
    return Eval(board, board.getActivePlayer()).getScore();
  }

  MoveGen movegen(board);
  MoveBoardList legalMoves = movegen.getLegalMoves();

  // Check for checkmate
  if (legalMoves.size() == 0 && board.colorIsInCheck(board.getActivePlayer())) {
    return -INF;
  }

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
