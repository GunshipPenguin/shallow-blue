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
  _orderMoves(board, legalMoves);

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

  _tt.set(board.getZKey(), alpha, depth, TranspTable::EXACT);

  _bestMove = bestMove;
  _bestScore = alpha;
}

void Search::_orderMoves(const Board& board, MoveList& moveList) {
  // Order moves by tt score
  std::sort(moveList.begin(), moveList.end(), std::bind(&Search::_compareMovesTt, this, board, std::placeholders::_1, std::placeholders::_2));

  // Ending index of sorted section of moveList
  unsigned int i;

  // Order captures by MVV/LVA
  Board movedBoard;
  for (i=0;i<moveList.size();i++) {
    movedBoard = board;
    movedBoard.doMove(moveList.at(i));

    if (!_tt.contains(movedBoard.getZKey())) {
      break;
    }
  }

  std::sort(moveList.begin() + i, moveList.end(), std::bind(&Search::_compareMovesMvvLva, this, std::placeholders::_1, std::placeholders::_2));

  // Order promotions by promotion value
  for(;i<moveList.size();i++) {
    if (!(moveList.at(i).getFlags() & Move::CAPTURE)) {
      break;
    }
  }
  std::sort(moveList.begin() + i, moveList.end(), std::bind(&Search::_compareMovesPromotionValue, this, std::placeholders::_1, std::placeholders::_2));
}

void Search::_orderMovesQSearch(MoveList& moveList) {
  std::sort(moveList.begin(), moveList.end(), std::bind(&Search::_compareMovesMvvLva, this, std::placeholders::_1, std::placeholders::_2));
}

bool Search::_compareMovesTt(Board board, Move a, Move b) {
  Board aBoard = board;
  Board bBoard = board;

  aBoard.doMove(a);
  bBoard.doMove(b);

  ZKey aKey = aBoard.getZKey();
  ZKey bKey = bBoard.getZKey();

  int aScore = -INF;
  int bScore = -INF;

  // Get A score
  if (_tt.contains(aKey) && _tt.getFlag(aKey) == TranspTable::EXACT) {
    aScore = _tt.getScore(aKey);
  }

  // Get B score
  if (_tt.contains(bKey) && _tt.getFlag(bKey) == TranspTable::EXACT) {
    bScore = _tt.getScore(bKey);
  }

  return aScore < bScore;
}

bool Search::_compareMovesMvvLva(Move a, Move b) {
  bool aIsCapture = a.getFlags() & Move::CAPTURE;
  bool bIsCapture = b.getFlags() & Move::CAPTURE;

  if (aIsCapture && !bIsCapture) {
    return true;
  } else if (bIsCapture && !aIsCapture) {
    return false;
  } else { // Both captures
    int aCaptureValue = _getPieceValue(a.getCapturedPieceType());
    int bCaptureValue = _getPieceValue(b.getCapturedPieceType());

    if (aCaptureValue != bCaptureValue) {
      return aCaptureValue > bCaptureValue;
    } else { // Captured piece type value is the same, order by attacker value
      int aPieceValue = _getPieceValue(a.getPieceType());
      int bPieceValue = _getPieceValue(b.getPieceType());

      return aPieceValue < bPieceValue;
    }
  }
}

bool Search::_compareMovesPromotionValue(Move a, Move b) {
  bool aIsPromotion = a.getFlags() & Move::PROMOTION;
  bool bIsPromotion = b.getFlags() & Move::PROMOTION;

  if (aIsPromotion && !bIsPromotion) {
    return true;
  } else if (bIsPromotion && !aIsPromotion) {
    return false;
  } else { // Both promotions
    int aPromotionValue = _getPieceValue(a.getPromotionPieceType());
    int bPromotionValue = _getPieceValue(b.getPromotionPieceType());

    return aPromotionValue > bPromotionValue;
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

int Search::_negaMax(const Board& board, int depth, int alpha, int beta, MoveList &ppv) {
  _nodes ++;
  int alphaOrig = alpha;

  ZKey zKey = board.getZKey();
  // Check transposition table cache
  if (_tt.contains(zKey) && (_tt.getDepth(zKey) >= depth)) {
    switch(_tt.getFlag(zKey)) {
      case TranspTable::EXACT:
        return _tt.getScore(zKey);
      case TranspTable::UPPER_BOUND:
        beta = std::min(beta, _tt.getScore(zKey));
        break;
      case TranspTable::LOWER_BOUND:
        alpha = std::max(alpha, _tt.getScore(zKey));
        break;
    }

    if (alpha >= beta) {
      return _tt.getScore(zKey);
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
    int score = _qSearch(board, alpha, beta);
    _tt.set(board.getZKey(), score, 0, TranspTable::EXACT);
    return score;
  }

  MoveList pv;
  _orderMoves(board, legalMoves);

  Board movedBoard;
  for (auto move : legalMoves) {
    movedBoard = board;
    movedBoard.doMove(move);

    int score = -_negaMax(movedBoard, depth-1, -beta, -alpha, pv);

    if (score >= beta) {
      _tt.set(zKey, alpha, depth, TranspTable::LOWER_BOUND);
      return beta;
    }

    // Check if alpha raised (new best move)
    if (score > alpha) {
      alpha = score;
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
  TranspTable::Flag flag;
  if (alpha <= alphaOrig) {
    flag = TranspTable::UPPER_BOUND;
  } else {
    flag = TranspTable::EXACT;
  }
  _tt.set(zKey, alpha, depth, flag);

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

  _orderMovesQSearch(legalMoves);
  int standPat = Eval(board, board.getActivePlayer()).getScore();

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
