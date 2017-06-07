#include "board.h"
#include "cmove.h"
#include <string>
#include <iostream>
#include <string.h>

std::string Board::getStringRep() {
  std::string stringRep;
  U64 base = 1;

  for(U64 i=0;i<64;i++) {
    U64 square = base<<i;
    bool squareOccupied = (square & OCCUPIED) != 0;

    if (i % 8 == 0) {
      stringRep.insert(0, "\n");
    }

    if (squareOccupied) {
      if (square & WHITE_PAWNS) stringRep.insert(0, " P ");
      else if (square & BLACK_PAWNS) stringRep.insert(0, " p ");

      else if (square & WHITE_ROOKS) stringRep.insert(0, " R ");
      else if (square & BLACK_ROOKS) stringRep.insert(0, " r ");

      else if (square & WHITE_KNIGHTS) stringRep.insert(0, " N ");
      else if (square & BLACK_KNIGHTS) stringRep.insert(0, " n ");

      else if (square & WHITE_BISHOPS) stringRep.insert(0, " B ");
      else if (square & BLACK_BISHOPS) stringRep.insert(0, " b ");

      else if (square & WHITE_QUEENS) stringRep.insert(0, " Q ");
      else if (square & BLACK_QUEENS) stringRep.insert(0, " q ");

      else if (square & WHITE_KING) stringRep.insert(0, " K ");
      else if (square & BLACK_KING) stringRep.insert(0, " k ");
    } else {
      stringRep.insert(0, " . ");
    }
  }

  return stringRep;
}

void Board::setToFen(std::string fenString) {
  U64 boardPos = 56; // Fen string starts at a8 = index 56
  unsigned int strIndex = 0;

  U64 one64 = static_cast<U64>(1);

  while(fenString[strIndex] != ' ') {
    switch(fenString[strIndex]) {
      case 'p': BLACK_PAWNS |= (one64 << boardPos++);
        break;
      case 'r': BLACK_ROOKS |= (one64 << boardPos++);
        break;
      case 'n': BLACK_KNIGHTS |= (one64 << boardPos++);
        break;
      case 'b': BLACK_BISHOPS |= (one64 << boardPos++);
        break;
      case 'q': BLACK_QUEENS |= (one64 << boardPos++);
        break;
      case 'k': BLACK_KING |= (one64 << boardPos++);
        break;
      case 'P': WHITE_PAWNS |= (one64 << boardPos++);
        break;
      case 'R': WHITE_ROOKS |= (one64 << boardPos++);
        break;
      case 'N': WHITE_KNIGHTS |= (one64 << boardPos++);
        break;
      case 'B': WHITE_BISHOPS |= (one64 << boardPos++);
        break;
      case 'Q': WHITE_QUEENS |= (one64 << boardPos++);
        break;
      case 'K': WHITE_KING |= (one64 << boardPos++);
        break;
      case '/': boardPos -= 16; // Go down one rank
        break;
      default:
        boardPos += static_cast<U64>(fenString[strIndex] - '0');
    }
    strIndex++;
  }

  // Get next color to move
  // char nextToMove = fenString[strIndex + 1]; // Currently unused
  strIndex += 3;

  // Get castling availability
  int castleStart = strIndex;
  while (fenString[strIndex] != ' ') {
    strIndex ++;
  }
  std::string castlingAval = fenString.substr(castleStart, strIndex);

  // Get en passant target square
  int enPasStart = strIndex + 1;
  while (strIndex != fenString.size()) {
    strIndex ++;
  }
  std::string enPasSquare = fenString.substr(enPasStart, strIndex-enPasStart);

  // Set bitboards
  if (enPasSquare == "-") {
    EN_PASSANT = 0;
  } else {
    int enPasIndex = CMove::notationToIndex(enPasSquare);
    EN_PASSANT = static_cast<U64>(1) << enPasIndex;
  }

  WHITE_PIECES = getWhitePieces();
  WHITE_ATTACKABLE = WHITE_PIECES & ~WHITE_KING;

  BLACK_PIECES = getBlackPieces();
  BLACK_ATTACKABLE = BLACK_PIECES & ~BLACK_KING;

  OCCUPIED = getOccupied();
  NOT_OCCUPIED = ~OCCUPIED;
}

U64 Board::getOccupied() {
  return getWhitePieces() | getBlackPieces();
}

U64 Board::getWhitePieces() {
  return WHITE_PAWNS | \
  WHITE_ROOKS | \
  WHITE_KNIGHTS | \
  WHITE_BISHOPS | \
  WHITE_QUEENS | \
  WHITE_KING;
}

U64 Board::getBlackPieces() {
  return BLACK_PAWNS | \
  BLACK_ROOKS | \
  BLACK_KNIGHTS | \
  BLACK_BISHOPS | \
  BLACK_QUEENS | \
  BLACK_KING;
}

void Board::setToStartPos() {
  setToFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

MoveList Board::getPawnPromotions(unsigned int from, unsigned int to, unsigned int flags) {
  MoveList moves;
  moves.push_back(CMove(from, to, flags | CMove::QUEEN_PROMOTION));
  moves.push_back(CMove(from, to, flags | CMove::KNIGHT_PROMOTION));
  moves.push_back(CMove(from, to, flags | CMove::ROOK_PROMOTION));
  moves.push_back(CMove(from, to, flags | CMove::BISHOP_PROMOTION));

  return moves;
}

MoveList Board::getWhitePawnMoves() {
  MoveList potentialMoves;

  // Move all pawns up 1
  U64 movedPawns1 = WHITE_PAWNS << 8;
  U64 movedPawns2 = (WHITE_PAWNS & RANK_2) << 16;

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if ((movedPawns1 & square) && (square & NOT_OCCUPIED)) {
      if (square & RANK_8) {
        MoveList promotions = getPawnPromotions(i-8, i);
        potentialMoves.insert(potentialMoves.end(), promotions.begin(), promotions.end());
      } else {
        potentialMoves.push_back(CMove(i-8, i));
      }
    }

    if ((movedPawns2 & square) && (square & NOT_OCCUPIED)) {
      potentialMoves.push_back(CMove(i-16, i));
    }
  }

  return potentialMoves;
}

MoveList Board::getBlackPawnMoves() {
  MoveList potentialMoves;

  U64 movedPawns1 = BLACK_PAWNS >> 8;
  U64 movedPawns2 = (BLACK_PAWNS & RANK_7) >> 16;
  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if ((movedPawns1 & square) && (square & NOT_OCCUPIED)) {
      if (square & RANK_1) {
        MoveList promotions = getPawnPromotions(i+8, i);
        potentialMoves.insert(potentialMoves.end(), promotions.begin(), promotions.end());
      } else {
        potentialMoves.push_back(CMove(i+8, i));
      }
    }

    if ((movedPawns2 & square) && (square & NOT_OCCUPIED)) {
      potentialMoves.push_back(CMove(i+16, i));
    }
  }

  return potentialMoves;
}

MoveList Board::getWhitePawnAttacks() {
  MoveList potentialAttacks;

  U64 leftAttacks = (WHITE_PAWNS << 7) & (~FILE_H);
  U64 rightAttacks = (WHITE_PAWNS << 9) & (~FILE_A);
  U64 attackablePieces = BLACK_PIECES | EN_PASSANT;

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if ((leftAttacks & square) && (square & attackablePieces)) {
      if (square & RANK_8) {
        MoveList promotions = getPawnPromotions(i-7, i, CMove::CAPTURE);
        potentialAttacks.insert(potentialAttacks.end(), promotions.begin(), promotions.end());
      } else {
        potentialAttacks.push_back(CMove(i-7, i, CMove::CAPTURE));
      }
    }

    if ((rightAttacks & square) && (square & attackablePieces)) {
      if (square & RANK_8) {
        MoveList promotions = getPawnPromotions(i-9, i, CMove::CAPTURE);
        potentialAttacks.insert(potentialAttacks.end(), promotions.begin(), promotions.end());
      } else {
        potentialAttacks.push_back(CMove(i-9, i, CMove::CAPTURE));
      }
    }
  }

  return potentialAttacks;
}

MoveList Board::getBlackPawnAttacks() {
  MoveList potentialAttacks;

  U64 leftAttacks = (BLACK_PAWNS >> 9) & (~FILE_H);
  U64 rightAttacks = (BLACK_PAWNS >> 7) & (~FILE_A);

  U64 attackablePieces = WHITE_PIECES | EN_PASSANT;

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if ((leftAttacks & square) && (square & attackablePieces)) {
      if (square & RANK_1) {
        MoveList promotions = getPawnPromotions(i+9, i, CMove::CAPTURE);
        potentialAttacks.insert(potentialAttacks.end(), promotions.begin(), promotions.end());
      } else {
        potentialAttacks.push_back(CMove(i+9, i, CMove::CAPTURE));
      }
    }

    if ((rightAttacks & square) && (square & attackablePieces)) {
      if (square & RANK_1) {
        MoveList promotions = getPawnPromotions(i+7, i, CMove::CAPTURE);
        potentialAttacks.insert(potentialAttacks.end(), promotions.begin(), promotions.end());
      } else {
        potentialAttacks.push_back(CMove(i+7, i, CMove::CAPTURE));
      }
    }
  }

  return potentialAttacks;
}

MoveList Board::getWhiteKingMoves() {
  return getKingMoves(WHITE_KING, WHITE_PIECES, BLACK_ATTACKABLE);
}

MoveList Board::getBlackKingMoves() {
  return getKingMoves(BLACK_KING, BLACK_PIECES, WHITE_ATTACKABLE);
}

MoveList Board::getKingMoves(U64 king, U64 own, U64 attackable) {
  MoveList possibleMoves;

  int kingIndex = ffsll(king) - 1;


  U64 moves = (((king << 7) | (king >> 9) | (king >> 1)) & (~FILE_H)) |
    (((king << 9) | (king >> 7) | (king << 1)) & (~FILE_A)) |
    ((king >> 8) | (king << 8));

  for(U64 i=0;i<64;i++) {
    U64 square = static_cast<U64>(1) << i;

    if (square & moves) {
      if (square & attackable) {
        possibleMoves.push_back(CMove(kingIndex, i, CMove::CAPTURE));
      } else {
        possibleMoves.push_back(CMove(kingIndex, i));
      }
    }
  }

  return possibleMoves;
}
