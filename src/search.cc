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

  for (int i=1;i<9;i++) {
    _bestMove = rootMax(board, i);
    std::string pvString = "info depth " + std::to_string(i);
    for(auto move : getPv(board, _tt.getScore(board.getZKey()), i)) {
      pvString += " " + move.getNotation();
    }
    std::cout << pvString << std::endl;
  }
}

MoveList Search::getPv(const Board& board, int score, int depth) {
  if (depth == 0) {
    return MoveList();
  }

  for (auto moveBoard : MoveGen(board).getLegalMoves()) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;


    if (_tt.contains(movedBoard.getZKey())) {
      if(_tt.getScore(movedBoard.getZKey()) == score) {
        MoveList pvList = getPv(movedBoard, -score, depth-1);
        pvList.insert(pvList.begin(), move);
        return pvList;
      }
    }
  }

  throw "Principal variation not found in transposition table";
}

CMove Search::getBestMove() {
  return _bestMove;
}

CMove Search::rootMax(const Board& board, int depth) {
  CMove bestMove;

  int max = -INF;

  for (auto moveBoard : MoveGen(board).getLegalMoves()) {
    CMove move = moveBoard.first;
    Board movedBoard = moveBoard.second;

    int score;
    if (_tt.contains(movedBoard.getZKey()) && _tt.getDepth(movedBoard.getZKey()) >= depth) {
      score = _tt.getScore(movedBoard.getZKey());
    } else {
      score = -negaMax(movedBoard, depth-1, -INF, INF);
      _tt.set(movedBoard.getZKey(), score, depth);
    }

    if (score > max) {
      max = score;
      bestMove = move;
    }
  }

  _tt.set(board.getZKey(), max, depth);

  return bestMove;
}

int Search::negaMax(const Board& board, int depth, int alpha, int beta) {
  if (depth == 0) {
    return Eval(board, board.getActivePlayer()).getScore();
  }

  for (auto moveBoard : MoveGen(board).getLegalMoves()) {
    Board movedBoard = moveBoard.second;

    int score;
    if (_tt.contains(movedBoard.getZKey()) && _tt.getDepth(movedBoard.getZKey()) >= depth) {
      score = _tt.getScore(movedBoard.getZKey());
    } else {
      score = -negaMax(movedBoard, depth-1, -beta, -alpha);
      _tt.set(movedBoard.getZKey(), score, depth);
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
