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
  genMoves();
}

MoveList MoveGen::getMoves() {
  return _moves;
}

void MoveGen::genMoves() {
  if (_board.WHITE_TO_MOVE) {
    genWhiteMoves();
  } else {
    genBlackMoves();
  }
}

void MoveGen::genWhiteMoves() {
  genWhitePawnMoves();
  genWhiteRookMoves();
  genWhiteKnightMoves();
  genWhiteBishopMoves();
  genWhiteKingMoves();
  genWhiteQueenMoves();
}

void MoveGen::genBlackMoves() {
  genBlackPawnMoves();
  genBlackRookMoves();
  genBlackKnightMoves();
  genBlackBishopMoves();
  genBlackKingMoves();
  genBlackQueenMoves();
}

void MoveGen::printMoves() {
  for(auto move : getMoves()) {
    std::cout << move.getNotation() << std::endl;
  }
}

void MoveGen::genPawnPromotions(unsigned int from, unsigned int to, unsigned int flags) {
  _moves.push_back(CMove(from, to, flags | CMove::QUEEN_PROMOTION));
  _moves.push_back(CMove(from, to, flags | CMove::KNIGHT_PROMOTION));
  _moves.push_back(CMove(from, to, flags | CMove::ROOK_PROMOTION));
  _moves.push_back(CMove(from, to, flags | CMove::BISHOP_PROMOTION));
}

void MoveGen::genWhitePawnMoves() {
  U64 movedPawns1 = _board.WHITE_PAWNS << 8;
  U64 movedPawns2 = (_board.WHITE_PAWNS & RANK_2) << 16;

  U64 leftAttacks = (_board.WHITE_PAWNS << 7) & (~FILE_H);
  U64 rightAttacks = (_board.WHITE_PAWNS << 9) & (~FILE_A);

  U64 attackablePieces = _board.BLACK_ATTACKABLE | _board.EN_PASSANT;

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if ((movedPawns1 & square) && (square & _board.NOT_OCCUPIED)) {
      if (square & RANK_8) {
        genPawnPromotions(i-8, i);
      } else {
        _moves.push_back(CMove(i-8, i));
      }
    }

    if ((movedPawns2 & square) && (square & _board.NOT_OCCUPIED) && ((square >> 8) & _board.NOT_OCCUPIED)) {
      _moves.push_back(CMove(i-16, i));
    }

    if ((leftAttacks & square) && (square & attackablePieces)) {
      if (square & RANK_8) {
        genPawnPromotions(i-7, i, CMove::CAPTURE);
      } else {
        if (square & _board.EN_PASSANT) {
          _moves.push_back(CMove(i-7, i, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i-7, i, CMove::CAPTURE));
        }
      }
    }

    if ((rightAttacks & square) && (square & attackablePieces)) {
      if (square & RANK_8) {
        genPawnPromotions(i-9, i, CMove::CAPTURE);
      } else {
        if (square & _board.EN_PASSANT) {
          _moves.push_back(CMove(i-9, i, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i-9, i, CMove::CAPTURE));
        }
      }
    }
  }
}

void MoveGen::genBlackPawnMoves() {
  U64 movedPawns1 = _board.BLACK_PAWNS >> 8;
  U64 movedPawns2 = (_board.BLACK_PAWNS & RANK_7) >> 16;

  U64 leftAttacks = (_board.BLACK_PAWNS >> 9) & (~FILE_H);
  U64 rightAttacks = (_board.BLACK_PAWNS >> 7) & (~FILE_A);

  U64 attackablePieces = _board.WHITE_ATTACKABLE | _board.EN_PASSANT;

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if ((movedPawns1 & square) && (square & _board.NOT_OCCUPIED)) {
      if (square & RANK_1) {
        genPawnPromotions(i+8, i);
      } else {
        _moves.push_back(CMove(i+8, i));
      }
    }

    if ((movedPawns2 & square) && (square & _board.NOT_OCCUPIED) && ((square << 8) & _board.NOT_OCCUPIED)) {
      _moves.push_back(CMove(i+16, i));
    }

    if ((leftAttacks & square) && (square & attackablePieces)) {
      if (square & RANK_1) {
        genPawnPromotions(i+9, i, CMove::CAPTURE);
      } else {
        if (square & _board.EN_PASSANT) {
          _moves.push_back(CMove(i+9, i, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i+9, i, CMove::CAPTURE));
        }
      }
    }

    if ((rightAttacks & square) && (square & attackablePieces)) {
      if (square & RANK_1) {
        genPawnPromotions(i+7, i, CMove::CAPTURE);
      } else {
        if (square & _board.EN_PASSANT) {
          _moves.push_back(CMove(i+7, i, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i+7, i, CMove::CAPTURE));
        }
      }
    }
  }
}

void MoveGen::genWhiteKingMoves() {
  genKingMoves(_board.WHITE_KING, _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);

  if (_board.whiteCanCastleKs()) {
    _moves.push_back(CMove(e1, g1, CMove::KSIDE_CASTLE));
  }
  if (_board.whiteCanCastleQs()) {
    _moves.push_back(CMove(e1, b1, CMove::QSIDE_CASTLE));
  }
}

void MoveGen::genBlackKingMoves() {
  genKingMoves(_board.BLACK_KING, _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);

  if (_board.blackCanCastleKs()) {
    _moves.push_back(CMove(e8, g8, CMove::KSIDE_CASTLE));
  }
  if (_board.blackCanCastleQs()) {
    _moves.push_back(CMove(e8, c8, CMove::QSIDE_CASTLE));
  }
}

void MoveGen::genKingMoves(U64 king, U64 own, U64 attackable) {
  if (king == 0) {
    return;
  }

  int kingIndex = __builtin_ffsll(king) - 1;

  U64 moves = _board.getKingAttacksForSquare(kingIndex, own);

  addMoves(kingIndex, moves, attackable);
}

void MoveGen::genWhiteKnightMoves() {
  genKnightMoves(_board.WHITE_KNIGHTS, _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);
}

void MoveGen::genBlackKnightMoves() {
  genKnightMoves(_board.BLACK_KNIGHTS, _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);
}

void MoveGen::genKnightMoves(U64 knights, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = static_cast<U64>(1) << from;
    if ((fromSquare & knights) == 0) {
      continue;
    }

    U64 moves = _board.getKnightAttacksForSquare(from, own);

    addMoves(from, moves, attackable);
  }
}

void MoveGen::genWhiteBishopMoves() {
  genBishopMoves(_board.WHITE_BISHOPS, _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);
}

void MoveGen::genBlackBishopMoves() {
  genBishopMoves(_board.BLACK_BISHOPS, _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);
}

void MoveGen::genBishopMoves(U64 bishops, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = U64(1) << from;
    if((fromSquare & bishops) == 0) {
      continue;
    }

    U64 moves = _board.getBishopAttacksForSquare(from, own);

    addMoves(from, moves, attackable);
  }
}

void MoveGen::genWhiteRookMoves() {
  genRookMoves(_board.WHITE_ROOKS, _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);
}

void MoveGen::genBlackRookMoves() {
  genRookMoves(_board.BLACK_ROOKS, _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);
}

void MoveGen::genRookMoves(U64 rooks, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = U64(1) << from;
    if((fromSquare & rooks) == 0) {
      continue;
    }

    U64 moves = _board.getRookAttacksForSquare(from, own);

    addMoves(from, moves, attackable);
  }
}

void MoveGen::genWhiteQueenMoves() {
  genQueenMoves(_board.WHITE_QUEENS, _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);
}

void MoveGen::genBlackQueenMoves() {
  genQueenMoves(_board.BLACK_QUEENS, _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);
}

void MoveGen::genQueenMoves(U64 queens, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = U64(1) << from;
    if((fromSquare & queens) == 0) {
      continue;
    }

    U64 moves = _board.getQueenAttacksForSquare(from, own);

    addMoves(from, moves, attackable);
  }
}

void MoveGen::addMoves(int from, U64 moves, U64 attackable) {
  for(int to=0;to<64;to++) {
    U64 toSquare = U64(1) << to;
    if ((toSquare & moves) == 0) {
      continue;
    }

    // Ignore any moves to squares occupied by kings
    if ((toSquare & (_board.WHITE_KING | _board.BLACK_KING))) {
      continue;
    }

    if(toSquare & attackable) {
      _moves.push_back(CMove(from, to, CMove::CAPTURE));
    } else {
      _moves.push_back(CMove(from, to));
    }
  }
}
