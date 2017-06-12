#include "board.h"
#include "cmove.h"
#include "raytable.h"
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>

Board::Board() {
  setToFen("8/8/8/8/8/8/8/8 w - -");
}

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
  std::istringstream fenStream(fenString);
  std::string token;

  clearBitBoards();

  U64 boardPos = 56; // Fen string starts at a8 = index 56
  fenStream >> token;

  U64 one64 = U64(1);
  for (auto currChar : token) {
    switch(currChar) {
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
        boardPos += static_cast<U64>(currChar - '0');
    }
  }

  // Next to move
  fenStream >> token;
  WHITE_TO_MOVE = token == "w";

  // Castling availability
  fenStream >> token;

  WHITE_CAN_CASTLE_KS = false, WHITE_CAN_CASTLE_QS = false,
    BLACK_CAN_CASTLE_KS = false, BLACK_CAN_CASTLE_QS = false;
  for (auto currChar : token) {
    switch(currChar) {
      case 'K':
        WHITE_CAN_CASTLE_KS = true;
        break;
      case 'Q':
        WHITE_CAN_CASTLE_QS = true;
        break;
      case 'k':
        BLACK_CAN_CASTLE_KS = true;
        break;
      case 'q':
        BLACK_CAN_CASTLE_QS = true;
        break;
    }
  }

  // Get en passant target square
  std::string enPasSquare;
  fenStream >> enPasSquare;

  // Set bitboards
  if (enPasSquare == "-") {
    EN_PASSANT = 0;
  } else {
    int enPasIndex = CMove::notationToIndex(enPasSquare);
    EN_PASSANT = static_cast<U64>(1) << enPasIndex;
  }

  updateBitBoards();
}

U64 Board::getOccupied() {
  return getWhitePieces() | getBlackPieces();
}

U64* Board::getWhiteBitBoard(int squareIndex) {
  U64 square = U64(1) << squareIndex;

  U64* pieces;
  if (square & WHITE_PAWNS) pieces = &WHITE_PAWNS;
  else if (square & WHITE_ROOKS) pieces = &WHITE_ROOKS;
  else if (square & WHITE_KNIGHTS) pieces = &WHITE_KNIGHTS;
  else if (square & WHITE_BISHOPS) pieces = &WHITE_BISHOPS;
  else if (square & WHITE_KING) pieces = &WHITE_KING;
  else if (square & WHITE_QUEENS) pieces = &WHITE_QUEENS;

  return pieces;
}

void Board::updateBitBoards() {
  WHITE_PIECES = getWhitePieces();
  WHITE_ATTACKABLE = WHITE_PIECES & ~WHITE_KING;

  BLACK_PIECES = getBlackPieces();
  BLACK_ATTACKABLE = BLACK_PIECES & ~BLACK_KING;

  OCCUPIED = getOccupied();
  NOT_OCCUPIED = ~OCCUPIED;
}

U64* Board::getBlackBitBoard(int squareIndex) {
  U64 square = U64(1) << squareIndex;

  U64* pieces;
  if (square & BLACK_PAWNS) pieces = &BLACK_PAWNS;
  else if (square & BLACK_ROOKS) pieces = &BLACK_ROOKS;
  else if (square & BLACK_KNIGHTS) pieces = &BLACK_KNIGHTS;
  else if (square & BLACK_BISHOPS) pieces = &BLACK_BISHOPS;
  else if (square & BLACK_KING) pieces = &BLACK_KING;
  else if (square & BLACK_QUEENS) pieces = &BLACK_QUEENS;

  return pieces;
}

void Board::doMove(CMove move) {
  U64* pieces;
  U64* capturePieces = 0;

  if (WHITE_TO_MOVE) {
    pieces = getWhiteBitBoard(move.getFrom());

    if ((move.getFlags() | CMove::CAPTURE) == 0) {
      capturePieces = getBlackBitBoard(move.getTo());
    }
  } else {
    pieces = getBlackBitBoard(move.getFrom());

    if ((move.getFlags() | CMove::CAPTURE) == 0) {
      capturePieces = getWhiteBitBoard(move.getTo());
    }
  }

  U64 fromSquare = U64(1) << move.getFrom();
  U64 toSquare = U64(1) << move.getTo();

  *pieces ^= fromSquare;
  *pieces ^= toSquare;

  if (capturePieces) {
    *capturePieces ^= toSquare;
  }

  WHITE_TO_MOVE = !WHITE_TO_MOVE;
  updateBitBoards();
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

U64 Board::getWhitePawnAttacksForSquare(int square) {
  U64 fromSquare = U64(1) << square;

  U64 attacks = ((fromSquare << 7) & ~FILE_H) | ((fromSquare << 9) & ~FILE_A);

  return attacks;
}

U64 Board::getBlackPawnAttacksForSquare(int square) {
  U64 fromSquare = U64(1) << square;

  U64 attacks = ((fromSquare >> 7) & ~FILE_A) | ((fromSquare >> 9) & ~FILE_H);

  return attacks;
}


U64 Board::getKnightAttacksForSquare(int square, U64 own) {
  U64 fromSquare = U64(1) << square;

  U64 moves = (((fromSquare << 15) | (fromSquare >> 17)) & ~FILE_H) | // Left 1
    (((fromSquare >> 15) | (fromSquare << 17)) & ~FILE_A) | // Right 1
    (((fromSquare << 6) | (fromSquare >> 10)) & ~(FILE_G | FILE_H)) | // Left 2
    (((fromSquare >> 6) | (fromSquare << 10)) & ~(FILE_A | FILE_B)); // Right 2

  return moves & (~own);
}

U64 Board::getKingAttacksForSquare(int square, U64 own) {
  U64 king = U64(1) << square;

  U64 moves = (((king << 7) | (king >> 9) | (king >> 1)) & (~FILE_H)) |
    (((king << 9) | (king >> 7) | (king << 1)) & (~FILE_A)) |
    ((king >> 8) | (king << 8));

  return moves & (~own);
}

U64 Board::getBishopAttacksForSquare(int square, U64 own) {
  U64 moves = raytable.getPositiveAttacks(RayTable::NORTH_WEST, square, OCCUPIED) |
    raytable.getPositiveAttacks(RayTable::NORTH_EAST, square, OCCUPIED) |
    raytable.getNegativeAttacks(RayTable::SOUTH_WEST, square, OCCUPIED) |
    raytable.getPositiveAttacks(RayTable::SOUTH_EAST, square, OCCUPIED);

  return moves & (~own);
}

U64 Board::getRookAttacksForSquare(int square, U64 own) {
  U64 moves = raytable.getPositiveAttacks(RayTable::NORTH, square, OCCUPIED) |
    raytable.getPositiveAttacks(RayTable::EAST, square, OCCUPIED) |
    raytable.getNegativeAttacks(RayTable::SOUTH, square, OCCUPIED) |
    raytable.getNegativeAttacks(RayTable::WEST, square, OCCUPIED);

  return moves & (~own);
}

U64 Board::getQueenAttacksForSquare(int square, U64 own) {
  return getBishopAttacksForSquare(square, own) | getRookAttacksForSquare(square, own);
}

U64 Board::getWhiteAttacks() {
  U64 attacks = U64(0);

  for(int squareIndex=0;squareIndex<64;squareIndex++) {
    U64 square = U64(1) << squareIndex;
    if ((square & WHITE_PIECES) == 0) {
      continue;
    }

    if (square & WHITE_PAWNS) attacks |= getWhitePawnAttacksForSquare(squareIndex);
    else if (square & WHITE_ROOKS) attacks |= getRookAttacksForSquare(squareIndex, WHITE_PIECES);
    else if (square & WHITE_KNIGHTS) attacks |= getKnightAttacksForSquare(squareIndex, WHITE_PIECES);
    else if (square & WHITE_BISHOPS) attacks |= getBishopAttacksForSquare(squareIndex, WHITE_PIECES);
    else if (square & WHITE_KING) attacks |= getKingAttacksForSquare(squareIndex, WHITE_PIECES);
    else if (square & WHITE_QUEENS) attacks |= getQueenAttacksForSquare(squareIndex, WHITE_PIECES);
  }
  attacks |= EN_PASSANT;

  return attacks;
}

U64 Board::getBlackAttacks() {
  U64 attacks = U64(0);

  for(int squareIndex=0;squareIndex<64;squareIndex++) {
    U64 square = U64(1) << squareIndex;
    if ((square & BLACK_PIECES) == 0) {
      continue;
    }

    if (square & BLACK_PAWNS) attacks |= getBlackPawnAttacksForSquare(squareIndex);
    else if (square & BLACK_ROOKS) attacks |= getRookAttacksForSquare(squareIndex, BLACK_PIECES);
    else if (square & BLACK_KNIGHTS) attacks |= getKnightAttacksForSquare(squareIndex, BLACK_PIECES);
    else if (square & BLACK_BISHOPS) attacks |= getBishopAttacksForSquare(squareIndex, BLACK_PIECES);
    else if (square & BLACK_KING) attacks |= getKingAttacksForSquare(squareIndex, BLACK_PIECES);
    else if (square & BLACK_QUEENS) attacks |= getQueenAttacksForSquare(squareIndex, BLACK_PIECES);
  }
  attacks |= EN_PASSANT;

  return attacks;
}
