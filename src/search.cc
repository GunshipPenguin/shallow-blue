#include "defs.h"
#include "search.h"
#include "eval.h"
#include "movegen.h"
#include "transptable.h"
#include "movepicker.h"
#include "generalmovepicker.h"
#include "capturemovepicker.h"
#include <string>
#include <algorithm>
#include <time.h>
#include <iostream>

Search::Search(const Board& board, bool logUci) : _orderingInfo(OrderingInfo(const_cast<TranspTable*>(&_tt))) {
  _logUci = logUci;
  _board = board;
  _bestScore = 0;
  _orderingInfo = OrderingInfo(const_cast<TranspTable*>(&_tt));
}

void Search::perform(int depth) {
  _rootMax(_board, depth);

  if (_logUci) {
    _logUciInfo(_getPv(depth), depth, _bestMove, _bestScore, _nodes);
  }
}

MoveList Search::_getPv(int length) {
  MoveList pv;
  Board currBoard = _board;
  const TranspTableEntry* currEntry;
  int currLength = 0;

  while (currLength++ < length && (currEntry = _tt.getEntry(currBoard.getZKey()))) {
    if (currEntry->getFlag() == TranspTableEntry::EXACT) {
      pv.push_back(currEntry->getBestMove());
      currBoard.doMove(currEntry->getBestMove());
    } else {
      break;
    }
  }

  return pv;
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
  _nodes = 0;

  // If no legal moves are avaliable, just return, setting bestmove to a null move
  if (legalMoves.size() == 0) {
    _bestMove = Move();
    _bestScore = -INF;
    return;
  }

  GeneralMovePicker movePicker(const_cast<OrderingInfo*>(&_orderingInfo), const_cast<Board*>(&board), const_cast<MoveList*>(&legalMoves));

  int alpha = -INF;
  int beta = INF;

  int currScore;

  Move bestMove;
  Board movedBoard;
  while (movePicker.hasNext()) {
    Move move = movePicker.getNext();
    movedBoard = board;
    movedBoard.doMove(move);

    _orderingInfo.incrementPly();
    currScore = -_negaMax(movedBoard, depth-1, -beta, -alpha);
    _orderingInfo.deincrementPly();

    // If the current score is better than alpha, or this is the first move in the loop
    if (currScore > alpha || (alpha == -INF)) {
      bestMove = move;
      alpha = currScore;

      // Break if we've found a checkmate
      if (currScore == INF) {
        break;
      }
    }
  }

  TranspTableEntry ttEntry(alpha, depth, TranspTableEntry::EXACT, bestMove);
  _tt.set(board.getZKey(), ttEntry);

  _bestMove = bestMove;
  _bestScore = alpha;
}

int Search::_negaMax(const Board& board, int depth, int alpha, int beta) {
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
    int score = board.colorIsInCheck(board.getActivePlayer()) ? -INF : 0; // -INF = checkmate, 0 = stalemate (draw)
    return score;
  }

  // Eval if depth is 0
  if (depth == 0) {
    return _qSearch(board, alpha, beta);
  }

  GeneralMovePicker movePicker(const_cast<OrderingInfo*>(&_orderingInfo), const_cast<Board*>(&board), const_cast<MoveList*>(&legalMoves));
  
  Move bestMove;
  Board movedBoard;
  while (movePicker.hasNext()) {
    Move move = movePicker.getNext();

    movedBoard = board;
    movedBoard.doMove(move);

    _orderingInfo.incrementPly();
    int score = -_negaMax(movedBoard, depth-1, -beta, -alpha);
    _orderingInfo.deincrementPly();
    
    // Beta cutoff
    if (score >= beta) {
      // Add this move as a new killer move and update history if move is quiet
      _orderingInfo.updateKillers(_orderingInfo.getPly(), move);
      if (!(move.getFlags() & Move::CAPTURE)) {
        _orderingInfo.incrementHistory(_board.getActivePlayer(), move.getFrom(), move.getTo(), depth);
      }

      // Add a new tt entry for this node
      TranspTableEntry newTTEntry(score, depth, TranspTableEntry::LOWER_BOUND, move);
      _tt.set(board.getZKey(), newTTEntry);
      return beta;
    }

    // Check if alpha raised (new best move)
    if (score > alpha) {
      alpha = score;
      bestMove = move;
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

  int standPat = Eval(board, board.getActivePlayer()).getScore();
  _nodes ++;

  CaptureMovePicker movePicker(const_cast<MoveList*>(&legalMoves));

  // If node is quiet, just return eval
  if (!movePicker.hasNext()) {
    return standPat;
  }

  if (standPat >= beta) {
    return beta;
  }
  if (alpha < standPat) {
    alpha = standPat;
  }

  Board movedBoard;
  while (movePicker.hasNext()) {
    Move move = movePicker.getNext();

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
