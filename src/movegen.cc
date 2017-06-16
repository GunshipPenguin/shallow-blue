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
  _moves.push_back(CMove(from, to, PAWN, flags | CMove::QUEEN_PROMOTION));
  _moves.push_back(CMove(from, to, PAWN, flags | CMove::KNIGHT_PROMOTION));
  _moves.push_back(CMove(from, to, PAWN, flags | CMove::ROOK_PROMOTION));
  _moves.push_back(CMove(from, to, PAWN, flags | CMove::BISHOP_PROMOTION));
}

void MoveGen::genWhitePawnMoves() {
  U64 movedPawns1 = _board.getWhiteBitBoard(PAWN) << 8;
  U64 movedPawns2 = (_board.getWhiteBitBoard(PAWN) & RANK_2) << 16;

  U64 leftAttacks = (_board.getWhiteBitBoard(PAWN) << 7) & (~FILE_H);
  U64 rightAttacks = (_board.getWhiteBitBoard(PAWN) << 9) & (~FILE_A);

  U64 attackablePieces = _board.BLACK_ATTACKABLE | _board.EN_PASSANT;

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if (movedPawns1 & square & _board.NOT_OCCUPIED) {
      if (square & RANK_8) {
        genPawnPromotions(i-8, i);
      } else {
        _moves.push_back(CMove(i-8, i, PAWN));
      }
    }

    if ((movedPawns2 & square & _board.NOT_OCCUPIED) && ((square >> 8) & _board.NOT_OCCUPIED)) {
      _moves.push_back(CMove(i-16, i, PAWN, CMove::DOUBLE_PAWN_PUSH));
    }

    if (leftAttacks & square & attackablePieces) {
      if (square & RANK_8) {
        genPawnPromotions(i-7, i, CMove::CAPTURE);
      } else {
        if (square & _board.EN_PASSANT) {
          _moves.push_back(CMove(i-7, i, PAWN, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i-7, i, PAWN, CMove::CAPTURE));
        }
      }
    }

    if (rightAttacks & square & attackablePieces) {
      if (square & RANK_8) {
        genPawnPromotions(i-9, i, CMove::CAPTURE);
      } else {
        if (square & _board.EN_PASSANT) {
          _moves.push_back(CMove(i-9, i, PAWN, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i-9, i, PAWN, CMove::CAPTURE));
        }
      }
    }
  }
}

void MoveGen::genBlackPawnMoves() {
  U64 movedPawns1 = _board.getBlackBitBoard(PAWN) >> 8;
  U64 movedPawns2 = (_board.getBlackBitBoard(PAWN) & RANK_7) >> 16;

  U64 leftAttacks = (_board.getBlackBitBoard(PAWN) >> 9) & (~FILE_H);
  U64 rightAttacks = (_board.getBlackBitBoard(PAWN) >> 7) & (~FILE_A);

  U64 attackablePieces = _board.WHITE_ATTACKABLE | _board.EN_PASSANT;

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if (movedPawns1 & square & _board.NOT_OCCUPIED) {
      if (square & RANK_1) {
        genPawnPromotions(i+8, i);
      } else {
        _moves.push_back(CMove(i+8, i, PAWN));
      }
    }

    if ((movedPawns2 & square & _board.NOT_OCCUPIED) && ((square << 8) & _board.NOT_OCCUPIED)) {
      _moves.push_back(CMove(i+16, i, PAWN, CMove::DOUBLE_PAWN_PUSH));
    }

    if (leftAttacks & square & attackablePieces) {
      if (square & RANK_1) {
        genPawnPromotions(i+9, i, CMove::CAPTURE);
      } else {
        if (square & _board.EN_PASSANT) {
          _moves.push_back(CMove(i+9, i, PAWN, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i+9, i, PAWN, CMove::CAPTURE));
        }
      }
    }

    if (rightAttacks & square & attackablePieces) {
      if (square & RANK_1) {
        genPawnPromotions(i+7, i, CMove::CAPTURE);
      } else {
        if (square & _board.EN_PASSANT) {
          _moves.push_back(CMove(i+7, i, PAWN, CMove::EN_PASSANT));
        } else {
          _moves.push_back(CMove(i+7, i, PAWN, CMove::CAPTURE));
        }
      }
    }
  }
}

void MoveGen::genWhiteKingMoves() {
  genKingMoves(_board.getWhiteBitBoard(KING), _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);

  if (_board.whiteCanCastleKs()) {
    _moves.push_back(CMove(e1, g1, KING, CMove::KSIDE_CASTLE));
  }
  if (_board.whiteCanCastleQs()) {
    _moves.push_back(CMove(e1, c1, KING, CMove::QSIDE_CASTLE));
  }
}

void MoveGen::genBlackKingMoves() {
  genKingMoves(_board.getBlackBitBoard(KING), _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);

  if (_board.blackCanCastleKs()) {
    _moves.push_back(CMove(e8, g8, KING, CMove::KSIDE_CASTLE));
  }
  if (_board.blackCanCastleQs()) {
    _moves.push_back(CMove(e8, c8, KING, CMove::QSIDE_CASTLE));
  }
}

void MoveGen::genKingMoves(U64 king, U64 own, U64 attackable) {
  if (king == 0) {
    return;
  }

  int kingIndex = __builtin_ffsll(king) - 1;

  U64 moves = _board.getKingAttacksForSquare(kingIndex, own);

  addMoves(kingIndex, KING, moves, attackable);
}

void MoveGen::genWhiteKnightMoves() {
  genKnightMoves(_board.getWhiteBitBoard(KNIGHT), _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);
}

void MoveGen::genBlackKnightMoves() {
  genKnightMoves(_board.getBlackBitBoard(KNIGHT), _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);
}

void MoveGen::genKnightMoves(U64 knights, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = static_cast<U64>(1) << from;
    if ((fromSquare & knights) == 0) {
      continue;
    }

    U64 moves = _board.getKnightAttacksForSquare(from, own);

    addMoves(from, KNIGHT, moves, attackable);
  }
}

void MoveGen::genWhiteBishopMoves() {
  genBishopMoves(_board.getWhiteBitBoard(BISHOP), _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);
}

void MoveGen::genBlackBishopMoves() {
  genBishopMoves(_board.getBlackBitBoard(BISHOP), _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);
}

void MoveGen::genBishopMoves(U64 bishops, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & bishops) == 0) {
      continue;
    }

    U64 moves = _board.getBishopAttacksForSquare(from, own);

    addMoves(from, BISHOP, moves, attackable);
  }
}

void MoveGen::genWhiteRookMoves() {
  genRookMoves(_board.getWhiteBitBoard(ROOK), _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);
}

void MoveGen::genBlackRookMoves() {
  genRookMoves(_board.getBlackBitBoard(ROOK), _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);
}

void MoveGen::genRookMoves(U64 rooks, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & rooks) == 0) {
      continue;
    }

    U64 moves = _board.getRookAttacksForSquare(from, own);

    addMoves(from, ROOK, moves, attackable);
  }
}

void MoveGen::genWhiteQueenMoves() {
  genQueenMoves(_board.getWhiteBitBoard(QUEEN), _board.WHITE_PIECES, _board.BLACK_ATTACKABLE);
}

void MoveGen::genBlackQueenMoves() {
  genQueenMoves(_board.getBlackBitBoard(QUEEN), _board.BLACK_PIECES, _board.WHITE_ATTACKABLE);
}

void MoveGen::genQueenMoves(U64 queens, U64 own, U64 attackable) {
  for(int from=0;from<64;from++) {
    U64 fromSquare = ONE << from;
    if((fromSquare & queens) == 0) {
      continue;
    }

    U64 moves = _board.getQueenAttacksForSquare(from, own);

    addMoves(from, QUEEN, moves, attackable);
  }
}

void MoveGen::addMoves(int from, PieceType pieceType, U64 moves, U64 attackable) {
  for(int to=0;to<64;to++) {
    U64 toSquare = ONE << to;
    if ((toSquare & moves) == 0) {
      continue;
    }

    // Ignore any moves to squares occupied by kings
    if ((toSquare & (_board.getWhiteBitBoard(KING) | _board.getBlackBitBoard(KING)))) {
      continue;
    }

    if(toSquare & attackable) {
      _moves.push_back(CMove(from, to, pieceType, CMove::CAPTURE));
    } else {
      _moves.push_back(CMove(from, to, pieceType));
    }
  }
}
