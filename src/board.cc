#include "board.h"
#include "cmove.h"
#include "raytable.h"
#include "defs.h"
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>

RayTable Board::_raytable = RayTable();

Board::Board() {
  setToFen("8/8/8/8/8/8/8/8 w - -");
}

U64 Board::getWhitePieces(PieceType piece) {
  return _whitePieces[piece];
}

U64 Board::getBlackPieces(PieceType piece) {
  return _blackPieces[piece];
}

U64 Board::getAllWhitePieces() {
  return _allWhitePieces;
}

U64 Board::getAllBlackPieces() {
  return _allBlackPieces;
}

U64 Board::getWhiteAttackable() {
  return _whiteAttackable;
}

U64 Board::getBlackAttackable() {
  return _blackAttackable;
}

U64 Board::getWhiteAttacks() {
  return _whiteAttacks;
}

U64 Board::getBlackAttacks() {
  return _blackAttacks;
}

U64 Board::getOccupied() {
  return _occupied;
}

U64 Board::getNotOccupied() {
  return _notOccupied;
}

U64 Board::getEnPassant() {
  return _enPassant;
}

bool Board::whiteToMove() {
  return _whiteToMove;
}

bool Board::whiteIsInCheck() {
  return (bool) (_blackAttacks & _whitePieces[KING]);
}

bool Board::blackIsInCheck() {
  return (bool) (_whiteAttacks & _blackPieces[KING]);
}

bool Board::whiteCanCastleKs() {
  if (!_whiteCanCastleKs) {
    return false;
  }

  U64 passThroughSquares = (ONE << f1) | (ONE << g1);
  bool squaresOccupied = passThroughSquares & _occupied;
  bool squaresAttacked = passThroughSquares & _blackAttacks;

  return !whiteIsInCheck() && !squaresOccupied && !squaresAttacked;
}

bool Board::whiteCanCastleQs() {
  if (!_whiteCanCastleQs) {
    return false;
  }

  U64 inbetweenSquares = (ONE << b1) | (ONE << c1) | (ONE << d1);
  U64 passThroughSquares = (ONE << c1) | (ONE << d1);
  bool squaresOccupied = inbetweenSquares & _occupied;
  bool squaresAttacked = passThroughSquares & _blackAttacks;

  return !whiteIsInCheck() && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleKs() {
  if (!_blackCanCastleKs) {
    return false;
  }

  U64 passThroughSquares = (ONE << f8) | (ONE << g8);
  bool squaresOccupied = passThroughSquares & _occupied;
  bool squaresAttacked = passThroughSquares & _whiteAttacks;

  return !blackIsInCheck() && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleQs() {
  if (!_blackCanCastleQs) {
    return false;
  }

  U64 inbetweenSquares = (ONE << b8) | (ONE << c8) | (ONE << d8);
  U64 passThroughSquares = (ONE << c8) | (ONE << d8);
  bool squaresOccupied = inbetweenSquares & _occupied;
  bool squaresAttacked = passThroughSquares & _whiteAttacks;

  return !blackIsInCheck() && !squaresOccupied && !squaresAttacked;
}

std::string Board::getStringRep() {
  std::string stringRep;
  U64 base = 1;
  U64 boardPos = 56; // Starts at a8, goes down rank by rank
  int squaresProcessed = 0;

  while (squaresProcessed < 64) {
    U64 square = base << boardPos;
    bool squareOccupied = (square & _occupied) != 0;

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

void Board::_clearBitBoards() {
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

  _allWhitePieces = ZERO;
  _allBlackPieces = ZERO;

  _whiteAttackable = ZERO;
  _blackAttackable = ZERO;

  _enPassant = ZERO;

  _occupied = ZERO;
  _notOccupied = ZERO;

  return;
}

void Board::setToFen(std::string fenString) {
  std::istringstream fenStream(fenString);
  std::string token;

  _clearBitBoards();

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
  _whiteToMove = token == "w";

  // Castling availability
  fenStream >> token;

  _whiteCanCastleKs = false, _whiteCanCastleQs = false,
    _blackCanCastleKs = false, _blackCanCastleQs = false;
  for (auto currChar : token) {
    switch(currChar) {
      case 'K':
        _whiteCanCastleKs = true;
        break;
      case 'Q':
        _whiteCanCastleQs = true;
        break;
      case 'k':
        _blackCanCastleKs = true;
        break;
      case 'q':
        _blackCanCastleQs = true;
        break;
    }
  }

  // Get en passant target square
  std::string enPasSquare;
  fenStream >> enPasSquare;

  // Set bitboards
  if (enPasSquare == "-") {
    _enPassant = 0;
  } else {
    int enPasIndex = CMove::notationToIndex(enPasSquare);
    _enPassant = static_cast<U64>(1) << enPasIndex;
  }

  _updateNonPieceBitBoards();
}

U64* Board::_getWhiteBitBoard(int squareIndex) {
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

void Board::_updateNonPieceBitBoards() {
  _allWhitePieces = _whitePieces[PAWN] | \
    _whitePieces[ROOK] | \
    _whitePieces[KNIGHT] | \
    _whitePieces[BISHOP] | \
    _whitePieces[QUEEN] | \
    _whitePieces[KING];
  _whiteAttackable = _allWhitePieces & ~_whitePieces[KING];

  _allBlackPieces = _blackPieces[PAWN] | \
    _blackPieces[ROOK] | \
    _blackPieces[KNIGHT] | \
    _blackPieces[BISHOP] | \
    _blackPieces[QUEEN] | \
    _blackPieces[KING];
  _blackAttackable = _allBlackPieces & ~_blackPieces[KING];

  _occupied = _allWhitePieces | _allBlackPieces;
  _notOccupied = ~_occupied;

  _whiteAttacks = _genWhiteAttacks();
  _blackAttacks = _genBlackAttacks();
}

U64* Board::_getBlackBitBoard(int squareIndex) {
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

void Board::_doRegularMove(CMove move) {
  U64* pieces = _whiteToMove ? &_whitePieces[move.getPieceType()] : &_blackPieces[move.getPieceType()];

  U64 fromSquare = ONE << move.getFrom();
  U64 toSquare = ONE << move.getTo();

  *pieces ^= fromSquare;
  *pieces ^= toSquare;
}

void Board::doMove(CMove move) {
  _doRegularMove(move);

  // Handle special moves
  unsigned int flags = move.getFlags();
  if (flags & CMove::CAPTURE) {
    U64* capturePieces = _whiteToMove ? _getBlackBitBoard(move.getTo()) : _getWhiteBitBoard(move.getTo());
    *capturePieces ^= (ONE << move.getTo());

    switch(move.getTo()) {
      case a1: _whiteCanCastleQs = false;
        break;
      case h1: _whiteCanCastleKs = false;
        break;
      case a8: _blackCanCastleQs = false;
        break;
      case h8: _blackCanCastleKs = false;
        break;
    }
  }
  if (flags & CMove::KSIDE_CASTLE) {
    if (_whiteToMove) {
      _whitePieces[ROOK] ^= ((ONE << h1) | (ONE << f1));
    } else {
      _blackPieces[ROOK] ^= ((ONE << h8) | (ONE << f8));
    }
  }
  if (flags & CMove::QSIDE_CASTLE) {
    // King has already been moved, kingside rook must be moved
    if (_whiteToMove) {
      _whitePieces[ROOK] ^= ((ONE << a1) | (ONE << d1));
    } else {
      _blackPieces[ROOK] ^= ((ONE << a8) | (ONE << d8));
    }
  }
  if (flags & CMove::EN_PASSANT) {
    if (_whiteToMove) {
      _blackPieces[PAWN] ^= (_enPassant >> 8);
    } else {
      _whitePieces[PAWN] ^= (_enPassant << 8);
    }
  }

  if (flags & CMove::DOUBLE_PAWN_PUSH) {
    // Set square behind as _enPassant
    unsigned int enPasIndex = _whiteToMove ? move.getTo() - 8 : move.getTo() + 8;
    _enPassant = ONE << enPasIndex;
  } else {
    _enPassant =ZERO;
  }

  // Handle promotions
  bool isPromotion = flags & (CMove::QUEEN_PROMOTION | CMove::ROOK_PROMOTION | CMove::BISHOP_PROMOTION | CMove::KNIGHT_PROMOTION);
  if (isPromotion) {
    // Add promoted piece
    if (flags & CMove::QUEEN_PROMOTION) {
      U64* queensToUpdate = _whiteToMove ? &_whitePieces[QUEEN] : &_blackPieces[QUEEN];
      *queensToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::ROOK_PROMOTION) {
      U64* rooksToUpdate = _whiteToMove ? &_whitePieces[ROOK] : &_blackPieces[ROOK];
      *rooksToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::BISHOP_PROMOTION) {
      U64* bishopsToUpdate = _whiteToMove ? &_whitePieces[BISHOP] : &_blackPieces[BISHOP];
      *bishopsToUpdate ^= (ONE << move.getTo());
    } else if (flags & CMove::KNIGHT_PROMOTION) {
      U64* knightsToUpdate = _whiteToMove ? &_whitePieces[KNIGHT] : &_blackPieces[KNIGHT];
      *knightsToUpdate ^= (ONE << move.getTo());
    }

    // Remove promoted pawn
    if (_whiteToMove) {
      _whitePieces[PAWN] ^= ONE << move.getTo();
    } else {
      _blackPieces[PAWN] ^= ONE << move.getTo();
    }
  }


  // Update castling flags if rooks or kings have moved
  switch(move.getFrom()) {
    case e1:
      _whiteCanCastleKs = false;
      _whiteCanCastleQs = false;
      break;
    case e8:
      _blackCanCastleKs = false;
      _blackCanCastleQs = false;
      break;
    case a1: _whiteCanCastleQs = false;
      break;
    case h1: _whiteCanCastleKs = false;
      break;
    case a8: _blackCanCastleQs = false;
      break;
    case h8: _blackCanCastleKs = false;
      break;
  }

  _whiteToMove = !_whiteToMove;
  _updateNonPieceBitBoards();
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
  U64 moves = _raytable.getPositiveAttacks(RayTable::NORTH_WEST, square, _occupied) |
    _raytable.getPositiveAttacks(RayTable::NORTH_EAST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH_WEST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH_EAST, square, _occupied);

  return moves & (~own);
}

U64 Board::getRookAttacksForSquare(int square, U64 own) {
  U64 moves = _raytable.getPositiveAttacks(RayTable::NORTH, square, _occupied) |
    _raytable.getPositiveAttacks(RayTable::EAST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::WEST, square, _occupied);

  return moves & (~own);
}

U64 Board::getQueenAttacksForSquare(int square, U64 own) {
  return getBishopAttacksForSquare(square, own) | getRookAttacksForSquare(square, own);
}

U64 Board::_genWhiteAttacks() {
  U64 attacks = U64(0);

  for(int squareIndex=0;squareIndex<64;squareIndex++) {
    U64 square = ONE << squareIndex;
    if ((square & _allWhitePieces) == 0) {
      continue;
    }

    if (square & _whitePieces[PAWN]) attacks |= getWhitePawnAttacksForSquare(squareIndex);
    else if (square & _whitePieces[ROOK]) attacks |= getRookAttacksForSquare(squareIndex, _allWhitePieces);
    else if (square & _whitePieces[KNIGHT]) attacks |= getKnightAttacksForSquare(squareIndex, _allWhitePieces);
    else if (square & _whitePieces[BISHOP]) attacks |= getBishopAttacksForSquare(squareIndex, _allWhitePieces);
    else if (square & _whitePieces[KING]) attacks |= getKingAttacksForSquare(squareIndex, _allWhitePieces);
    else if (square & _whitePieces[QUEEN]) attacks |= getQueenAttacksForSquare(squareIndex, _allWhitePieces);
  }
  attacks |= _enPassant;

  return attacks;
}

U64 Board::_genBlackAttacks() {
  U64 attacks = U64(0);

  for(int squareIndex=0;squareIndex<64;squareIndex++) {
    U64 square = ONE << squareIndex;
    if ((square & _allBlackPieces) == 0) {
      continue;
    }

    if (square & _blackPieces[PAWN]) attacks |= getBlackPawnAttacksForSquare(squareIndex);
    else if (square & _blackPieces[ROOK]) attacks |= getRookAttacksForSquare(squareIndex, _allBlackPieces);
    else if (square & _blackPieces[KNIGHT]) attacks |= getKnightAttacksForSquare(squareIndex, _allBlackPieces);
    else if (square & _blackPieces[BISHOP]) attacks |= getBishopAttacksForSquare(squareIndex, _allBlackPieces);
    else if (square & _blackPieces[KING]) attacks |= getKingAttacksForSquare(squareIndex, _allBlackPieces);
    else if (square & _blackPieces[QUEEN]) attacks |= getQueenAttacksForSquare(squareIndex, _allBlackPieces);
  }
  attacks |= _enPassant;

  return attacks;
}
