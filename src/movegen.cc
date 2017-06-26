#include "movegen.h"
#include "board.h"
#include "eval.h"
#include "defs.h"
#include <iostream>

MoveGen::MoveGen(const Board& board) {
  setBoard(board);
}

MoveGen::MoveGen() {
  setBoard(Board());
}

void MoveGen::setBoard(const Board& board) {
  _moves = MoveList();
  _legalMoves = MoveBoardList();
  _genMoves(board);
}

void MoveGen::_genLegalMoves(const Board& board) {
  for (auto move : _moves) {
    Board tempBoard = board;
    tempBoard.doMove(move);

    // Skip this move if it results in moving into check
    if (!(tempBoard.getActivePlayer() == WHITE) && tempBoard.colorIsInCheck(WHITE)) {
      continue;
    } else if (!(tempBoard.getActivePlayer() == BLACK) && tempBoard.colorIsInCheck(BLACK)) {
      continue;
    }

    _legalMoves.push_back(MoveBoard(move, tempBoard));
  }
}

MoveList MoveGen::getMoves() {
  return _moves;
}

MoveBoardList MoveGen::getLegalMoves() {
  return _legalMoves;
}

void MoveGen::_genMoves(const Board& board) {
  switch(board.getActivePlayer()) {
    case WHITE: _genWhiteMoves(board);
      break;
    case BLACK: _genBlackMoves(board);
      break;
  }
  _genLegalMoves(board);
}

void MoveGen::_genWhiteMoves(const Board& board) {
  _genWhitePawnMoves(board);
  _genWhiteRookMoves(board);
  _genWhiteKnightMoves(board);
  _genWhiteBishopMoves(board);
  _genWhiteKingMoves(board);
  _genWhiteQueenMoves(board);
}

void MoveGen::_genBlackMoves(const Board& board) {
  _genBlackPawnMoves(board);
  _genBlackRookMoves(board);
  _genBlackKnightMoves(board);
  _genBlackBishopMoves(board);
  _genBlackKingMoves(board);
  _genBlackQueenMoves(board);
}

void MoveGen::printMoves() {
  for(auto move : getMoves()) {
    std::cout << move.getNotation() << std::endl;
  }
}

void MoveGen::_genPawnPromotions(unsigned int from, unsigned int to, unsigned int flags, PieceType capturedPieceType) {
  CMove promotionBase = CMove(from, to, PAWN, flags | CMove::PROMOTION);
  if (flags & CMove::CAPTURE) {
    promotionBase.setCapturedPieceType(capturedPieceType);
  }

  CMove queenPromotion = promotionBase;
  queenPromotion.setPromotionPieceType(QUEEN);
  _moves.push_back(queenPromotion);

  CMove rookPromotion = promotionBase;
  rookPromotion.setPromotionPieceType(ROOK);
  _moves.push_back(rookPromotion);

  CMove bishopPromotion = promotionBase;
  bishopPromotion.setPromotionPieceType(BISHOP);
  _moves.push_back(bishopPromotion);

  CMove knightPromotion = promotionBase;
  knightPromotion.setPromotionPieceType(KNIGHT);
  _moves.push_back(knightPromotion);
}

void MoveGen::_genWhitePawnSingleMoves(const Board& board) {
  U64 movedPawns = board.getPieces(WHITE, PAWN) << 8;
  movedPawns &= board.getNotOccupied();

  U64 promotions = movedPawns & RANK_8;
  movedPawns &= ~RANK_8;

  // Generate single non promotion moves
  while (movedPawns) {
    int to = _pop_lsb(movedPawns);
    _moves.push_back(CMove(to-8, to, PAWN));
  }

  // Generate promotions
  while (promotions) {
    int to = _pop_lsb(promotions);
    _genPawnPromotions(to-8, to);
  }
}

void MoveGen::_genWhitePawnDoubleMoves(const Board& board) {
  U64 singlePushes = (board.getPieces(WHITE, PAWN) << 8) & board.getNotOccupied();
  U64 doublePushes = (singlePushes << 8) & board.getNotOccupied() & RANK_4;

  while (doublePushes) {
    int to = _pop_lsb(doublePushes);
    _moves.push_back(CMove(to-16, to, PAWN, CMove::DOUBLE_PAWN_PUSH));
  }
}

void MoveGen::_genWhitePawnLeftAttacks(const Board& board) {
  U64 leftRegularAttacks = (board.getPieces(WHITE, PAWN) << 7)  & board.getAttackable(BLACK) & ~FILE_H;

  U64 leftAttackPromotions = leftRegularAttacks & RANK_8;
  leftRegularAttacks &= ~RANK_8;

  U64 leftEnPassant = (board.getPieces(WHITE, PAWN) << 7) & board.getEnPassant() & ~FILE_H;

  // Add regular attacks (Not promotions or en passants)
  while (leftRegularAttacks) {
    int to = _pop_lsb(leftRegularAttacks);

    CMove move = CMove(to-7, to, PAWN, CMove::CAPTURE);
    move.setCapturedPieceType(board.getPieceAtSquare(BLACK, to));

    _moves.push_back(move);
  }

  // Add promotion attacks
  while (leftAttackPromotions) {
    int to = _pop_lsb(leftAttackPromotions);
    _genPawnPromotions(to-7, to, CMove::CAPTURE, board.getPieceAtSquare(BLACK, to));
  }

  // Add en passant attacks
  // There can only be one en passant square at a time, so no need for loop
  if (leftEnPassant) {
    int to = _pop_lsb(leftEnPassant);
    _moves.push_back(CMove(to-7, to, PAWN, CMove::EN_PASSANT));
  }
}

void MoveGen::_genWhitePawnRightAttacks(const Board& board) {
  U64 rightRegularAttacks = (board.getPieces(WHITE, PAWN) << 9)  & board.getAttackable(BLACK) & ~FILE_A;

  U64 rightAttackPromotions = rightRegularAttacks & RANK_8;
  rightRegularAttacks &= ~RANK_8;

  U64 rightEnPassant = (board.getPieces(WHITE, PAWN) << 9) & board.getEnPassant() & ~FILE_A;

  // Add regular attacks (Not promotions or en passants)
  while (rightRegularAttacks) {
    int to = _pop_lsb(rightRegularAttacks);

    CMove move = CMove(to-9, to, PAWN, CMove::CAPTURE);
    move.setCapturedPieceType(board.getPieceAtSquare(BLACK, to));

    _moves.push_back(move);
  }

  // Add promotion attacks
  while (rightAttackPromotions) {
    int to = _pop_lsb(rightAttackPromotions);
    _genPawnPromotions(to-9, to, CMove::CAPTURE, board.getPieceAtSquare(BLACK, to));
  }

  // Add en passant attacks
  // There can only be one en passant square at a time, so no need for loop
  if (rightEnPassant) {
    int to = _pop_lsb(rightEnPassant);
    _moves.push_back(CMove(to-9, to, PAWN, CMove::EN_PASSANT));
  }
}

void MoveGen::_genBlackPawnSingleMoves(const Board& board) {
  U64 movedPawns = board.getPieces(BLACK, PAWN) >> 8;
  movedPawns &= board.getNotOccupied();

  U64 promotions = movedPawns & RANK_1;
  movedPawns &= ~ RANK_1;

  // Generate single non promotion moves'
  while (movedPawns) {
    int to = _pop_lsb(movedPawns);
    _moves.push_back(CMove(to+8, to, PAWN));
  }

  // Generate promotions
  while (promotions) {
    int to = _pop_lsb(promotions);
    _genPawnPromotions(to+8, to);
  }
}

void MoveGen::_genBlackPawnDoubleMoves(const Board& board) {
  U64 singlePushes = (board.getPieces(BLACK, PAWN) >> 8) & board.getNotOccupied();
  U64 doublePushes = (singlePushes >> 8) & board.getNotOccupied() & RANK_5;

  while (doublePushes) {
    int to = _pop_lsb(doublePushes);
    _moves.push_back(CMove(to+16, to, PAWN, CMove::DOUBLE_PAWN_PUSH));
  }
}

void MoveGen::_genBlackPawnLeftAttacks(const Board& board) {
  U64 leftRegularAttacks = (board.getPieces(BLACK, PAWN) >> 9)  & board.getAttackable(WHITE) & ~FILE_H;

  U64 leftAttackPromotions = leftRegularAttacks & RANK_1;
  leftRegularAttacks &= ~RANK_1;

  U64 leftEnPassant = (board.getPieces(BLACK, PAWN) >> 9) & board.getEnPassant() & ~FILE_H;

  // Add regular attacks (Not promotions or en passants)
  while (leftRegularAttacks) {
    int to = _pop_lsb(leftRegularAttacks);

    CMove move = CMove(to+9, to, PAWN, CMove::CAPTURE);
    move.setCapturedPieceType(board.getPieceAtSquare(WHITE, to));

    _moves.push_back(move);
  }

  // Add promotion attacks
  while (leftAttackPromotions) {
    int to = _pop_lsb(leftAttackPromotions);
    _genPawnPromotions(to+9, to, CMove::CAPTURE, board.getPieceAtSquare(WHITE, to));
  }

  // Add en passant attacks
  // There can only be one en passant square at a time, so no need for loop
  if (leftEnPassant) {
    int to = _pop_lsb(leftEnPassant);
    _moves.push_back(CMove(to+9, to, PAWN, CMove::EN_PASSANT));
  }
}

void MoveGen::_genBlackPawnRightAttacks(const Board& board) {
  U64 rightRegularAttacks = (board.getPieces(BLACK, PAWN) >> 7)  & board.getAttackable(WHITE) & ~FILE_A;

  U64 rightAttackPromotions = rightRegularAttacks & RANK_1;
  rightRegularAttacks &= ~RANK_1;

  U64 rightEnPassant = (board.getPieces(BLACK, PAWN) >> 7) & board.getEnPassant() & ~FILE_A;

  // Add regular attacks (Not promotions or en passants)
  while (rightRegularAttacks) {
    int to = _pop_lsb(rightRegularAttacks);

    CMove move = CMove(to+7, to, PAWN, CMove::CAPTURE);
    move.setCapturedPieceType(board.getPieceAtSquare(WHITE, to));

    _moves.push_back(move);
  }

  // Add promotion attacks
  while (rightAttackPromotions) {
    int to = _pop_lsb(rightAttackPromotions);
    _genPawnPromotions(to+7, to, CMove::CAPTURE, board.getPieceAtSquare(WHITE, to));
  }

  // Add en passant attacks
  // There can only be one en passant square at a time, so no need for loop
  if (rightEnPassant) {
    int to = _pop_lsb(rightEnPassant);
    _moves.push_back(CMove(to+7, to, PAWN, CMove::EN_PASSANT));
  }
}

void MoveGen::_genWhitePawnMoves(const Board& board) {
  _genWhitePawnSingleMoves(board);
  _genWhitePawnDoubleMoves(board);
  _genWhitePawnLeftAttacks(board);
  _genWhitePawnRightAttacks(board);
}

void MoveGen::_genBlackPawnMoves(const Board& board) {
  _genBlackPawnSingleMoves(board);
  _genBlackPawnDoubleMoves(board);
  _genBlackPawnLeftAttacks(board);
  _genBlackPawnRightAttacks(board);
}

void MoveGen::_genWhiteKingMoves(const Board& board) {
  _genKingMoves(board, board.getPieces(WHITE, KING), board.getAllPieces(WHITE), board.getAttackable(BLACK));

  if (board.whiteCanCastleKs()) {
    _moves.push_back(CMove(e1, g1, KING, CMove::KSIDE_CASTLE));
  }
  if (board.whiteCanCastleQs()) {
    _moves.push_back(CMove(e1, c1, KING, CMove::QSIDE_CASTLE));
  }
}

void MoveGen::_genBlackKingMoves(const Board& board) {
  _genKingMoves(board, board.getPieces(BLACK, KING), board.getAllPieces(BLACK), board.getAttackable(WHITE));

  if (board.blackCanCastleKs()) {
    _moves.push_back(CMove(e8, g8, KING, CMove::KSIDE_CASTLE));
  }
  if (board.blackCanCastleQs()) {
    _moves.push_back(CMove(e8, c8, KING, CMove::QSIDE_CASTLE));
  }
}

void MoveGen::_genKingMoves(const Board& board, U64 king, U64 own, U64 attackable) {
  if (king == 0) {
    return;
  }

  int kingIndex = __builtin_ffsll(king) - 1;

  U64 moves = board.getKingAttacksForSquare(kingIndex, own);

  _addMoves(board, kingIndex, KING, moves, attackable);
}

void MoveGen::_genWhiteKnightMoves(const Board& board) {
  _genKnightMoves(board, board.getPieces(WHITE, KNIGHT), board.getAllPieces(WHITE), board.getAttackable(BLACK));
}

void MoveGen::_genBlackKnightMoves(const Board& board) {
  _genKnightMoves(board, board.getPieces(BLACK, KNIGHT), board.getAllPieces(BLACK), board.getAttackable(WHITE));
}

void MoveGen::_genKnightMoves(const Board& board, U64 knights, U64 own, U64 attackable) {
  while (knights) {
    int from = _pop_lsb(knights);

    U64 moves = board.getKnightAttacksForSquare(from, own);

    _addMoves(board, from, KNIGHT, moves, attackable);
  }
}

void MoveGen::_genWhiteBishopMoves(const Board& board) {
  _genBishopMoves(board, board.getPieces(WHITE, BISHOP), board.getAllPieces(WHITE), board.getAttackable(BLACK));
}

void MoveGen::_genBlackBishopMoves(const Board& board) {
  _genBishopMoves(board, board.getPieces(BLACK, BISHOP), board.getAllPieces(BLACK), board.getAttackable(WHITE));
}

void MoveGen::_genBishopMoves(const Board& board, U64 bishops, U64 own, U64 attackable) {
  while (bishops) {
    int from = _pop_lsb(bishops);

    U64 moves = board.getBishopAttacksForSquare(from, own);

    _addMoves(board, from, BISHOP, moves, attackable);
  }
}

void MoveGen::_genWhiteRookMoves(const Board& board) {
  _genRookMoves(board, board.getPieces(WHITE, ROOK), board.getAllPieces(WHITE), board.getAttackable(BLACK));
}

void MoveGen::_genBlackRookMoves(const Board& board) {
  _genRookMoves(board, board.getPieces(BLACK, ROOK), board.getAllPieces(BLACK), board.getAttackable(WHITE));
}

void MoveGen::_genRookMoves(const Board& board, U64 rooks, U64 own, U64 attackable) {
  while (rooks) {
    int from = _pop_lsb(rooks);

    U64 moves = board.getRookAttacksForSquare(from, own);

    _addMoves(board, from, ROOK, moves, attackable);
  }
}

void MoveGen::_genWhiteQueenMoves(const Board& board) {
  _genQueenMoves(board, board.getPieces(WHITE, QUEEN), board.getAllPieces(WHITE), board.getAttackable(BLACK));
}

void MoveGen::_genBlackQueenMoves(const Board& board) {
  _genQueenMoves(board, board.getPieces(BLACK, QUEEN), board.getAllPieces(BLACK), board.getAttackable(WHITE));
}

void MoveGen::_genQueenMoves(const Board& board, U64 queens, U64 own, U64 attackable) {
  while (queens) {
    int from = _pop_lsb(queens);

    U64 moves = board.getQueenAttacksForSquare(from, own);

    _addMoves(board, from, QUEEN, moves, attackable);
  }
}

void MoveGen::_addMoves(const Board& board, int from, PieceType pieceType, U64 moves, U64 attackable) {
  // Ignore all moves/attacks to kings
  moves &= ~(board.getPieces(board.getInactivePlayer(), KING));

  // Generate non attacks
  U64 nonAttacks = moves & ~attackable;
  while (nonAttacks) {
    int to = _pop_lsb(nonAttacks);
    _moves.push_back(CMove(from, to, pieceType));
  }

  // Generate attacks
  U64 attacks = moves & attackable;
  while (attacks) {
    int to = _pop_lsb(attacks);

    CMove move(from, to, pieceType, CMove::CAPTURE);
    move.setCapturedPieceType(board.getPieceAtSquare(board.getInactivePlayer(), to));

    _moves.push_back(move);
  }
}

int MoveGen::_pop_lsb(U64& val) {
  int lsbIndex = __builtin_ffsll(val) - 1;
  val &= val-1;
  return lsbIndex;
}
