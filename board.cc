#include "board.h"
#include <string>
#include <iostream>

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
  _whiteToMove = true;

  WHITE_PAWNS = 65280ull;
  BLACK_PAWNS = 71776119061217280ull;

  WHITE_ROOKS = 129ull;
  BLACK_ROOKS = 9295429630892703744ull;

  WHITE_KNIGHTS = 66ull;
  BLACK_KNIGHTS = 4755801206503243776ull;

  WHITE_BISHOPS = 36ull;
  BLACK_BISHOPS = 2594073385365405696ull;

  WHITE_QUEENS = 16ull;
  BLACK_QUEENS = 1152921504606846976ull;

  WHITE_KING = 8ull;
  BLACK_KING = 576460752303423488ull;

  WHITE_PIECES = getWhitePieces();
  BLACK_PIECES = getBlackPieces();
  OCCUPIED = getOccupied();
  NOT_OCCUPIED = ~OCCUPIED;
}

U64 Board::getPawnMoves() {
  U64 pawns = _whiteToMove ? WHITE_PAWNS : BLACK_PAWNS;

  U64 potential_moves = pawns << 8;

  U64 base = 1;
  for(U64 i=0;i<64;i++) {
    U64 square = base << i;

    // If this is a square in front of a pawn and is occupied
    if ((potential_moves & square) && (square & OCCUPIED)) {
      // Unset bit
      potential_moves ^= square;
    }
  }

  return potential_moves;
}

U64 Board::getPawnAttacks() {
  U64 pawns = _whiteToMove ? WHITE_PAWNS : BLACK_PAWNS;
  U64 opponent_pieces = _whiteToMove ? BLACK_PIECES : WHITE_PIECES;
  U64 not_opponent_pieces = ~opponent_pieces;

  // Get potential attacks
  U64 potential_attacks = ((pawns << 9) ^ FILE_8) & ((pawns << 8) ^ FILE_1);

  U64 base = 1;
  for(U64 i=0;i<64;i++) {
    U64 square = base << i;

    // If this is a square that a pawn can attack and is not occupied by an opponent
    if ((potential_attacks & square) && (square & not_opponent_pieces)) {
      // Unset bit
      potential_attacks ^= square;
    }
  }

  return potential_attacks;
}
