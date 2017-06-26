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

  for(int i=0;i<64;i++) {
    U64 square = ONE << i;

    if (movedPawns & square & board.getNotOccupied()) {
      if (square & RANK_8) {
        _genPawnPromotions(i-8, i);
      } else {
        _moves.push_back(CMove(i-8, i, PAWN));
      }
    }
  }
}

void MoveGen::_genWhitePawnDoubleMoves(const Board& board) {
  U64 movedPawns = (board.getPieces(WHITE, PAWN) & RANK_2) << 16;

  for(int i=0;i<64;i++) {
    U64 square = ONE << i;
    if ((movedPawns & square & board.getNotOccupied()) && ((square >> 8) & board.getNotOccupied())) {
      _moves.push_back(CMove(i-16, i, PAWN, CMove::DOUBLE_PAWN_PUSH));
    }
  }
}

void MoveGen::_genWhitePawnLeftAttacks(const Board& board) {
  U64 leftAttacks = (board.getPieces(WHITE, PAWN) << 7) & (~FILE_H);
  U64 attackablePieces = board.getAttackable(BLACK) | board.getEnPassant();

  for (int i=0;i<64;i++) {
    U64 square = ONE << i;
    if ((leftAttacks & square & attackablePieces) == ZERO) {
      continue;
    }

    PieceType capturedPieceType = board.getPieceAtSquare(BLACK, i);

    if (square & RANK_8) {
      _genPawnPromotions(i-7, i, CMove::CAPTURE, capturedPieceType);
    } else {
      CMove move = CMove(i-7, i, PAWN);
      if (square & board.getEnPassant()) {
        move.setFlag(CMove::EN_PASSANT);
      } else {
        move.setFlag(CMove::CAPTURE);
        move.setCapturedPieceType(capturedPieceType);
      }
      _moves.push_back(move);
    }
  }
}

void MoveGen::_genWhitePawnRightAttacks(const Board& board) {
  U64 rightAttacks = (board.getPieces(WHITE, PAWN) << 9) & (~FILE_A);
  U64 attackablePieces = board.getAttackable(BLACK) | board.getEnPassant();

  for (int i=0;i<64;i++) {
    U64 square = ONE << i;
    if ((rightAttacks & square & attackablePieces) == ZERO) {
      continue;
    }

    PieceType capturedPieceType = board.getPieceAtSquare(BLACK, i);

    if (square & RANK_8) {
      _genPawnPromotions(i-9, i, CMove::CAPTURE, capturedPieceType);
    } else {
      CMove move = CMove(i-9, i, PAWN);
      if (square & board.getEnPassant()) {
        move.setFlag(CMove::EN_PASSANT);
      } else {
        move.setFlag(CMove::CAPTURE);
        move.setCapturedPieceType(capturedPieceType);
      }
      _moves.push_back(move);
    }
  }
}

void MoveGen::_genBlackPawnSingleMoves(const Board& board) {
  U64 movedPawns = board.getPieces(BLACK, PAWN) >> 8;

  for(int i=0;i<64;i++) {
    U64 square = ONE << i;
    if ((movedPawns & square & board.getNotOccupied()) == ZERO) {
      continue;
    }

    if (square & RANK_1) {
      _genPawnPromotions(i+8, i);
    } else {
      _moves.push_back(CMove(i+8, i, PAWN));
    }
  }
}

void MoveGen::_genBlackPawnDoubleMoves(const Board& board) {
  U64 movedPawns = (board.getPieces(BLACK, PAWN) & RANK_7) >> 16;

  for(int i=0;i<64;i++) {
    U64 square = ONE << i;

    if ((movedPawns & square & board.getNotOccupied()) && ((square << 8) & board.getNotOccupied())) {
      _moves.push_back(CMove(i+16, i, PAWN, CMove::DOUBLE_PAWN_PUSH));
    }
  }
}

void MoveGen::_genBlackPawnLeftAttacks(const Board& board) {
  U64 leftAttacks = (board.getPieces(BLACK, PAWN) >> 9) & (~FILE_H);
  U64 attackablePieces = board.getAttackable(WHITE) | board.getEnPassant();

  for(int i=0;i<64;i++) {
    U64 square = ONE << i;

    if ((leftAttacks & square & attackablePieces) == ZERO) {
      continue;
    }

    PieceType capturedPieceType = board.getPieceAtSquare(WHITE, i);

    if (square & RANK_1) {
      _genPawnPromotions(i+9, i, CMove::CAPTURE, capturedPieceType);
    } else {
      CMove move(i+9, i, PAWN);
      if (square & board.getEnPassant()) {
        move.setFlag(CMove::EN_PASSANT);
      } else {
        move.setFlag(CMove::CAPTURE);
        move.setCapturedPieceType(capturedPieceType);
      }
      _moves.push_back(move);
    }
  }
}

void MoveGen::_genBlackPawnRightAttacks(const Board& board) {
  U64 rightAttacks = (board.getPieces(BLACK, PAWN) >> 7) & (~FILE_A);
  U64 attackablePieces = board.getAttackable(WHITE) | board.getEnPassant();

  for (int i=0;i<64;i++) {
    U64 square = ONE << i;

    if ((rightAttacks & square & attackablePieces) == ZERO) {
      continue;
    }

    PieceType capturedPieceType = board.getPieceAtSquare(WHITE, i);

    if (square & RANK_1) {
      _genPawnPromotions(i+7, i, CMove::CAPTURE, capturedPieceType);
    } else {
      CMove move(i+7, i, PAWN);
      if (square & board.getEnPassant()) {
        move.setFlag(CMove::EN_PASSANT);
      } else {
        move.setFlag(CMove::CAPTURE);
        move.setCapturedPieceType(capturedPieceType);
      }
      _moves.push_back(move);
    }
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
  for(int from=0;from<64;from++) {
    U64 fromSquare = static_cast<U64>(1) << from;
    if ((fromSquare & knights) == 0) {
      continue;
    }

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
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & bishops) == 0) {
      continue;
    }

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
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & rooks) == 0) {
      continue;
    }

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
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & queens) == 0) {
      continue;
    }

    U64 moves = board.getQueenAttacksForSquare(from, own);

    _addMoves(board, from, QUEEN, moves, attackable);
  }
}

void MoveGen::_addMoves(const Board& board, int from, PieceType pieceType, U64 moves, U64 attackable) {
  for(int to=0;to<64;to++) {
    U64 toSquare = ONE << to;
    if ((toSquare & moves) == 0) {
      continue;
    }

    // Ignore any moves to squares occupied by kings
    if ((toSquare & (board.getPieces(WHITE, KING) | board.getPieces(BLACK,  KING)))) {
      continue;
    }

    if(toSquare & attackable) {
      CMove move(from, to, pieceType, CMove::CAPTURE);
      move.setCapturedPieceType(board.getPieceAtSquare(board.getInactivePlayer(), to));

      _moves.push_back(move);
    } else {
      _moves.push_back(CMove(from, to, pieceType));
    }
  }
}
