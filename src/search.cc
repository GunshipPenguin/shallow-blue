#include "defs.h"
#include "search.h"
#include "eval.h"
#include "movegen.h"
#include "transptable.h"
#include <limits>
#include <iostream>

Search::Search(const Board& board) {
  _tt.clear();

  _bestMove = rootMax(board, 5);
}

CMove Search::getBestMove() {
  return _bestMove;
}

CMove Search::rootMax(const Board& board, int depth) {
  Color ownColor = board.getActivePlayer();

  CMove bestMove;

  int alpha = -INF;
  int beta = INF;

  for (auto moveBoard : MoveGen(board).getLegalMoves()) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;

    std::cout << "info Searching: " << move.getNotation() << std::endl;

    int score = negaMax(movedBoard, ownColor, depth-1, alpha, beta);

    if (score >= beta) {
      return bestMove; // Fail hard beta cutoff
    }
    if (score > alpha) {
      alpha = score; // Alpha acts like max in minimax
      bestMove = move;
    }
  }

  return bestMove;
}

int Search::negaMax(const Board& board, Color ownColor, int depth, int alpha, int beta) {
  if (depth == 0) {
    return Eval(board, ownColor).getScore();
  }

  for (auto moveBoard : MoveGen(board).getLegalMoves()) {
    Board movedBoard = moveBoard.second;

    int score;
    if (_tt.contains(movedBoard.getZKey())) {
      score = _tt.get(movedBoard.getZKey());
    } else {
      score = -negaMax(movedBoard, ownColor, depth-1, -beta, -alpha);
      _tt.add(movedBoard.getZKey(), score);
    }

    if (score >= beta) {
      return beta; // Fail hard beta cutoff
    }
    if (score > alpha) {
      alpha = score; // Alpha acts like max in minimax
    }
  }

  return alpha;
}
