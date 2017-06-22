#include "defs.h"
#include "search.h"
#include "eval.h"
#include "movegen.h"
#include "transptable.h"
#include <string>
#include <ostream>
#include <time.h>
#include <iostream>

Search::Search(const Board& board, int depth, int maxTime, bool logUci) {
  _logUci = logUci;
  _iterDeep(board, depth, maxTime);
}

void Search::_iterDeep(const Board& board, int depth, int maxTime) {
  _tt.clear();

  int timeRemaining = maxTime;
  clock_t startTime;
  for(int i=1;i<=depth;i++) {
    startTime = clock();

    _rootMax(board, i);

    clock_t timeTaken = clock() - startTime;
    timeRemaining -= (float(timeTaken) / CLOCKS_PER_SEC)*1000;
    getPv(board);

    // Log UCI info about this iteration
    if (_logUci) {
      std::string pvString;
      for(auto move : getPv(board)) {
        pvString += move.getNotation() + " ";
      }
      std::cout << "info depth " + std::to_string(i) + " ";
      std::cout << "score cp " + std::to_string(_bestScore) + " ";
      std::cout << "pv " + pvString;
      std::cout << std::endl;
    }

    if (timeRemaining < 0) {
      return;
    }
  }
}

MoveList Search::getPv(const Board& board) {
  if (!_tt.contains(board.getZKey())) {
    return MoveList();
  }

  int scoreToFind = -_tt.getScore(board.getZKey());
  ZKey movedZKey;

  for (auto moveBoard : MoveGen(board).getLegalMoves()) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;

    movedZKey = movedBoard.getZKey();

    if (_tt.contains(movedZKey) && _tt.getScore(movedZKey) == scoreToFind) {
      MoveList pvList = getPv(movedBoard);
      pvList.insert(pvList.begin(), move);
      return pvList;
    }
  }

  return MoveList();
}

CMove Search::getBestMove() {
  return _bestMove;
}

void Search::_rootMax(const Board& board, int depth) {
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

  // If we couldn't find a path other than checkmate, just pick the first legal move
  if (bestMove.getFlags() & CMove::NULL_MOVE) {
    bestMove = legalMoves.at(0).first;
  }

  _tt.set(board.getZKey(), bestScore, depth, TranspTable::EXACT);

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

  MoveGen movegen(board);
  MoveBoardList legalMoves = movegen.getLegalMoves();

  // Check for checkmate
  if (legalMoves.size() == 0 && board.colorIsInCheck(board.getActivePlayer())) {
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
