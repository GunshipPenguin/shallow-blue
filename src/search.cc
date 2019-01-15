#include "defs.h"
#include "search.h"
#include "eval.h"
#include "movepicker.h"
#include "generalmovepicker.h"
#include "qsearchmovepicker.h"
#include <algorithm>
#include <iostream>

Search::Search(const Board &board, Limits limits, std::vector<ZKey> positionHistory, bool logUci) :
    _positionHistory(positionHistory),
    _orderingInfo(OrderingInfo(&_tt)),
    _limits(limits),
    _initialBoard(board),
    _logUci(logUci),
    _stop(false),
    _limitCheckCount(0),
    _bestScore(0) {

  if (_limits.infinite) { // Infinite search
    _searchDepth = INF;
    _timeAllocated = INF;
  } else if (_limits.depth != 0) { // Depth search
    _searchDepth = _limits.depth;
    _timeAllocated = INF;
  } else if (_limits.moveTime != 0) {
    _searchDepth = MAX_SEARCH_DEPTH;
    _timeAllocated = _limits.moveTime;
  } else if (_limits.time[_initialBoard.getActivePlayer()] != 0) { // Time search
    int ourTime = _limits.time[_initialBoard.getActivePlayer()];
    int opponentTime = _limits.time[_initialBoard.getInactivePlayer()];

    // Divide up the remaining time (If movestogo not specified we are in 
    // sudden death)
    if (_limits.movesToGo == 0) {
      // Allocate less time for this search if our opponent's time is greater
      // than our time by scaling movestogo by the ratio between our time
      // and our opponent's time (ratio max forced to 2.0, min forced to 1.0)
      double timeRatio = std::max((double) (ourTime / opponentTime), 1.0);

      int movesToGo = (int) (SUDDEN_DEATH_MOVESTOGO * std::min(2.0, timeRatio));
      _timeAllocated = ourTime / movesToGo;
    } else {
      // A small constant (3) is added to _limits.movesToGo when dividing to
      // ensure we don't go over time when movesToGo is small
      _timeAllocated = ourTime / (_limits.movesToGo + 3);
    }

    // Use all of the increment to think
    _timeAllocated += _limits.increment[_initialBoard.getActivePlayer()];

    // Depth is infinity in a timed search (ends when time runs out)
    _searchDepth = MAX_SEARCH_DEPTH;
  } else { // No limits specified, use default depth
    _searchDepth = DEFAULT_SEARCH_DEPTH;
    _timeAllocated = INF;
  }
}

void Search::iterDeep() {
  _start = std::chrono::steady_clock::now();

  for (int currDepth = 1; currDepth <= _searchDepth; currDepth++) {
    _rootMax(_initialBoard, currDepth);

    int elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start).count();

    // If limits were exceeded in the search, break without logging UCI info (search was incomplete)
    if (_stop) break;

    if (_logUci) {
      _logUciInfo(_getPv(currDepth), currDepth, _bestScore, _nodes, elapsed);
    }

    // If the last search has exceeded or hit 50% of the allocated time, stop searching
    if (elapsed >= (_timeAllocated / 2)) break;
  }

  if (_logUci) std::cout << "bestmove " << getBestMove().getNotation() << std::endl;
}

MoveList Search::_getPv(int length) {
  MoveList pv;
  Board currBoard = _initialBoard;
  const TranspTableEntry *currEntry;
  int currLength = 0;

  while (currLength++ < length && (currEntry = _tt.getEntry(currBoard.getZKey()))) {
    pv.push_back(currEntry->getBestMove());
    currBoard.doMove(currEntry->getBestMove());
  }

  return pv;
}

void Search::_logUciInfo(const MoveList &pv, int depth, int bestScore, int nodes, int elapsed) {
  std::string pvString;
  for (auto move : pv) {
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

  // Avoid divide by zero errors with nps
  elapsed++;

  std::cout << "info depth " + std::to_string(depth) + " ";
  std::cout << "nodes " + std::to_string(nodes) + " ";
  std::cout << "score " + scoreString + " ";
  std::cout << "nps " + std::to_string(nodes * 1000 / elapsed) + " ";
  std::cout << "time " + std::to_string(elapsed) + " ";
  std::cout << "pv " + pvString;
  std::cout << std::endl;
}

void Search::stop() {
  _stop = true;
}

Move Search::getBestMove() {
  return _bestMove;
}

bool Search::_checkLimits() {
  if (--_limitCheckCount > 0) {
    return false;
  }

  _limitCheckCount = 4096;

  int elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start).count();

  if (_limits.nodes != 0 && (_nodes >= _limits.nodes)) return true;
  if (elapsed >= (_timeAllocated)) return true;

  return false;
}

void Search::_rootMax(const Board &board, int depth) {
  MoveGen movegen(board);
  MoveList legalMoves = movegen.getLegalMoves();
  _nodes = 0;

  // If no legal moves are available, just return, setting bestmove to a null move
  if (legalMoves.empty()) {
    _bestMove = Move();
    _bestScore = -INF;
    return;
  }

  GeneralMovePicker movePicker
      (&_orderingInfo, const_cast<Board *>(&board), &legalMoves);

  int alpha = -INF;
  int beta = INF;

  int currScore;

  Move bestMove;
  bool fullWindow = true;
  while (movePicker.hasNext()) {
    Move move = movePicker.getNext();

    Board movedBoard = board;
    movedBoard.doMove(move);

    _orderingInfo.incrementPly();
    if (fullWindow) {
      currScore = -_negaMax(movedBoard, depth - 1, -beta, -alpha);
    } else {
      currScore = -_negaMax(movedBoard, depth - 1, -alpha - 1, -alpha);
      if (currScore > alpha) currScore = -_negaMax(movedBoard, depth - 1, -beta, -alpha);
    }
    _orderingInfo.deincrementPly();

    if (_stop || _checkLimits()) {
      _stop = true;
      break;
    }

    // If the current score is better than alpha, or this is the first move in the loop
    if (currScore > alpha) {
      fullWindow = false;
      bestMove = move;
      alpha = currScore;

      // Break if we've found a checkmate
      if (currScore == INF) {
        break;
      }
    }
  }

  // If the best move was not set in the main search loop
  // alpha was not raised at any point, just pick the first move
  // avaliable (arbitrary) to avoid putting a null move in the
  // transposition table
  if (bestMove.getFlags() & Move::NULL_MOVE) {
    bestMove = legalMoves.at(0);
  }

  if (!_stop) {
    TranspTableEntry ttEntry(alpha, depth, TranspTableEntry::EXACT, bestMove);
    _tt.set(board.getZKey(), ttEntry);

    _bestMove = bestMove;
    _bestScore = alpha;
  }
}

int Search::_negaMax(const Board &board, int depth, int alpha, int beta) {
  // Check search limits
  if (_stop || _checkLimits()) {
    _stop = true;
    return 0;
  }

  // Check for threefold repetition draws
  if (std::find(_positionHistory.begin(), _positionHistory.end(), board.getZKey()) != _positionHistory.end()) {
    return 0;
  }

  // Check for 50 move rule draws
  if (board.getHalfmoveClock() >= 50) {
    return 0;
  }

  int alphaOrig = alpha;
  const TranspTableEntry *ttEntry = _tt.getEntry(board.getZKey());
  // Check transposition table cache
  if (ttEntry && (ttEntry->getDepth() >= depth)) {
    switch (ttEntry->getFlag()) {
      case TranspTable::EXACT:return ttEntry->getScore();
      case TranspTable::UPPER_BOUND:beta = std::min(beta, ttEntry->getScore());
        break;
      case TranspTable::LOWER_BOUND:alpha = std::max(alpha, ttEntry->getScore());
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
  if (legalMoves.empty()) {
    int score = board.colorIsInCheck(board.getActivePlayer()) ? -INF : 0; // -INF = checkmate, 0 = stalemate (draw)
    return score;
  }

  // Extend when evading check
  int checkExtension = 0;
  if (board.colorIsInCheck(board.getActivePlayer())) {
    checkExtension = 1;
  }

  // Eval if depth is 0
  if ((depth + checkExtension) == 0) {
    return _qSearch(board, alpha, beta);
  }

  GeneralMovePicker movePicker
      (&_orderingInfo, const_cast<Board *>(&board), &legalMoves);

  Move bestMove;
  bool fullWindow = true;
  while (movePicker.hasNext()) {
    Move move = movePicker.getNext();

    Board movedBoard = board;
    movedBoard.doMove(move);

    int score;
    _orderingInfo.incrementPly();
    if (fullWindow) {
      score = -_negaMax(movedBoard, depth - 1 + checkExtension, -beta, -alpha);
    } else {
      score = -_negaMax(movedBoard, depth - 1 + checkExtension, -alpha - 1, -alpha);
      if (score > alpha) score = -_negaMax(movedBoard, depth - 1 + checkExtension, -beta, -alpha);
    }
    _orderingInfo.deincrementPly();

    // Beta cutoff
    if (score >= beta) {
      // Add this move as a new killer move and update history if move is quiet
      _orderingInfo.updateKillers(_orderingInfo.getPly(), move);
      if (!(move.getFlags() & Move::CAPTURE)) {
        _orderingInfo.incrementHistory(board.getActivePlayer(), move.getFrom(), move.getTo(), depth);
      }

      // Add a new tt entry for this node
      TranspTableEntry newTTEntry(score, depth, TranspTableEntry::LOWER_BOUND, move);
      _tt.set(board.getZKey(), newTTEntry);
      return beta;
    }

    // Check if alpha raised (new best move)
    if (score > alpha) {
      fullWindow = false;
      alpha = score;
      bestMove = move;
    }
  }

  // If the best move was not set in the main search loop
  // alpha was not raised at any point, just pick the first move
  // avaliable (arbitrary) to avoid putting a null move in the
  // transposition table
  if (bestMove.getFlags() & Move::NULL_MOVE) {
    bestMove = legalMoves.at(0);
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

int Search::_qSearch(const Board &board, int alpha, int beta) {
  // Check search limits
  if (_stop || _checkLimits()) {
    _stop = true;
    return 0;
  }

  MoveGen movegen(board);
  MoveList legalMoves = movegen.getLegalMoves();

  // Check for checkmate / stalemate
  if (legalMoves.empty()) {
    if (board.colorIsInCheck(board.getActivePlayer())) { // Checkmate
      return -INF;
    } else { // Stalemate
      return 0;
    }
  }

  int standPat = Eval::evaluate(board, board.getActivePlayer());
  _nodes++;

  QSearchMovePicker movePicker(&legalMoves);

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

  while (movePicker.hasNext()) {
    Move move = movePicker.getNext();

    Board movedBoard = board;
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
