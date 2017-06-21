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

U64 Board::getAttacks(Color color) const {
  return _attacks[color];
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
  return (bool) (_attacks[color == WHITE ? BLACK : WHITE] & _pieces[color][KING]);
}

bool Board::whiteCanCastleKs() const {
  if (!_whiteCanCastleKs) {
    return false;
  }

  U64 passThroughSquares = (ONE << f1) | (ONE << g1);
  bool squaresOccupied = passThroughSquares & _occupied;
  bool squaresAttacked = passThroughSquares & _attacks[BLACK];

  return !colorIsInCheck(WHITE) && !squaresOccupied && !squaresAttacked;
}

bool Board::whiteCanCastleQs() const {
  if (!_whiteCanCastleQs) {
    return false;
  }

  U64 inbetweenSquares = (ONE << b1) | (ONE << c1) | (ONE << d1);
  U64 passThroughSquares = (ONE << c1) | (ONE << d1);
  bool squaresOccupied = inbetweenSquares & _occupied;
  bool squaresAttacked = passThroughSquares & _attacks[BLACK];

  return !colorIsInCheck(WHITE) && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleKs() const {
  if (!_blackCanCastleKs) {
    return false;
  }

  U64 passThroughSquares = (ONE << f8) | (ONE << g8);
  bool squaresOccupied = passThroughSquares & _occupied;
  bool squaresAttacked = passThroughSquares & _attacks[WHITE];

  return !colorIsInCheck(BLACK) && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleQs() const {
  if (!_blackCanCastleQs) {
    return false;
  }

  U64 inbetweenSquares = (ONE << b8) | (ONE << c8) | (ONE << d8);
  U64 passThroughSquares = (ONE << c8) | (ONE << d8);
  bool squaresOccupied = inbetweenSquares & _occupied;
  bool squaresAttacked = passThroughSquares & _attacks[WHITE];

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

  _updateNonPieceBitBoards();
  _zKey = ZKey(*this);
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

  _attacks[WHITE] = _genWhiteAttacks();
  _attacks[BLACK] = _genBlackAttacks();
}

PieceType Board::_getPieceAtSquare(Color color, int squareIndex) const {
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

void Board::_doRegularMove(CMove move) {
  U64* pieces = &_pieces[_activePlayer][move.getPieceType()];

  U64 fromSquare = ONE << move.getFrom();
  U64 toSquare = ONE << move.getTo();

  *pieces ^= fromSquare;
  *pieces ^= toSquare;

  _zKey.movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());
  _pst.movePiece(_activePlayer, move.getPieceType(), move.getFrom(), move.getTo());
}

void Board::doMove(CMove move) {
  if (move.getFlags() & CMove::NULL_MOVE) {
    _zKey.flipActivePlayer();
    _activePlayer = _activePlayer == WHITE ? BLACK : WHITE;
    return;
  }

  _doRegularMove(move);

  // Handle special moves
  unsigned int flags = move.getFlags();
  if (flags & CMove::CAPTURE) {
    PieceType piece = _getPieceAtSquare(getInactivePlayer(), move.getTo());

    _zKey.flipPiece(getInactivePlayer(), piece, move.getTo());
    _pst.removePiece(getInactivePlayer(), piece, move.getTo());

    _pieces[getInactivePlayer()][piece] ^= ONE << move.getTo();

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
  if (flags & CMove::KSIDE_CASTLE) {
    // King has already been moved, kingside rook must be moved
    if (_activePlayer == WHITE) {
      _pieces[WHITE][ROOK] ^= ((ONE << h1) | (ONE << f1));
      _zKey.movePiece(WHITE, ROOK, h1, f1);
      _pst.movePiece(WHITE, ROOK, h1, f1);
    } else {
      _pieces[BLACK][ROOK] ^= ((ONE << h8) | (ONE << f8));
      _zKey.movePiece(BLACK, ROOK, h8, f8);
      _pst.movePiece(BLACK, ROOK, h8, f8);
    }
  }
  if (flags & CMove::QSIDE_CASTLE) {
    // King has already been moved, kingside rook must be moved
    if (_activePlayer == WHITE) {
      _pieces[WHITE][ROOK] ^= ((ONE << a1) | (ONE << d1));
      _zKey.movePiece(WHITE, ROOK, a1, d1);
      _pst.movePiece(WHITE, ROOK, a1, d1);
    } else {
      _pieces[BLACK][ROOK] ^= ((ONE << a8) | (ONE << d8));
      _zKey.movePiece(BLACK, ROOK, a8, d8);
      _pst.movePiece(BLACK, ROOK, a8, d8);
    }
  }

  if (flags & CMove::EN_PASSANT) {
    _zKey.flipPiece(getInactivePlayer(), PAWN, move.getTo());
    _pst.removePiece(getInactivePlayer(), PAWN, move.getTo());
    if (_activePlayer == WHITE) {
      _pieces[BLACK][PAWN] ^= (_enPassant >> 8);
    } else {
      _pieces[WHITE][PAWN] ^= (_enPassant << 8);
    }
  }


  _zKey.clearEnPassant();
  if (flags & CMove::DOUBLE_PAWN_PUSH) {
    // Set square behind as _enPassant
    unsigned int enPasIndex = _activePlayer == WHITE ? move.getTo() - 8 : move.getTo() + 8;
    _enPassant = ONE << enPasIndex;
    _zKey.setEnPassantFile(enPasIndex % 8);
  } else {
    _enPassant = ZERO;
  }

  // Handle promotions
  unsigned int promotionType = flags & (CMove::QUEEN_PROMOTION | CMove::ROOK_PROMOTION | CMove::BISHOP_PROMOTION | CMove::KNIGHT_PROMOTION);
  if (promotionType) {
    PieceType promotionPiece;
    switch(promotionType) {
      case CMove::QUEEN_PROMOTION: promotionPiece = QUEEN;
        break;
      case CMove::ROOK_PROMOTION: promotionPiece = ROOK;
        break;
      case CMove::BISHOP_PROMOTION: promotionPiece = BISHOP;
        break;
      case CMove::KNIGHT_PROMOTION: promotionPiece = KNIGHT;
        break;
    }
    // Add promoted piece
    _pieces[_activePlayer][promotionPiece] ^= ONE << move.getTo();

    // Remove promoted pawn
    _pieces[_activePlayer][PAWN] ^= ONE << move.getTo();

    _zKey.flipPiece(_activePlayer, promotionPiece, move.getTo());
    _pst.addPiece(_activePlayer, promotionPiece, move.getTo());
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

  _zKey.flipActivePlayer();
  _activePlayer = _activePlayer == WHITE ? BLACK : WHITE;
  _updateNonPieceBitBoards();
}

void Board::setToStartPos() {
  setToFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

U64 Board::getWhitePawnAttacksForSquare(int square) const {
  U64 fromSquare = ONE << square;

  U64 attacks = ((fromSquare << 7) & ~FILE_H) | ((fromSquare << 9) & ~FILE_A);

  return attacks;
}

U64 Board::getBlackPawnAttacksForSquare(int square) const {
  U64 fromSquare = ONE << square;

  U64 attacks = ((fromSquare >> 7) & ~FILE_A) | ((fromSquare >> 9) & ~FILE_H);

  return attacks;
}


U64 Board::getKnightAttacksForSquare(int square, U64 own) const {
  U64 fromSquare = ONE << square;

  U64 moves = (((fromSquare << 15) | (fromSquare >> 17)) & ~FILE_H) | // Left 1
    (((fromSquare >> 15) | (fromSquare << 17)) & ~FILE_A) | // Right 1
    (((fromSquare << 6) | (fromSquare >> 10)) & ~(FILE_G | FILE_H)) | // Left 2
    (((fromSquare >> 6) | (fromSquare << 10)) & ~(FILE_A | FILE_B)); // Right 2

  return moves & (~own);
}

U64 Board::getKingAttacksForSquare(int square, U64 own) const {
  U64 king = ONE << square;

  U64 moves = (((king << 7) | (king >> 9) | (king >> 1)) & (~FILE_H)) |
    (((king << 9) | (king >> 7) | (king << 1)) & (~FILE_A)) |
    ((king >> 8) | (king << 8));

  return moves & (~own);
}

U64 Board::getBishopAttacksForSquare(int square, U64 own) const {
  U64 moves = _raytable.getPositiveAttacks(RayTable::NORTH_WEST, square, _occupied) |
    _raytable.getPositiveAttacks(RayTable::NORTH_EAST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH_WEST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH_EAST, square, _occupied);

  return moves & (~own);
}

U64 Board::getRookAttacksForSquare(int square, U64 own) const {
  U64 moves = _raytable.getPositiveAttacks(RayTable::NORTH, square, _occupied) |
    _raytable.getPositiveAttacks(RayTable::EAST, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::SOUTH, square, _occupied) |
    _raytable.getNegativeAttacks(RayTable::WEST, square, _occupied);

  return moves & (~own);
}

U64 Board::getQueenAttacksForSquare(int square, U64 own) const {
  return getBishopAttacksForSquare(square, own) | getRookAttacksForSquare(square, own);
}

U64 Board::_genWhiteAttacks() const {
  U64 attacks = U64(0);

  for(int squareIndex=0;squareIndex<64;squareIndex++) {
    U64 square = ONE << squareIndex;
    if ((square & _allPieces[WHITE]) == 0) {
      continue;
    }

    if (square & _pieces[WHITE][PAWN]) attacks |= getWhitePawnAttacksForSquare(squareIndex);
    else if (square & _pieces[WHITE][ROOK]) attacks |= getRookAttacksForSquare(squareIndex, _allPieces[WHITE]);
    else if (square & _pieces[WHITE][KNIGHT]) attacks |= getKnightAttacksForSquare(squareIndex, _allPieces[WHITE]);
    else if (square & _pieces[WHITE][BISHOP]) attacks |= getBishopAttacksForSquare(squareIndex, _allPieces[WHITE]);
    else if (square & _pieces[WHITE][KING]) attacks |= getKingAttacksForSquare(squareIndex, _allPieces[WHITE]);
    else if (square & _pieces[WHITE][QUEEN]) attacks |= getQueenAttacksForSquare(squareIndex, _allPieces[WHITE]);
  }
  attacks |= _enPassant;

  return attacks;
}

U64 Board::_genBlackAttacks() const {
  U64 attacks = U64(0);

  for(int squareIndex=0;squareIndex<64;squareIndex++) {
    U64 square = ONE << squareIndex;
    if ((square & _allPieces[BLACK]) == 0) {
      continue;
    }

    if (square & _pieces[BLACK][PAWN]) attacks |= getBlackPawnAttacksForSquare(squareIndex);
    else if (square & _pieces[BLACK][ROOK]) attacks |= getRookAttacksForSquare(squareIndex, _allPieces[BLACK]);
    else if (square & _pieces[BLACK][KNIGHT]) attacks |= getKnightAttacksForSquare(squareIndex, _allPieces[BLACK]);
    else if (square & _pieces[BLACK][BISHOP]) attacks |= getBishopAttacksForSquare(squareIndex, _allPieces[BLACK]);
    else if (square & _pieces[BLACK][KING]) attacks |= getKingAttacksForSquare(squareIndex, _allPieces[BLACK]);
    else if (square & _pieces[BLACK][QUEEN]) attacks |= getQueenAttacksForSquare(squareIndex, _allPieces[BLACK]);
  }
  attacks |= _enPassant;

  return attacks;
}
