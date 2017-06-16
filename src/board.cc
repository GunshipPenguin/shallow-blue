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

U64 Board::getWhiteBitBoard(PieceType piece) {
  return _whitePieces[piece];
}

U64 Board::getBlackBitBoard(PieceType piece) {
  return _blackPieces[piece];
}

bool Board::whiteIsInCheck() {
  return (bool) (BLACK_ATTACKS & _whitePieces[KING]);
}

bool Board::blackIsInCheck() {
  return (bool) (WHITE_ATTACKS & _blackPieces[KING]);
}

bool Board::whiteCanCastleKs() {
  if (!WHITE_CAN_CASTLE_KS) {
    return false;
  }

  U64 passThroughSquares = (ONE << f1) | (ONE << g1);
  bool squaresOccupied = passThroughSquares & OCCUPIED;
  bool squaresAttacked = passThroughSquares & BLACK_ATTACKS;

  return !whiteIsInCheck() && !squaresOccupied && !squaresAttacked;
}

bool Board::whiteCanCastleQs() {
  if (!WHITE_CAN_CASTLE_QS) {
    return false;
  }

  U64 inbetweenSquares = (ONE << b1) | (ONE << c1) | (ONE << d1);
  U64 passThroughSquares = (ONE << c1) | (ONE << d1);
  bool squaresOccupied = inbetweenSquares & OCCUPIED;
  bool squaresAttacked = passThroughSquares & BLACK_ATTACKS;

  return !whiteIsInCheck() && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleKs() {
  if (!BLACK_CAN_CASTLE_KS) {
    return false;
  }

  U64 passThroughSquares = (ONE << f8) | (ONE << g8);
  bool squaresOccupied = passThroughSquares & OCCUPIED;
  bool squaresAttacked = passThroughSquares & WHITE_ATTACKS;

  return !blackIsInCheck() && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleQs() {
  if (!BLACK_CAN_CASTLE_QS) {
    return false;
  }

  U64 inbetweenSquares = (ONE << b8) | (ONE << c8) | (ONE << d8);
  U64 passThroughSquares = (ONE << c8) | (ONE << d8);
  bool squaresOccupied = inbetweenSquares & OCCUPIED;
  bool squaresAttacked = passThroughSquares & WHITE_ATTACKS;

  return !blackIsInCheck() && !squaresOccupied && !squaresAttacked;
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
      if (square & _whitePieces[PAWN]) stringRep += " P ";
      else if (square & _blackPieces[PAWN]) stringRep += " p ";

      else if (square & _whitePieces[ROOK]) stringRep += " R ";
      else if (square & _blackPieces[ROOK]) stringRep += " r ";

      else if (square & _whitePieces[KNIGHT]) stringRep += " N ";
      else if (square & _blackPieces[KNIGHT]) stringRep += " n ";

      else if (square & _whitePieces[BISHOP]) stringRep += " B ";
      else if (square & _blackPieces[BISHOP]) stringRep += " b ";

      else if (square & _whitePieces[QUEEN]) stringRep += " Q ";
      else if (square & _blackPieces[QUEEN]) stringRep += " q ";

      else if (square & _whitePieces[KING]) stringRep += " K ";
      else if (square & _blackPieces[KING]) stringRep += " k ";
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
  _whitePieces[PAWN] = ZERO;
  _blackPieces[PAWN] = ZERO;

  _whitePieces[ROOK] = ZERO;
  _blackPieces[ROOK] = ZERO;

  _whitePieces[KNIGHT] = ZERO;
  _blackPieces[KNIGHT] = ZERO;

  _whitePieces[BISHOP] = ZERO;
  _blackPieces[BISHOP] = ZERO;

  _whitePieces[QUEEN] = ZERO;
  _blackPieces[QUEEN] = ZERO;

  _whitePieces[KING] = ZERO;
  _blackPieces[KING] = ZERO;

  WHITE_PIECES = ZERO;
  BLACK_PIECES = ZERO;

  WHITE_ATTACKABLE = ZERO;
  BLACK_ATTACKABLE = ZERO;

  EN_PASSANT = ZERO;

  OCCUPIED = ZERO;
  NOT_OCCUPIED = ZERO;

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
      case 'p': _blackPieces[PAWN] |= (one64 << boardPos++);
        break;
      case 'r': _blackPieces[ROOK] |= (one64 << boardPos++);
        break;
      case 'n': _blackPieces[KNIGHT] |= (one64 << boardPos++);
        break;
      case 'b': _blackPieces[BISHOP] |= (one64 << boardPos++);
        break;
      case 'q': _blackPieces[QUEEN] |= (one64 << boardPos++);
        break;
      case 'k': _blackPieces[KING] |= (one64 << boardPos++);
        break;
      case 'P': _whitePieces[PAWN] |= (one64 << boardPos++);
        break;
      case 'R': _whitePieces[ROOK] |= (one64 << boardPos++);
        break;
      case 'N': _whitePieces[KNIGHT] |= (one64 << boardPos++);
        break;
      case 'B': _whitePieces[BISHOP] |= (one64 << boardPos++);
        break;
      case 'Q': _whitePieces[QUEEN] |= (one64 << boardPos++);
        break;
      case 'K': _whitePieces[KING] |= (one64 << boardPos++);
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

  updateNonPieceBitBoards();
}

U64 Board::getOccupied() {
  return getWhitePieces() | getBlackPieces();
}

U64* Board::getWhiteBitBoard(int squareIndex) {
  U64 square = ONE << squareIndex;

  U64* pieces;
  if (square & _whitePieces[PAWN]) pieces = &_whitePieces[PAWN];
  else if (square & _whitePieces[ROOK]) pieces = &_whitePieces[ROOK];
  else if (square & _whitePieces[KNIGHT]) pieces = &_whitePieces[KNIGHT];
  else if (square & _whitePieces[BISHOP]) pieces = &_whitePieces[BISHOP];
  else if (square & _whitePieces[KING]) pieces = &_whitePieces[KING];
  else if (square & _whitePieces[QUEEN]) pieces = &_whitePieces[QUEEN];

  return pieces;
}

void Board::updateNonPieceBitBoards() {
  WHITE_PIECES = getWhitePieces();
  WHITE_ATTACKABLE = WHITE_PIECES & ~_whitePieces[KING];

  BLACK_PIECES = getBlackPieces();
  BLACK_ATTACKABLE = BLACK_PIECES & ~_blackPieces[KING];

  OCCUPIED = getOccupied();
  NOT_OCCUPIED = ~OCCUPIED;

  WHITE_ATTACKS = getWhiteAttacks();
  BLACK_ATTACKS = getBlackAttacks();
}

U64* Board::getBlackBitBoard(int squareIndex) {
  U64 square = ONE << squareIndex;

  U64* pieces;
  if (square & _blackPieces[PAWN]) pieces = &_blackPieces[PAWN];
  else if (square & _blackPieces[ROOK]) pieces = &_blackPieces[ROOK];
  else if (square & _blackPieces[KNIGHT]) pieces = &_blackPieces[KNIGHT];
  else if (square & _blackPieces[BISHOP]) pieces = &_blackPieces[BISHOP];
  else if (square & _blackPieces[KING]) pieces = &_blackPieces[KING];
  else if (square & _blackPieces[QUEEN]) pieces = &_blackPieces[QUEEN];

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

    switch(move.getTo()) {
      case a1: WHITE_CAN_CASTLE_QS = false;
        break;
      case h1: WHITE_CAN_CASTLE_KS = false;
        break;
      case a8: BLACK_CAN_CASTLE_QS = false;
        break;
      case h8: BLACK_CAN_CASTLE_KS = false;
        break;
    }
  }
  if (flags & CMove::KSIDE_CASTLE) {
    if (WHITE_TO_MOVE) {
      _whitePieces[ROOK] ^= ((ONE << h1) | (ONE << f1));
    } else {
      _blackPieces[ROOK] ^= ((ONE << h8) | (ONE << f8));
    }
  }
  if (flags & CMove::QSIDE_CASTLE) {
    // King has already been moved, kingside rook must be moved
    if (WHITE_TO_MOVE) {
      _whitePieces[ROOK] ^= ((ONE << a1) | (ONE << d1));
    } else {
      _blackPieces[ROOK] ^= ((ONE << a8) | (ONE << d8));
    }
  }
  if (flags & CMove::EN_PASSANT) {
    if (WHITE_TO_MOVE) {
      _blackPieces[PAWN] ^= (EN_PASSANT >> 8);
    } else {
      _whitePieces[PAWN] ^= (EN_PASSANT << 8);
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
      U64* queensToUpdate = WHITE_TO_MOVE ? &_whitePieces[QUEEN] : &_blackPieces[QUEEN];
      *queensToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::ROOK_PROMOTION) {
      U64* rooksToUpdate = WHITE_TO_MOVE ? &_whitePieces[ROOK] : &_blackPieces[ROOK];
      *rooksToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::BISHOP_PROMOTION) {
      U64* bishopsToUpdate = WHITE_TO_MOVE ? &_whitePieces[BISHOP] : &_blackPieces[BISHOP];
      *bishopsToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::KNIGHT_PROMOTION) {
      U64* knightsToUpdate = WHITE_TO_MOVE ? &_whitePieces[KNIGHT] : &_blackPieces[KNIGHT];
      *knightsToUpdate ^= (ONE << move.getTo());
    }

    // Remove promoted pawn
    if (WHITE_TO_MOVE) {
      _whitePieces[PAWN] ^= ONE << move.getTo();
    } else {
      _blackPieces[PAWN] ^= ONE << move.getTo();
    }
  }


  // Update castling flags if rooks or kings have moved
  switch(move.getFrom()) {
    case e1:
      WHITE_CAN_CASTLE_KS = false;
      WHITE_CAN_CASTLE_QS = false;
      break;
    case e8:
      BLACK_CAN_CASTLE_KS = false;
      BLACK_CAN_CASTLE_QS = false;
      break;
    case a1: WHITE_CAN_CASTLE_QS = false;
      break;
    case h1: WHITE_CAN_CASTLE_KS = false;
      break;
    case a8: BLACK_CAN_CASTLE_QS = false;
      break;
    case h8: BLACK_CAN_CASTLE_KS = false;
      break;
  }

  WHITE_TO_MOVE = !WHITE_TO_MOVE;
  updateNonPieceBitBoards();
}

U64 Board::getWhitePieces() {
  return _whitePieces[PAWN] | \
  _whitePieces[ROOK] | \
  _whitePieces[KNIGHT] | \
  _whitePieces[BISHOP] | \
  _whitePieces[QUEEN] | \
  _whitePieces[KING];
}

U64 Board::getBlackPieces() {
  return _blackPieces[PAWN] | \
  _blackPieces[ROOK] | \
  _blackPieces[KNIGHT] | \
  _blackPieces[BISHOP] | \
  _blackPieces[QUEEN] | \
  _blackPieces[KING];
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

    if (square & _whitePieces[PAWN]) attacks |= getWhitePawnAttacksForSquare(squareIndex);
    else if (square & _whitePieces[ROOK]) attacks |= getRookAttacksForSquare(squareIndex, WHITE_PIECES);
    else if (square & _whitePieces[KNIGHT]) attacks |= getKnightAttacksForSquare(squareIndex, WHITE_PIECES);
    else if (square & _whitePieces[BISHOP]) attacks |= getBishopAttacksForSquare(squareIndex, WHITE_PIECES);
    else if (square & _whitePieces[KING]) attacks |= getKingAttacksForSquare(squareIndex, WHITE_PIECES);
    else if (square & _whitePieces[QUEEN]) attacks |= getQueenAttacksForSquare(squareIndex, WHITE_PIECES);
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

    if (square & _blackPieces[PAWN]) attacks |= getBlackPawnAttacksForSquare(squareIndex);
    else if (square & _blackPieces[ROOK]) attacks |= getRookAttacksForSquare(squareIndex, BLACK_PIECES);
    else if (square & _blackPieces[KNIGHT]) attacks |= getKnightAttacksForSquare(squareIndex, BLACK_PIECES);
    else if (square & _blackPieces[BISHOP]) attacks |= getBishopAttacksForSquare(squareIndex, BLACK_PIECES);
    else if (square & _blackPieces[KING]) attacks |= getKingAttacksForSquare(squareIndex, BLACK_PIECES);
    else if (square & _blackPieces[QUEEN]) attacks |= getQueenAttacksForSquare(squareIndex, BLACK_PIECES);
  }
  attacks |= EN_PASSANT;

  return attacks;
}
