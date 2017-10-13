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
  _bestScore = 0;
}

void Search::perform(int depth) {
  _rootMax(_board, depth);

  if (_logUci) {
    _logUciInfo(_pv, depth, _bestMove, _bestScore, _nodes);
  }
}

void Search::_logUciInfo(const MoveList& pv, int depth, Move bestMove, int bestScore, int nodes) {
  std::string pvString;
  for(auto move : pv) {
    pvString += move.getNotation() + " ";
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
  std::cout << "nodes " + std::to_string(nodes) + " ";
  std::cout << "score " + scoreString + " ";
  std::cout << "pv " + pvString;
  std::cout << std::endl;
}

Move Search::getBestMove() {
  return _bestMove;
}

int Search::getBestScore() {
  return _bestScore;
}

void Search::_rootMax(const Board& board, int depth) {
  MoveGen movegen(board);
  MoveList legalMoves = movegen.getLegalMoves();
  _scoreMoves(board, legalMoves);
  _sortMovesByValue(legalMoves);

  _nodes = 0;

  _pv = MoveList();
  MoveList pv;

  int alpha = -INF;
  int beta = INF;

  int currScore;

  Move bestMove;
  Board movedBoard;
  for (auto move : legalMoves) {
    movedBoard = board;
    movedBoard.doMove(move);

    currScore = -_negaMax(movedBoard, depth-1, -beta, -alpha, pv);

    if (currScore > alpha) {
      bestMove = move;
      alpha = currScore;

      MoveList newMoves;
      newMoves.push_back(move);

      for (auto move : pv) {
        newMoves.push_back(move);
      }
      _pv = newMoves;

      // Break if we've found a checkmate
      if (currScore == INF) {
        break;
      }
    }
  }

  // If we couldn't find a path other than checkmate, just pick the first legal move
  if (bestMove.getFlags() & Move::NULL_MOVE) {
    bestMove = legalMoves.at(0);
  }

  TranspTableEntry ttEntry(alpha, depth, TranspTableEntry::EXACT, bestMove);
  _tt.set(board.getZKey(), ttEntry);

  _bestMove = bestMove;
  _bestScore = alpha;
}

void Search::_scoreMoves(const Board& board, MoveList& moveList) {
  const TranspTableEntry* ttEntry = _tt.getEntry(board.getZKey());
  Move pvMove;
  if (ttEntry) {
    pvMove = ttEntry->getBestMove();
  }

  for (auto &move : moveList) {
    Board tempBoard = board;
    tempBoard.doMove(move);

    // PV move always first
    if (move == pvMove) {
      move.setValue(INF);
      continue;
    }

    // Transposition table lookups considered second (dynamic move ordering)
    const TranspTableEntry *ttEntry = _tt.getEntry(tempBoard.getZKey());
    if (ttEntry && ((ttEntry->getFlag() == TranspTableEntry::EXACT) || (ttEntry->getFlag() == TranspTableEntry::LOWER_BOUND))) {
      // Score is negated since score is for opponent
      move.setValue(-ttEntry->getScore());
    } else {
      // Static move ordering (considers promotions and MVV/LVA captures)
      bool hasStaticTraits = move.getFlags() & (Move::PROMOTION | Move::CAPTURE);
      int value = 0;
      if (move.getFlags() & Move::PROMOTION) {
        value += Eval::getMaterialValue(move.getPromotionPieceType()) - Eval::getMaterialValue(PAWN);
      }
      if (move.getFlags() & Move::CAPTURE) {
        value += Eval::getMaterialValue(move.getCapturedPieceType()) - Eval::getMaterialValue(move.getPieceType());
      }

      if (hasStaticTraits) {
        move.setValue(value);
      } else {
        move.setValue(-INF);
      }
    }
  }
}

void Search::_scoreMovesQsearch(MoveList& moveList) {
  for (auto &move : moveList) {
    if (move.getFlags() & Move::CAPTURE) {
      move.setValue(Eval::getMaterialValue(move.getCapturedPieceType()) - Eval::getMaterialValue(move.getPieceType()));
    } else {
      move.setValue(-INF);
    }
  }
  _sortMovesByValue(moveList);
}

void Search::_sortMovesByValue(MoveList& moveList) {
  std::sort(moveList.begin(), moveList.end(),
    std::bind(&Search::_compareMovesValue, this, std::placeholders::_1, std::placeholders::_2));
}

bool Search::_compareMovesValue(Move a, Move b) {
  return a.getValue() > b.getValue();
}

int Search::_negaMax(const Board& board, int depth, int alpha, int beta, MoveList &ppv) {
  int alphaOrig = alpha;

  const TranspTableEntry* ttEntry = _tt.getEntry(board.getZKey());
  // Check transposition table cache
  if (ttEntry && (ttEntry->getDepth() >= depth)) {
    switch(ttEntry->getFlag()) {
      case TranspTable::EXACT:
        return ttEntry->getScore();
      case TranspTable::UPPER_BOUND:
        beta = std::min(beta, ttEntry->getScore());
        break;
      case TranspTable::LOWER_BOUND:
        alpha = std::max(alpha, ttEntry->getScore());
        break;
    }

    if (alpha >= beta) {
      return ttEntry->getScore();
    }
  }

  // Transposition table lookups are inconclusive, generate moves and recurse
  MoveGen movegen(board);
  MoveList legalMoves = movegen.getLegalMoves();

  // Check for checkmate and stalemate
  if (legalMoves.size() == 0) {
    ppv.clear();
    int score = board.colorIsInCheck(board.getActivePlayer()) ? -INF : 0; // -INF = checkmate, 0 = stalemate (draw)
    return score;
  }

  // Eval if depth is 0
  if (depth == 0) {
    ppv.clear();
    return _qSearch(board, alpha, beta);
  }

  MoveList pv;
  _scoreMoves(board, legalMoves);
  _sortMovesByValue(legalMoves);

  Move bestMove;
  Board movedBoard;
  for (auto move : legalMoves) {
    movedBoard = board;
    movedBoard.doMove(move);

    int score = -_negaMax(movedBoard, depth-1, -beta, -alpha, pv);

    if (score >= beta) {
      TranspTableEntry newTTEntry(alpha, depth, TranspTableEntry::LOWER_BOUND, bestMove);
      _tt.set(board.getZKey(), newTTEntry);
      return beta;
    }

    // Check if alpha raised (new best move)
    if (score > alpha) {
      alpha = score;
      bestMove = move;
      // Copy PV data (if alpha raised then we're on a new PV node)
      MoveList newMoves;
      newMoves.push_back(move);
      for (auto move : pv) {
        newMoves.push_back(move);
      }
      ppv = newMoves;
    }
  }

  // Store bestScore in transposition table
  TranspTableEntry::Flag flag;
  if (alpha <= alphaOrig) {
    flag = TranspTableEntry::UPPER_BOUND;
  } else {
    flag = TranspTableEntry::EXACT;
  }
  TranspTableEntry newTTEntry(alpha, depth, flag, bestMove);
  _tt.set(board.getZKey(), newTTEntry);

  return alpha;
}

int Search::_qSearch(const Board& board, int alpha, int beta) {
  MoveGen movegen(board);
  MoveList legalMoves = movegen.getLegalMoves();

  // Check for checkmate / stalemate
  if (legalMoves.size() == 0) {
    if (board.colorIsInCheck(board.getActivePlayer())) { // Checkmate
      return -INF;
    } else { // Stalemate
      return 0;
    }
  }

  _scoreMovesQsearch(legalMoves);
  _sortMovesByValue(legalMoves);

  int standPat = Eval(board, board.getActivePlayer()).getScore();
  _nodes ++;

  // If node is quiet, just return eval
  if (!(legalMoves.at(0).getFlags() & Move::CAPTURE)) {
    return standPat;
  }

  if (standPat >= beta) {
    return beta;
  }
  if (alpha < standPat) {
    alpha = standPat;
  }

  Board movedBoard;
  for (auto move : legalMoves) {
    if ((move.getFlags() & Move::CAPTURE) == 0) {
      break;
    }

    movedBoard = board;
    movedBoard.doMove(move);

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
