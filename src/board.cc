#include "board.h"
#include "cmove.h"
#include "raytable.h"
#include <string>
#include <iostream>
#include <string.h>

std::string Board::getStringRep() {
  std::string stringRep;
  U64 base = 1;
  U64 boardPos = 56; // Starts at a8, goes down rank by rank
  int squaresProcessed = 0;

  while (squaresProcessed < 64) {
    U64 square = base << boardPos;
    bool squareOccupied = (square & OCCUPIED) != 0;

    if (squareOccupied) {
      if (square & WHITE_PAWNS) stringRep += " P ";
      else if (square & BLACK_PAWNS) stringRep += " p ";

      else if (square & WHITE_ROOKS) stringRep += " R ";
      else if (square & BLACK_ROOKS) stringRep += " r ";

      else if (square & WHITE_KNIGHTS) stringRep += " N ";
      else if (square & BLACK_KNIGHTS) stringRep += " n ";

      else if (square & WHITE_BISHOPS) stringRep += " B ";
      else if (square & BLACK_BISHOPS) stringRep += " b ";

      else if (square & WHITE_QUEENS) stringRep += " Q ";
      else if (square & BLACK_QUEENS) stringRep += " q ";

      else if (square & WHITE_KING) stringRep += " K ";
      else if (square & BLACK_KING) stringRep += " k ";
    } else {
      stringRep += " . ";
    }

    if ((squaresProcessed+1) % 8 == 0) {
      stringRep += '\n';
      boardPos -= 16;
    }

    boardPos ++;
    squaresProcessed ++;
  }

  stringRep.pop_back();
  return stringRep;
}

void Board::clearBitBoards() {
  WHITE_PAWNS = 0ull;
  BLACK_PAWNS = 0ull;

  WHITE_ROOKS = 0ull;
  BLACK_ROOKS = 0ull;

  WHITE_KNIGHTS = 0ull;
  BLACK_KNIGHTS = 0ull;

  WHITE_BISHOPS = 0ull;
  BLACK_BISHOPS = 0ull;

  WHITE_QUEENS = 0ull;
  BLACK_QUEENS = 0ull;

  WHITE_KING = 0ull;
  BLACK_KING = 0ull;

  WHITE_PIECES = 0ull;
  BLACK_PIECES = 0ull;

  WHITE_ATTACKABLE = 0ull;
  BLACK_ATTACKABLE = 0ull;

  EN_PASSANT = 0ull;

  OCCUPIED = 0ull;
  NOT_OCCUPIED = 0ull;

  return;
}

void Board::setToFen(std::string fenString) {
  clearBitBoards();

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
    if ((square & moves) == 0 || (square & own) != 0) {
      continue;
    }

    if (square & attackable) {
      possibleMoves.push_back(CMove(kingIndex, i, CMove::CAPTURE));
    } else {
      possibleMoves.push_back(CMove(kingIndex, i));
    }
  }

  return possibleMoves;
}

MoveList Board::getWhiteKnightMoves() {
  return getKnightMoves(WHITE_KNIGHTS, WHITE_PIECES, BLACK_ATTACKABLE);
}

MoveList Board::getBlackKnightMoves() {
  return getKnightMoves(BLACK_KNIGHTS, BLACK_PIECES, WHITE_ATTACKABLE);
}

MoveList Board::getKnightMoves(U64 knights, U64 own, U64 attackable) {
  MoveList possibleMoves;
  for(U64 from=0;from<64;from++) {
    U64 fromSquare = static_cast<U64>(1) << from;
    if ((fromSquare & knights) == 0) {
      continue;
    }

    U64 moves = (((fromSquare << 15) | (fromSquare >> 17)) & ~FILE_H) | // Left 1
      (((fromSquare >> 15) | (fromSquare << 17)) & ~FILE_A) | // Right 1
      (((fromSquare << 6) | (fromSquare >> 10)) & ~(FILE_G | FILE_H)) | // Left 2
      (((fromSquare >> 6) | (fromSquare << 10)) & ~(FILE_A | FILE_B)); // Right 2

    for(U64 to=0;to<64;to++) {
      U64 toSquare = static_cast<U64>(1) << to;
      if ((toSquare & moves) == 0 || (toSquare & own) != 0) {
        continue;
      }

      if (toSquare & attackable) {
        possibleMoves.push_back(CMove(from, to, CMove::CAPTURE));
      } else {
        possibleMoves.push_back(CMove(from, to));
      }
    }
  }

  return possibleMoves;
}

MoveList Board::getWhiteBishopMoves() {
  return getBishopMoves(WHITE_BISHOPS, WHITE_PIECES, BLACK_PIECES);
}

MoveList Board::getBlackBishopMoves() {
  return getBishopMoves(BLACK_BISHOPS, BLACK_PIECES, BLACK_PIECES);
}

MoveList Board::getBishopMoves(U64 bishops, U64 own, U64 attackable) {
  MoveList possibleMoves;
  for(int from=0;from<64;from++) {
    U64 fromSquare = U64(1) << from;
    if((fromSquare & bishops) == 0) {
      continue;
    }

    U64 moves = raytable.getPositiveAttacks(RayTable::NORTH_WEST, from, OCCUPIED) |
      raytable.getPositiveAttacks(RayTable::NORTH_EAST, from, OCCUPIED) |
      raytable.getNegativeAttacks(RayTable::SOUTH_WEST, from, OCCUPIED) |
      raytable.getPositiveAttacks(RayTable::SOUTH_EAST, from, OCCUPIED);

    // Remove any attacks to own pieces
    moves &= ~own;

    for(int to=0;to<64;to++) {
      U64 toSquare = U64(1) << to;
      if ((toSquare & moves) == 0) {
        continue;
      }

      if(toSquare & attackable) {
        possibleMoves.push_back(CMove(from, to, CMove::CAPTURE));
      } else {
        possibleMoves.push_back(CMove(from, to));
      }
    }
  }

  return possibleMoves;
}
