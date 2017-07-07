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

U64 Board::getPieces(Color color, PieceType pieceType) const {
  return _pieces[color][pieceType];
}

U64 Board::getAllPieces(Color color) const {
  return _allPieces[color];
}

U64 Board::getAttackable(Color color) const {
  return _attackable[color];
}

U64 Board::getOccupied() const {
  return _occupied;
}

U64 Board::getNotOccupied() const {
  return _notOccupied;
}

U64 Board::getEnPassant() const {
  return _enPassant;
}

Color Board::getActivePlayer() const {
  return _activePlayer;
}

U64 Board::getAttacksForSquare(PieceType pieceType, Color color, int square) const {
  // Special case for pawns
  if (pieceType == PAWN) {
    switch (color) {
      case WHITE: return _getWhitePawnAttacksForSquare(square);
      case BLACK: return _getBlackPawnAttacksForSquare(square);
    }
  }

  U64 own = getAllPieces(color);
  U64 attacks;
  switch (pieceType) {
    case ROOK: attacks = _getRookAttacksForSquare(square, own);
      break;
    case KNIGHT: attacks = _getKnightAttacksForSquare(square, own);
      break;
    case BISHOP: attacks = _getBishopAttacksForSquare(square, own);
      break;
    case QUEEN: attacks = _getQueenAttacksForSquare(square, own);
      break;
    case KING: attacks = _getKingAttacksForSquare(square, own);
      break;
    default:
      break;
  }

  return attacks;
}

Color Board::getInactivePlayer() const {
  return _activePlayer == WHITE ? BLACK : WHITE;
}

ZKey Board::getZKey() const {
  return _zKey;
}

PSquareTable Board::getPSquareTable() const {
  return _pst;
}

bool Board::colorIsInCheck(Color color) const {
  int kingSquare = __builtin_ffsll(getPieces(color, KING)) - 1;

  return _squareUnderAttack(getOppositeColor(color), kingSquare);
  if (_checksCalculated[color]) {
    return _checks[color];
  } else {
    int kingSquare = __builtin_ffsll(getPieces(color, KING)) - 1;

    bool isInCheck = _squareUnderAttack(getOppositeColor(color), kingSquare);

    _checks[color] = isInCheck;
    _checksCalculated[color] = true;

    return isInCheck;
  }
}

bool Board::whiteCanCastleKs() const {
  if (!_whiteCanCastleKs) {
    return false;
  }

  U64 passThroughSquares = (ONE << f1) | (ONE << g1);
  bool squaresOccupied = passThroughSquares & _occupied;
  bool squaresAttacked = _squareUnderAttack(BLACK, f1) || _squareUnderAttack(BLACK, g1);

  return !colorIsInCheck(WHITE) && !squaresOccupied && !squaresAttacked;
}

bool Board::whiteCanCastleQs() const {
  if (!_whiteCanCastleQs) {
    return false;
  }

  U64 inbetweenSquares = (ONE << c1) | (ONE << d1) | (ONE << b1);
  bool squaresOccupied = inbetweenSquares & _occupied;
  bool squaresAttacked = _squareUnderAttack(BLACK, d1) || _squareUnderAttack(BLACK, c1);

  return !colorIsInCheck(WHITE) && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleKs() const {
  if (!_blackCanCastleKs) {
    return false;
  }

  U64 passThroughSquares = (ONE << f8) | (ONE << g8);
  bool squaresOccupied = passThroughSquares & _occupied;
  bool squaresAttacked = _squareUnderAttack(WHITE, f8) || _squareUnderAttack(WHITE, g8);

  return !colorIsInCheck(BLACK) && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleQs() const {
  if (!_blackCanCastleQs) {
    return false;
  }

  U64 inbetweenSquares = (ONE << b8) | (ONE << c8) | (ONE << d8);
  bool squaresOccupied = inbetweenSquares & _occupied;
  bool squaresAttacked = _squareUnderAttack(WHITE, c8) || _squareUnderAttack(WHITE, d8);

  return !colorIsInCheck(BLACK) && !squaresOccupied && !squaresAttacked;
}

bool Board::whiteKsCastlingRight() const {
  return _whiteCanCastleKs;
}

bool Board::whiteQsCastlingRight() const {
  return _whiteCanCastleQs;
}

bool Board::blackKsCastlingRight() const {
  return _blackCanCastleKs;
}

bool Board::blackQsCastlingRight() const {
  return _blackCanCastleQs;
}

std::string Board::getStringRep() const {
  std::string stringRep;
  U64 base = 1;
  U64 boardPos = 56; // Starts at a8, goes down rank by rank
  int squaresProcessed = 0;

  while (squaresProcessed < 64) {
    U64 square = base << boardPos;
    bool squareOccupied = (square & _occupied) != 0;

    if (squareOccupied) {
      if (square & _pieces[WHITE][PAWN]) stringRep += " P ";
      else if (square & _pieces[BLACK][PAWN]) stringRep += " p ";

      else if (square & _pieces[WHITE][ROOK]) stringRep += " R ";
      else if (square & _pieces[BLACK][ROOK]) stringRep += " r ";

      else if (square & _pieces[WHITE][KNIGHT]) stringRep += " N ";
      else if (square & _pieces[BLACK][KNIGHT]) stringRep += " n ";

      else if (square & _pieces[WHITE][BISHOP]) stringRep += " B ";
      else if (square & _pieces[BLACK][BISHOP]) stringRep += " b ";

      else if (square & _pieces[WHITE][QUEEN]) stringRep += " Q ";
      else if (square & _pieces[BLACK][QUEEN]) stringRep += " q ";

      else if (square & _pieces[WHITE][KING]) stringRep += " K ";
      else if (square & _pieces[BLACK][KING]) stringRep += " k ";
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
  _pieces[WHITE][PAWN] = ZERO;
  _pieces[BLACK][PAWN] = ZERO;

  _pieces[WHITE][ROOK] = ZERO;
  _pieces[BLACK][ROOK] = ZERO;

  _pieces[WHITE][KNIGHT] = ZERO;
  _pieces[BLACK][KNIGHT] = ZERO;

  _pieces[WHITE][BISHOP] = ZERO;
  _pieces[BLACK][BISHOP] = ZERO;

  _pieces[WHITE][QUEEN] = ZERO;
  _pieces[BLACK][QUEEN] = ZERO;

  _pieces[WHITE][KING] = ZERO;
  _pieces[BLACK][KING] = ZERO;

  _allPieces[WHITE] = ZERO;
  _allPieces[BLACK] = ZERO;

  _attackable[WHITE] = ZERO;
  _attackable[BLACK] = ZERO;

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
      case 'p': _pieces[BLACK][PAWN] |= (one64 << boardPos++);
        break;
      case 'r': _pieces[BLACK][ROOK] |= (one64 << boardPos++);
        break;
      case 'n': _pieces[BLACK][KNIGHT] |= (one64 << boardPos++);
        break;
      case 'b': _pieces[BLACK][BISHOP] |= (one64 << boardPos++);
        break;
      case 'q': _pieces[BLACK][QUEEN] |= (one64 << boardPos++);
        break;
      case 'k': _pieces[BLACK][KING] |= (one64 << boardPos++);
        break;
      case 'P': _pieces[WHITE][PAWN] |= (one64 << boardPos++);
        break;
      case 'R': _pieces[WHITE][ROOK] |= (one64 << boardPos++);
        break;
      case 'N': _pieces[WHITE][KNIGHT] |= (one64 << boardPos++);
        break;
      case 'B': _pieces[WHITE][BISHOP] |= (one64 << boardPos++);
        break;
      case 'Q': _pieces[WHITE][QUEEN] |= (one64 << boardPos++);
        break;
      case 'K': _pieces[WHITE][KING] |= (one64 << boardPos++);
        break;
      case '/': boardPos -= 16; // Go down one rank
        break;
      default:
        boardPos += static_cast<U64>(currChar - '0');
    }
  }

  // Next to move
  fenStream >> token;
  _activePlayer = token == "w" ? WHITE : BLACK;

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

  _resetChecks();

  _updateNonPieceBitBoards();
  _zKey = ZKey(*this);
  _pst = PSquareTable(*this);
}

void Board::_updateNonPieceBitBoards() {
  _allPieces[WHITE] = _pieces[WHITE][PAWN] | \
    _pieces[WHITE][ROOK] | \
    _pieces[WHITE][KNIGHT] | \
    _pieces[WHITE][BISHOP] | \
    _pieces[WHITE][QUEEN] | \
    _pieces[WHITE][KING];
  _attackable[WHITE] = _allPieces[WHITE] & ~_pieces[WHITE][KING];

  _allPieces[BLACK] = _pieces[BLACK][PAWN] | \
    _pieces[BLACK][ROOK] | \
    _pieces[BLACK][KNIGHT] | \
    _pieces[BLACK][BISHOP] | \
    _pieces[BLACK][QUEEN] | \
    _pieces[BLACK][KING];
  _attackable[BLACK] = _allPieces[BLACK] & ~_pieces[BLACK][KING];

  _occupied = _allPieces[WHITE] | _allPieces[BLACK];
  _notOccupied = ~_occupied;
}

PieceType Board::getPieceAtSquare(Color color, int squareIndex) const {
  U64 square = ONE << squareIndex;

  PieceType piece;
  if (square & _pieces[color][PAWN]) piece = PAWN;
  else if (square & _pieces[color][ROOK]) piece = ROOK;
  else if (square & _pieces[color][KNIGHT]) piece = KNIGHT;
  else if (square & _pieces[color][BISHOP]) piece = BISHOP;
  else if (square & _pieces[color][KING]) piece = KING;
  else if (square & _pieces[color][QUEEN]) piece = QUEEN;

  return piece;
}

void Board::_movePiece(Color color, PieceType pieceType, int from, int to) {
  _removePiece(color, pieceType, from);
  _addPiece(color, pieceType, to);
}

void Board::_removePiece(Color color, PieceType pieceType, int squareIndex) {
  U64 square = ONE << squareIndex;

  _pieces[color][pieceType] ^= square;
  _allPieces[color] ^= square;

  _occupied ^= square;
  _notOccupied = ~_occupied;

  if (pieceType != KING) {
    _attackable[color] ^= square;
  }

  _zKey.flipPiece(color, pieceType, squareIndex);
  _pst.removePiece(color, pieceType, squareIndex);
}

void Board::_addPiece(Color color, PieceType pieceType, int squareIndex) {
  U64 square = ONE << squareIndex;

  _pieces[color][pieceType] |= square;
  _allPieces[color] |= square;

  _occupied |= square;
  _notOccupied = ~_occupied;

  if (pieceType != KING) {
    _attackable[color] ^= square;
  }

  _zKey.flipPiece(color, pieceType, squareIndex);
  _pst.addPiece(color, pieceType, squareIndex);
}

void Board::_resetChecks() {
  _checksCalculated[WHITE] = 0, _checksCalculated[BLACK] = 0;
  _checks[WHITE] = 0, _checks[BLACK] = 0;
}

void Board::doMove(CMove move) {
  unsigned int flags = move.getFlags();

  // En passant always cleared after a move
  _zKey.clearEnPassant();
  _enPassant = ZERO;

  // Checks always cleared after a move
  _resetChecks();

  // Handle move depending on what type of move it is
  if (move.getFlags() == 0) {
    // No flags set, not a special move
    _movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());
  } else if ((flags & CMove::CAPTURE) && (flags & CMove::PROMOTION)) { // Capture promotion special case
    // Remove captured Piece
    PieceType capturedPieceType = move.getCapturedPieceType();
    _removePiece(getInactivePlayer(), capturedPieceType, move.getTo());

    // Remove promoting pawn
    _removePiece(_activePlayer, PAWN, move.getFrom());

    // Add promoted piece
    PieceType promotionPieceType = move.getPromotionPieceType();
    _addPiece(_activePlayer, promotionPieceType, move.getTo());
  } else if (flags & CMove::CAPTURE) {
    // Remove captured Piece
    PieceType capturedPieceType = move.getCapturedPieceType();
    _removePiece(getInactivePlayer(), capturedPieceType, move.getTo());

    // Move capturing piece
    _movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());
  } else if (flags & CMove::KSIDE_CASTLE) {
    // Move the king
    _movePiece(_activePlayer, KING, move.getFrom(), move.getTo());

    // Move the correct rook
    if (_activePlayer == WHITE) {
      _movePiece(WHITE, ROOK, h1, f1);
    } else {
      _movePiece(BLACK, ROOK, h8, f8);
    }
  } else if (flags & CMove::QSIDE_CASTLE) {
    // Move the king
    _movePiece(_activePlayer, KING, move.getFrom(), move.getTo());

    // Move the correct rook
    if (_activePlayer == WHITE) {
      _movePiece(WHITE, ROOK, a1, d1);
    } else {
      _movePiece(BLACK, ROOK, a8, d8);
    }
  } else if (flags & CMove::EN_PASSANT) {
    // Remove the correct pawn
    if (_activePlayer == WHITE) {
      _removePiece(BLACK, PAWN, move.getTo() - 8);
    } else {
      _removePiece(WHITE, PAWN, move.getTo() + 8);
    }

    // Move the capturing pawn
    _movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());
  } else if (flags & CMove::PROMOTION) {
    // Remove promoted pawn
    _removePiece(_activePlayer, PAWN, move.getFrom());

    // Add promoted piece
    _addPiece(_activePlayer, move.getPromotionPieceType(), move.getTo());
  } else if (flags & CMove::DOUBLE_PAWN_PUSH) {
    _movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());

    // Set square behind pawn as _enPassant
    unsigned int enPasIndex = _activePlayer == WHITE ? move.getTo() - 8 : move.getTo() + 8;
    _enPassant = ONE << enPasIndex;
    _zKey.setEnPassantFile(enPasIndex % 8);
  }

  _updateCastlingRightsForMove(move);

  _zKey.flipActivePlayer();
  _activePlayer = getInactivePlayer();
}

bool Board::_squareUnderAttack(Color color, int squareIndex) const {
  bool squareAttacked = false;

  if (_getKnightAttacksForSquare(squareIndex, ZERO) & getPieces(color, KNIGHT)) squareAttacked = true;
  else if (_getBishopAttacksForSquare(squareIndex, ZERO) & getPieces(color, BISHOP)) squareAttacked = true;
  else if (_getRookAttacksForSquare(squareIndex, ZERO) & getPieces(color, ROOK)) squareAttacked = true;
  else if (_getQueenAttacksForSquare(squareIndex, ZERO) & getPieces(color, QUEEN)) squareAttacked = true;
  else if (_getKingAttacksForSquare(squareIndex, ZERO) & getPieces(color, KING)) squareAttacked = true;

  // Pawns special case
  if (!squareAttacked) {
    switch(color) {
      case WHITE:
        if (_getBlackPawnAttacksForSquare(squareIndex) & getPieces(WHITE, PAWN)) squareAttacked = true;
        break;
      case BLACK:
        if (_getWhitePawnAttacksForSquare(squareIndex) & getPieces(BLACK, PAWN)) squareAttacked = true;
        break;
    }
  }

  return squareAttacked;
}

void Board::_updateCastlingRightsForMove(CMove move) {
  unsigned int flags = move.getFlags();

  // Update castling flags if rooks have been captured
  if (flags & CMove::CAPTURE) {
    // Update castling rights if a rook was captured
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

  _zKey.updateCastlingRights(_whiteCanCastleKs, _whiteCanCastleQs, _blackCanCastleKs, _blackCanCastleQs);
}

void Board::setToStartPos() {
  setToFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

U64 Board::_getWhitePawnAttacksForSquare(int square) const {
  U64 fromSquare = ONE << square;

  U64 attacks = ((fromSquare << 7) & ~FILE_H) | ((fromSquare << 9) & ~FILE_A);

  return attacks;
}

U64 Board::_getBlackPawnAttacksForSquare(int square) const {
  U64 fromSquare = ONE << square;

  U64 attacks = ((fromSquare >> 7) & ~FILE_A) | ((fromSquare >> 9) & ~FILE_H);

  return attacks;
}


U64 Board::_getKnightAttacksForSquare(int square, U64 own) const {
  U64 fromSquare = ONE << square;

  U64 moves = (((fromSquare << 15) | (fromSquare >> 17)) & ~FILE_H) | // Left 1
    (((fromSquare >> 15) | (fromSquare << 17)) & ~FILE_A) | // Right 1
    (((fromSquare << 6) | (fromSquare >> 10)) & ~(FILE_G | FILE_H)) | // Left 2
    (((fromSquare >> 6) | (fromSquare << 10)) & ~(FILE_A | FILE_B)); // Right 2

  return moves & (~own);
}

U64 Board::_getKingAttacksForSquare(int square, U64 own) const {
  U64 king = ONE << square;

  U64 moves = (((king << 7) | (king >> 9) | (king >> 1)) & (~FILE_H)) |
    (((king << 9) | (king >> 7) | (king << 1)) & (~FILE_A)) |
    ((king >> 8) | (king << 8));

  return moves & (~own);
}

U64 Board::_getBishopAttacksForSquare(int square, U64 own) const {
  U64 moves = _raytable.getPositiveAttacks(RayTable::NORTH_WEST, square, _occupied) |
    _raytable.getPositiveAttacks(RayTable::NORTH_EAST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH_WEST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH_EAST, square, _occupied);

  return moves & (~own);
}

U64 Board::_getRookAttacksForSquare(int square, U64 own) const {
  U64 moves = _raytable.getPositiveAttacks(RayTable::NORTH, square, _occupied) |
    _raytable.getPositiveAttacks(RayTable::EAST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::WEST, square, _occupied);

  return moves & (~own);
}

U64 Board::_getQueenAttacksForSquare(int square, U64 own) const {
  return _getBishopAttacksForSquare(square, own) | _getRookAttacksForSquare(square, own);
}
