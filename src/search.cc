#include "defs.h"
#include "search.h"
#include "eval.h"
#include "movegen.h"
#include "transptable.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <string>

Search::Search(const Board& board) {
  _tt.clear();
  _bestMove = rootMax(board, 4);
}

CMove Search::getBestMove() {
  return _bestMove;
}

CMove Search::rootMax(const Board& board, int depth) {
  MoveGen movegen(board);
  MoveBoardList legalMoves = movegen.getLegalMoves();

  int bestScore = -INF;
  int currScore;
  CMove bestMove;
  for (auto moveBoard : legalMoves) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;

    currScore = -negaMax(movedBoard, depth-1, -INF, INF);

    if (currScore > bestScore) {
      bestMove = move;
      bestScore = currScore;
    }
  }

  return bestMove;
}

int Search::negaMax(const Board& board, int depth, int alpha, int beta) {
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

    bestScore = std::max(bestScore, -negaMax(movedBoard, depth-1, -beta, -alpha));
    
    alpha = std::max(alpha, bestScore);
    if (alpha > beta) {
      break;
    }
  }

  return bestScore;
}
