#include "board.h"
#include "cmove.h"
#include "raytable.h"
#include "defs.h"
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>

RayTable Board::raytable = RayTable();

Board::Board() {
  setToFen("8/8/8/8/8/8/8/8 w - -");
}

bool Board::whiteIsInCheck() {
  return (bool) (BLACK_ATTACKS & WHITE_KING);
}

bool Board::blackIsInCheck() {
  return (bool) (WHITE_ATTACKS & BLACK_KING);
}

bool Board::whiteCanCastleKs() {
  if (!WHITE_CAN_CASTLE_KS) {
    return false;
  }

  U64 passThroughSquares = (ONE << f1) | (ONE << g1);
  bool squaresOccupied = passThroughSquares & OCCUPIED;
  bool squaresAttacked = passThroughSquares & BLACK_ATTACKS;

  return !WHITE_KING_HAS_MOVED && !WHITE_KS_ROOK_HAS_MOVED && !whiteIsInCheck() &&
    !squaresOccupied && !squaresAttacked;
}

bool Board::whiteCanCastleQs() {
  if (!WHITE_CAN_CASTLE_QS) {
    return false;
  }

  U64 passThroughSquares = (ONE << c1) | (ONE << d1);
  bool squaresOccupied = passThroughSquares & OCCUPIED;
  bool squaresAttacked = passThroughSquares & BLACK_ATTACKS;

  return !WHITE_KING_HAS_MOVED && !WHITE_QS_ROOK_HAS_MOVED && !whiteIsInCheck() &&
    !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleKs() {
  if (!BLACK_CAN_CASTLE_KS) {
    return false;
  }

  U64 passThroughSquares = (ONE << f8) | (ONE << g8);
  bool squaresOccupied = passThroughSquares & OCCUPIED;
  bool squaresAttacked = passThroughSquares & WHITE_ATTACKS;

  return !BLACK_KING_HAS_MOVED && !BLACK_KS_ROOK_HAS_MOVED && !blackIsInCheck() &&
    !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleQs() {
  if (!BLACK_CAN_CASTLE_QS) {
    return false;
  }

  U64 passThroughSquares = (ONE << c8) | (ONE << d8);
  bool squaresOccupied = passThroughSquares & OCCUPIED;
  bool squaresAttacked = passThroughSquares & WHITE_ATTACKS;

  return !BLACK_KING_HAS_MOVED && !BLACK_QS_ROOK_HAS_MOVED && !blackIsInCheck() &&
    !squaresOccupied && !squaresAttacked;
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
  WHITE_PAWNS =ZERO;
  BLACK_PAWNS =ZERO;

  WHITE_ROOKS =ZERO;
  BLACK_ROOKS =ZERO;

  WHITE_KNIGHTS =ZERO;
  BLACK_KNIGHTS =ZERO;

  WHITE_BISHOPS =ZERO;
  BLACK_BISHOPS =ZERO;

  WHITE_QUEENS =ZERO;
  BLACK_QUEENS =ZERO;

  WHITE_KING =ZERO;
  BLACK_KING =ZERO;

  WHITE_PIECES =ZERO;
  BLACK_PIECES =ZERO;

  WHITE_ATTACKABLE =ZERO;
  BLACK_ATTACKABLE =ZERO;

  EN_PASSANT =ZERO;

  OCCUPIED =ZERO;
  NOT_OCCUPIED =ZERO;

  return;
}

void Board::setToFen(std::string fenString) {
  std::istringstream fenStream(fenString);
  std::string token;

  clearBitBoards();

  U64 boardPos = 56; // Fen string starts at a8 = index 56
  fenStream >> token;

  U64 one64 = ONE;
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

  WHITE_KING_HAS_MOVED = false, BLACK_KING_HAS_MOVED = false, WHITE_KS_ROOK_HAS_MOVED = false,
    BLACK_KS_ROOK_HAS_MOVED = false, WHITE_QS_ROOK_HAS_MOVED = false, BLACK_QS_ROOK_HAS_MOVED = false;

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

  updateNonPieceBitBoards();
}

U64 Board::getOccupied() {
  return getWhitePieces() | getBlackPieces();
}

U64* Board::getWhiteBitBoard(int squareIndex) {
  U64 square = ONE << squareIndex;

  U64* pieces;
  if (square & WHITE_PAWNS) pieces = &WHITE_PAWNS;
  else if (square & WHITE_ROOKS) pieces = &WHITE_ROOKS;
  else if (square & WHITE_KNIGHTS) pieces = &WHITE_KNIGHTS;
  else if (square & WHITE_BISHOPS) pieces = &WHITE_BISHOPS;
  else if (square & WHITE_KING) pieces = &WHITE_KING;
  else if (square & WHITE_QUEENS) pieces = &WHITE_QUEENS;

  return pieces;
}

void Board::updateNonPieceBitBoards() {
  WHITE_PIECES = getWhitePieces();
  WHITE_ATTACKABLE = WHITE_PIECES & ~WHITE_KING;

  BLACK_PIECES = getBlackPieces();
  BLACK_ATTACKABLE = BLACK_PIECES & ~BLACK_KING;

  OCCUPIED = getOccupied();
  NOT_OCCUPIED = ~OCCUPIED;

  WHITE_ATTACKS = getWhiteAttacks();
  BLACK_ATTACKS = getBlackAttacks();
}

U64* Board::getBlackBitBoard(int squareIndex) {
  U64 square = ONE << squareIndex;

  U64* pieces;
  if (square & BLACK_PAWNS) pieces = &BLACK_PAWNS;
  else if (square & BLACK_ROOKS) pieces = &BLACK_ROOKS;
  else if (square & BLACK_KNIGHTS) pieces = &BLACK_KNIGHTS;
  else if (square & BLACK_BISHOPS) pieces = &BLACK_BISHOPS;
  else if (square & BLACK_KING) pieces = &BLACK_KING;
  else if (square & BLACK_QUEENS) pieces = &BLACK_QUEENS;

  return pieces;
}

void Board::doRegularMove(CMove move) {
  U64* pieces = WHITE_TO_MOVE ? getWhiteBitBoard(move.getFrom()) : getBlackBitBoard(move.getFrom());

  U64 fromSquare = ONE << move.getFrom();
  U64 toSquare = ONE << move.getTo();

  *pieces ^= fromSquare;
  *pieces ^= toSquare;
}

void Board::doMove(CMove move) {
  doRegularMove(move);

  // Handle special moves
  unsigned int flags = move.getFlags();
  if (flags & CMove::CAPTURE) {
    U64* capturePieces = WHITE_TO_MOVE ? getBlackBitBoard(move.getTo()) : getWhiteBitBoard(move.getTo());
    *capturePieces ^= (ONE << move.getTo());
  }
  if (flags & CMove::KSIDE_CASTLE) {
    if (WHITE_TO_MOVE) {
      WHITE_ROOKS ^= ((ONE << h1) | (ONE << f1));
    } else {
      BLACK_ROOKS ^= ((ONE << h8) | (ONE << f8));
    }
  }
  if (flags & CMove::QSIDE_CASTLE) {
    // King has already been moved, kingside rook must be moved
    if (WHITE_TO_MOVE) {
      WHITE_ROOKS ^= ((ONE << a1) | (ONE << d1));
    } else {
      BLACK_ROOKS ^= ((ONE << a8) | (ONE << d8));
    }
  }
  if (flags & CMove::EN_PASSANT) {
    if (WHITE_TO_MOVE) {
      BLACK_PAWNS ^= (EN_PASSANT >> 8);
    } else {
      WHITE_PAWNS ^= (EN_PASSANT << 8);
    }
  }

  if (flags & CMove::DOUBLE_PAWN_PUSH) {
    // Set square behind as EN_PASSANT
    unsigned int enPasIndex = WHITE_TO_MOVE ? move.getTo() - 8 : move.getTo() + 8;
    EN_PASSANT = ONE << enPasIndex;
  } else {
    EN_PASSANT =ZERO;
  }

  // Handle promotions
  bool isPromotion = flags & (CMove::QUEEN_PROMOTION | CMove::ROOK_PROMOTION | CMove::BISHOP_PROMOTION | CMove::KNIGHT_PROMOTION);
  if (isPromotion) {
    // Add promoted piece
    if (flags & CMove::QUEEN_PROMOTION) {
      U64* queensToUpdate = WHITE_TO_MOVE ? &WHITE_QUEENS : &BLACK_QUEENS;
      *queensToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::ROOK_PROMOTION) {
      U64* rooksToUpdate = WHITE_TO_MOVE ? &WHITE_ROOKS : &BLACK_ROOKS;
      *rooksToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::BISHOP_PROMOTION) {
      U64* bishopsToUpdate = WHITE_TO_MOVE ? &WHITE_BISHOPS : &BLACK_BISHOPS;
      *bishopsToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::KNIGHT_PROMOTION) {
      U64* knightsToUpdate = WHITE_TO_MOVE ? &WHITE_KNIGHTS : &BLACK_KNIGHTS;
      *knightsToUpdate ^= (ONE << move.getTo());
    }

    // Remove promoted pawn
    if (WHITE_TO_MOVE) {
      WHITE_PAWNS ^= ONE << move.getTo();
    } else {
      BLACK_PAWNS ^= ONE << move.getTo();
    }
  }


  // Update castling flags if rooks or kings have moved
  switch(move.getFrom()) {
    case e1: WHITE_KING_HAS_MOVED = true;
      break;
    case e8: BLACK_KING_HAS_MOVED = true;
      break;
    case a1: WHITE_QS_ROOK_HAS_MOVED = true;
      break;
    case h1: WHITE_KS_ROOK_HAS_MOVED = true;
      break;
    case a8: BLACK_QS_ROOK_HAS_MOVED = true;
      break;
    case h8: BLACK_KS_ROOK_HAS_MOVED = true;
      break;
  }

  WHITE_TO_MOVE = !WHITE_TO_MOVE;
  updateNonPieceBitBoards();
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
  U64 fromSquare = ONE << square;

  U64 attacks = ((fromSquare << 7) & ~FILE_H) | ((fromSquare << 9) & ~FILE_A);

  return attacks;
}

U64 Board::getBlackPawnAttacksForSquare(int square) {
  U64 fromSquare = ONE << square;

  U64 attacks = ((fromSquare >> 7) & ~FILE_A) | ((fromSquare >> 9) & ~FILE_H);

  return attacks;
}


U64 Board::getKnightAttacksForSquare(int square, U64 own) {
  U64 fromSquare = ONE << square;

  U64 moves = (((fromSquare << 15) | (fromSquare >> 17)) & ~FILE_H) | // Left 1
    (((fromSquare >> 15) | (fromSquare << 17)) & ~FILE_A) | // Right 1
    (((fromSquare << 6) | (fromSquare >> 10)) & ~(FILE_G | FILE_H)) | // Left 2
    (((fromSquare >> 6) | (fromSquare << 10)) & ~(FILE_A | FILE_B)); // Right 2

  return moves & (~own);
}

U64 Board::getKingAttacksForSquare(int square, U64 own) {
  U64 king = ONE << square;

  U64 moves = (((king << 7) | (king >> 9) | (king >> 1)) & (~FILE_H)) |
    (((king << 9) | (king >> 7) | (king << 1)) & (~FILE_A)) |
    ((king >> 8) | (king << 8));

  return moves & (~own);
}

U64 Board::getBishopAttacksForSquare(int square, U64 own) {
  U64 moves = raytable.getPositiveAttacks(RayTable::NORTH_WEST, square, OCCUPIED) |
    raytable.getPositiveAttacks(RayTable::NORTH_EAST, square, OCCUPIED) |
    raytable.getNegativeAttacks(RayTable::SOUTH_WEST, square, OCCUPIED) |
    raytable.getNegativeAttacks(RayTable::SOUTH_EAST, square, OCCUPIED);

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
    U64 square = ONE << squareIndex;
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
    U64 square = ONE << squareIndex;
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
