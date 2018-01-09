#include "board.h"
#include "move.h"
#include "raytable.h"
#include "defs.h"
#include "bitutils.h"
#include "attacktable.h"
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>
#include <stdexcept>

Board::Board() {
  setToFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

Board::Board(std::string fen) {
  setToFen(fen);
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
    default: throw new std::logic_error("Invalid piece type");
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
  int kingSquare = _bitscanForward(getPieces(color, KING));
  return _squareUnderAttack(getOppositeColor(color), kingSquare);
}

int Board::getHalfmoveClock() const {
  return _halfmoveClock;
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
  std::string stringRep = "8  ";
  int rank = 8;

  U64 boardPos = 56; // Starts at a8, goes down rank by rank
  int squaresProcessed = 0;

  while (squaresProcessed < 64) {
    U64 square = ONE << boardPos;
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
    squaresProcessed++;

    if ((squaresProcessed % 8 == 0) && (squaresProcessed != 64)) {
      stringRep += "\n" + std::to_string(--rank) + "  ";
      boardPos -= 16;
    }

    boardPos ++;
  }

  stringRep += "\n\n    A  B  C  D  E  F  G  H";
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

  // En passant target square
  std::string enPasSquare;
  fenStream >> enPasSquare;
  if (enPasSquare == "-") {
    _enPassant = 0;
  } else {
    int enPasIndex = Move::notationToIndex(enPasSquare);
    _enPassant = static_cast<U64>(1) << enPasIndex;
  }

  // Halfmove clock
  fenStream >> _halfmoveClock;

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
  else throw std::logic_error((color == WHITE ? std::string("White") : std::string("Black")) +
      " piece at square " + std::to_string(squareIndex) + " does not exist");

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

void Board::doMove(Move move) {
  unsigned int flags = move.getFlags();

  // En passant always cleared after a move
  _zKey.clearEnPassant();
  _enPassant = ZERO;

  // Handle move depending on what type of move it is
  if (!flags) {
    // No flags set, not a special move
    _movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());
  } else if ((flags & Move::CAPTURE) && (flags & Move::PROMOTION)) { // Capture promotion special case
    // Remove captured Piece
    PieceType capturedPieceType = move.getCapturedPieceType();
    _removePiece(getInactivePlayer(), capturedPieceType, move.getTo());

    // Remove promoting pawn
    _removePiece(_activePlayer, PAWN, move.getFrom());

    // Add promoted piece
    PieceType promotionPieceType = move.getPromotionPieceType();
    _addPiece(_activePlayer, promotionPieceType, move.getTo());
  } else if (flags & Move::CAPTURE) {
    // Remove captured Piece
    PieceType capturedPieceType = move.getCapturedPieceType();
    _removePiece(getInactivePlayer(), capturedPieceType, move.getTo());

    // Move capturing piece
    _movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());
  } else if (flags & Move::KSIDE_CASTLE) {
    // Move the king
    _movePiece(_activePlayer, KING, move.getFrom(), move.getTo());

    // Move the correct rook
    if (_activePlayer == WHITE) {
      _movePiece(WHITE, ROOK, h1, f1);
    } else {
      _movePiece(BLACK, ROOK, h8, f8);
    }
  } else if (flags & Move::QSIDE_CASTLE) {
    // Move the king
    _movePiece(_activePlayer, KING, move.getFrom(), move.getTo());

    // Move the correct rook
    if (_activePlayer == WHITE) {
      _movePiece(WHITE, ROOK, a1, d1);
    } else {
      _movePiece(BLACK, ROOK, a8, d8);
    }
  } else if (flags & Move::EN_PASSANT) {
    // Remove the correct pawn
    if (_activePlayer == WHITE) {
      _removePiece(BLACK, PAWN, move.getTo() - 8);
    } else {
      _removePiece(WHITE, PAWN, move.getTo() + 8);
    }

    // Move the capturing pawn
    _movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());
  } else if (flags & Move::PROMOTION) {
    // Remove promoted pawn
    _removePiece(_activePlayer, PAWN, move.getFrom());

    // Add promoted piece
    _addPiece(_activePlayer, move.getPromotionPieceType(), move.getTo());
  } else if (flags & Move::DOUBLE_PAWN_PUSH) {
    _movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());

    // Set square behind pawn as _enPassant
    unsigned int enPasIndex = _activePlayer == WHITE ? move.getTo() - 8 : move.getTo() + 8;
    _enPassant = ONE << enPasIndex;
    _zKey.setEnPassantFile(enPasIndex % 8);
  }

  // Halfmove clock reset on pawn moves or captures, incremented otherwise
  if (move.getPieceType() == PAWN || move.getFlags() & Move::CAPTURE) {
    _halfmoveClock = 0;
  } else {
    _halfmoveClock++;
  }

  _updateCastlingRightsForMove(move);

  _zKey.flipActivePlayer();
  _activePlayer = getInactivePlayer();
}

bool Board::_squareUnderAttack(Color color, int squareIndex) const {
  // Check for pawn, knight and king attacks
  if (AttackTable::getAttacks(PAWN, squareIndex, getOppositeColor(color)) & getPieces(color, PAWN)) return true;
  if (AttackTable::getAttacks(KNIGHT, squareIndex) & getPieces(color, KNIGHT)) return true;
  if (AttackTable::getAttacks(KING, squareIndex) & getPieces(color, KING)) return true;

  // Check for bishop/queen attacks
  U64 bishopsQueens = getPieces(color, BISHOP) | getPieces(color, QUEEN);
  if (_getBishopAttacksForSquare(squareIndex, ZERO) & bishopsQueens) return true;

  // Check for rook/queen attacks
  U64 rooksQueens = getPieces(color, ROOK) | getPieces(color, QUEEN);
  if (_getRookAttacksForSquare(squareIndex, ZERO) & rooksQueens) return true;

  return false;
}

void Board::_updateCastlingRightsForMove(Move move) {
  unsigned int flags = move.getFlags();

  // Update castling flags if rooks have been captured
  if (flags & Move::CAPTURE) {
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
  U64 attacks = AttackTable::getAttacks(PAWN, square, WHITE);

  return attacks;
}

U64 Board::_getBlackPawnAttacksForSquare(int square) const {
  U64 attacks = AttackTable::getAttacks(PAWN, square, BLACK);

  return attacks;
}


U64 Board::_getKnightAttacksForSquare(int square, U64 own) const {
  U64 moves = AttackTable::getAttacks(KNIGHT, square);

  return moves & (~own);
}

U64 Board::_getKingAttacksForSquare(int square, U64 own) const {
  U64 moves = AttackTable::getAttacks(KING, square);

  return moves & (~own);
}

U64 Board::_getBishopAttacksForSquare(int square, U64 own) const {
  U64 moves = RayTable::getPositiveAttacks(RayTable::NORTH_WEST, square, _occupied) |
    RayTable::getPositiveAttacks(RayTable::NORTH_EAST, square, _occupied) |
    RayTable::getNegativeAttacks(RayTable::SOUTH_WEST, square, _occupied) |
    RayTable::getNegativeAttacks(RayTable::SOUTH_EAST, square, _occupied);

  return moves & (~own);
}

U64 Board::_getRookAttacksForSquare(int square, U64 own) const {
  U64 moves = RayTable::getPositiveAttacks(RayTable::NORTH, square, _occupied) |
    RayTable::getPositiveAttacks(RayTable::EAST, square, _occupied) |
    RayTable::getNegativeAttacks(RayTable::SOUTH, square, _occupied) |
    RayTable::getNegativeAttacks(RayTable::WEST, square, _occupied);

  return moves & (~own);
}

U64 Board::_getQueenAttacksForSquare(int square, U64 own) const {
  return _getBishopAttacksForSquare(square, own) | _getRookAttacksForSquare(square, own);
}
