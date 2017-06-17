#include "movegen.h"
#include "board.h"
#include "defs.h"
#include <iostream>

MoveGen::MoveGen(Board board) {
  setBoard(board);
}

MoveGen::MoveGen() {
  Board board;
  setBoard(board);
}

void MoveGen::setBoard(Board board) {
  _moves = MoveList();
  _board = board;
  _genMoves();
}

MoveList MoveGen::getMoves() {
  return _moves;
}

void MoveGen::_genMoves() {
  switch(_board.getActivePlayer()) {
    case WHITE: _genWhiteMoves();
      break;
    case BLACK: _genBlackMoves();
      break;
  }
}

void MoveGen::_genWhiteMoves() {
  _genWhitePawnMoves();
  _genWhiteRookMoves();
  _genWhiteKnightMoves();
  _genWhiteBishopMoves();
  _genWhiteKingMoves();
  _genWhiteQueenMoves();
}

void MoveGen::_genBlackMoves() {
  _genBlackPawnMoves();
  _genBlackRookMoves();
  _genBlackKnightMoves();
  _genBlackBishopMoves();
  _genBlackKingMoves();
  _genBlackQueenMoves();
}

void MoveGen::printMoves() {
  for(auto move : getMoves()) {
    std::cout << move.getNotation() << std::endl;
  }
}

void MoveGen::_genPawnPromotions(unsigned int from, unsigned int to, unsigned int flags) {
  _moves.push_back(CMove(from, to, PAWN, flags | CMove::QUEEN_PROMOTION));
  _moves.push_back(CMove(from, to, PAWN, flags | CMove::KNIGHT_PROMOTION));
  _moves.push_back(CMove(from, to, PAWN, flags | CMove::ROOK_PROMOTION));
  _moves.push_back(CMove(from, to, PAWN, flags | CMove::BISHOP_PROMOTION));
}

void MoveGen::_genWhitePawnMoves() {
  U64 movedPawns1 = _board.getPieces(WHITE, PAWN) << 8;
  U64 movedPawns2 = (_board.getPieces(WHITE, PAWN) & RANK_2) << 16;

  U64 leftAttacks = (_board.getPieces(WHITE, PAWN) << 7) & (~FILE_H);
  U64 rightAttacks = (_board.getPieces(WHITE, PAWN) << 9) & (~FILE_A);

  U64 attackablePieces = _board.getAttackable(BLACK) | _board.getEnPassant();

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if (movedPawns1 & square & _board.getNotOccupied()) {
      if (square & RANK_8) {
        _genPawnPromotions(i-8, i);
      } else {
        _moves.push_back(CMove(i-8, i, PAWN));
      }
    }

    if ((movedPawns2 & square & _board.getNotOccupied()) && ((square >> 8) & _board.getNotOccupied())) {
      _moves.push_back(CMove(i-16, i, PAWN, CMove::DOUBLE_PAWN_PUSH));
    }

    if (leftAttacks & square & attackablePieces) {
      if (square & RANK_8) {
        _genPawnPromotions(i-7, i, CMove::CAPTURE);
      } else {
        if (square & _board.getEnPassant()) {
          _moves.push_back(CMove(i-7, i, PAWN, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i-7, i, PAWN, CMove::CAPTURE));
        }
      }
    }

    if (rightAttacks & square & attackablePieces) {
      if (square & RANK_8) {
        _genPawnPromotions(i-9, i, CMove::CAPTURE);
      } else {
        if (square & _board.getEnPassant()) {
          _moves.push_back(CMove(i-9, i, PAWN, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i-9, i, PAWN, CMove::CAPTURE));
        }
      }
    }
  }
}

void MoveGen::_genBlackPawnMoves() {
  U64 movedPawns1 = _board.getPieces(BLACK, PAWN) >> 8;
  U64 movedPawns2 = (_board.getPieces(BLACK, PAWN) & RANK_7) >> 16;

  U64 leftAttacks = (_board.getPieces(BLACK, PAWN) >> 9) & (~FILE_H);
  U64 rightAttacks = (_board.getPieces(BLACK, PAWN) >> 7) & (~FILE_A);

  U64 attackablePieces = _board.getAttackable(WHITE) | _board.getEnPassant();

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if (movedPawns1 & square & _board.getNotOccupied()) {
      if (square & RANK_1) {
        _genPawnPromotions(i+8, i);
      } else {
        _moves.push_back(CMove(i+8, i, PAWN));
      }
    }

    if ((movedPawns2 & square & _board.getNotOccupied()) && ((square << 8) & _board.getNotOccupied())) {
      _moves.push_back(CMove(i+16, i, PAWN, CMove::DOUBLE_PAWN_PUSH));
    }

    if (leftAttacks & square & attackablePieces) {
      if (square & RANK_1) {
        _genPawnPromotions(i+9, i, CMove::CAPTURE);
      } else {
        if (square & _board.getEnPassant()) {
          _moves.push_back(CMove(i+9, i, PAWN, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i+9, i, PAWN, CMove::CAPTURE));
        }
      }
    }

    if (rightAttacks & square & attackablePieces) {
      if (square & RANK_1) {
        _genPawnPromotions(i+7, i, CMove::CAPTURE);
      } else {
        if (square & _board.getEnPassant()) {
          _moves.push_back(CMove(i+7, i, PAWN, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i+7, i, PAWN, CMove::CAPTURE));
        }
      }
    }
  }
}

void MoveGen::_genWhiteKingMoves() {
  _genKingMoves(_board.getPieces(WHITE, KING), _board.getAllPieces(WHITE), _board.getAttackable(BLACK));

  if (_board.whiteCanCastleKs()) {
    _moves.push_back(CMove(e1, g1, KING, CMove::KSIDE_CASTLE));
  }
  if (_board.whiteCanCastleQs()) {
    _moves.push_back(CMove(e1, c1, KING, CMove::QSIDE_CASTLE));
  }
}

void MoveGen::_genBlackKingMoves() {
  _genKingMoves(_board.getPieces(BLACK, KING), _board.getAllPieces(BLACK), _board.getAttackable(WHITE));

  if (_board.blackCanCastleKs()) {
    _moves.push_back(CMove(e8, g8, KING, CMove::KSIDE_CASTLE));
  }
  if (_board.blackCanCastleQs()) {
    _moves.push_back(CMove(e8, c8, KING, CMove::QSIDE_CASTLE));
  }
}

void MoveGen::_genKingMoves(U64 king, U64 own, U64 attackable) {
  if (king == 0) {
    return;
  }

  int kingIndex = __builtin_ffsll(king) - 1;

  U64 moves = _board.getKingAttacksForSquare(kingIndex, own);

  _addMoves(kingIndex, KING, moves, attackable);
}

void MoveGen::_genWhiteKnightMoves() {
  _genKnightMoves(_board.getPieces(WHITE, KNIGHT), _board.getAllPieces(WHITE), _board.getAttackable(BLACK));
}

void MoveGen::_genBlackKnightMoves() {
  _genKnightMoves(_board.getPieces(BLACK, KNIGHT), _board.getAllPieces(BLACK), _board.getAttackable(WHITE));
}

void MoveGen::_genKnightMoves(U64 knights, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = static_cast<U64>(1) << from;
    if ((fromSquare & knights) == 0) {
      continue;
    }

    U64 moves = _board.getKnightAttacksForSquare(from, own);

    _addMoves(from, KNIGHT, moves, attackable);
  }
}

void MoveGen::_genWhiteBishopMoves() {
  _genBishopMoves(_board.getPieces(WHITE, BISHOP), _board.getAllPieces(WHITE), _board.getAttackable(BLACK));
}

void MoveGen::_genBlackBishopMoves() {
  _genBishopMoves(_board.getPieces(BLACK, BISHOP), _board.getAllPieces(BLACK), _board.getAttackable(WHITE));
}

void MoveGen::_genBishopMoves(U64 bishops, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & bishops) == 0) {
      continue;
    }

    U64 moves = _board.getBishopAttacksForSquare(from, own);

    _addMoves(from, BISHOP, moves, attackable);
  }
}

void MoveGen::_genWhiteRookMoves() {
  _genRookMoves(_board.getPieces(WHITE, ROOK), _board.getAllPieces(WHITE), _board.getAttackable(BLACK));
}

void MoveGen::_genBlackRookMoves() {
  _genRookMoves(_board.getPieces(BLACK, ROOK), _board.getAllPieces(BLACK), _board.getAttackable(WHITE));
}

void MoveGen::_genRookMoves(U64 rooks, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & rooks) == 0) {
      continue;
    }

    U64 moves = _board.getRookAttacksForSquare(from, own);

    _addMoves(from, ROOK, moves, attackable);
  }
}

void MoveGen::_genWhiteQueenMoves() {
  _genQueenMoves(_board.getPieces(WHITE, QUEEN), _board.getAllPieces(WHITE), _board.getAttackable(BLACK));
}

void MoveGen::_genBlackQueenMoves() {
  _genQueenMoves(_board.getPieces(BLACK, QUEEN), _board.getAllPieces(BLACK), _board.getAttackable(WHITE));
}

void MoveGen::_genQueenMoves(U64 queens, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & queens) == 0) {
      continue;
    }

    U64 moves = _board.getQueenAttacksForSquare(from, own);

    _addMoves(from, QUEEN, moves, attackable);
  }
}

void MoveGen::_addMoves(int from, PieceType pieceType, U64 moves, U64 attackable) {
  for(int to=0;to<64;to++) {
    U64 toSquare = ONE << to;
    if ((toSquare & moves) == 0) {
      continue;
    }

    // Ignore any moves to squares occupied by kings
    if ((toSquare & (_board.getPieces(WHITE, KING) | _board.getPieces(BLACK,  KING)))) {
      continue;
    }

    if(toSquare & attackable) {
      _moves.push_back(CMove(from, to, pieceType, CMove::CAPTURE));
    } else {
      _moves.push_back(CMove(from, to, pieceType));
    }
  }
}
