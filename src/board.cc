#include "board.h"
#include "bitutils.h"
#include "attacks.h"
#include <sstream>

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
  return _allPieces[color] & ~_pieces[color][KING];
}

U64 Board::getOccupied() const {
  return _occupied;
}

U64 Board::getNotOccupied() const {
  return ~_occupied;
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
    default: fatal("Invalid piece type");
  }

  return attacks;
}

Color Board::getInactivePlayer() const {
  return _activePlayer == WHITE ? BLACK : WHITE;
}

ZKey Board::getZKey() const {
  return _zKey;
}

ZKey Board::getPawnStructureZKey() const {
  return _pawnStructureZkey;
}

PSquareTable Board::getPSquareTable() const {
  return _pst;
}

bool Board::colorIsInCheck(Color color) const {
  int kingSquare = _bitscanForward(getPieces(color, KING));
  
  // Don't choke in testing scenarios where there is no king
  if (kingSquare == -1) {
    return false;
  }

  return _squareUnderAttack(getOppositeColor(color), kingSquare);
}

int Board::getHalfmoveClock() const {
  return _halfmoveClock;
}

bool Board::whiteCanCastleKs() const {
  if (!getKsCastlingRights(WHITE)) {
    return false;
  }

  U64 passThroughSquares = (ONE << f1) | (ONE << g1);
  bool squaresOccupied = passThroughSquares & _occupied;
  bool squaresAttacked = _squareUnderAttack(BLACK, f1) || _squareUnderAttack(BLACK, g1);

  return !colorIsInCheck(WHITE) && !squaresOccupied && !squaresAttacked;
}

bool Board::whiteCanCastleQs() const {
  if (!getQsCastlingRights(WHITE)) {
    return false;
  }

  U64 inbetweenSquares = (ONE << c1) | (ONE << d1) | (ONE << b1);
  bool squaresOccupied = inbetweenSquares & _occupied;
  bool squaresAttacked = _squareUnderAttack(BLACK, d1) || _squareUnderAttack(BLACK, c1);

  return !colorIsInCheck(WHITE) && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleKs() const {
  if (!getKsCastlingRights(BLACK)) {
    return false;
  }

  U64 passThroughSquares = (ONE << f8) | (ONE << g8);
  bool squaresOccupied = passThroughSquares & _occupied;
  bool squaresAttacked = _squareUnderAttack(WHITE, f8) || _squareUnderAttack(WHITE, g8);

  return !colorIsInCheck(BLACK) && !squaresOccupied && !squaresAttacked;
}

bool Board::blackCanCastleQs() const {
  if (!getQsCastlingRights(BLACK)) {
    return false;
  }

  U64 inbetweenSquares = (ONE << b8) | (ONE << c8) | (ONE << d8);
  bool squaresOccupied = inbetweenSquares & _occupied;
  bool squaresAttacked = _squareUnderAttack(WHITE, c8) || _squareUnderAttack(WHITE, d8);

  return !colorIsInCheck(BLACK) && !squaresOccupied && !squaresAttacked;
}

bool Board::getKsCastlingRights(Color color) const {
  switch (color) {
    case WHITE: return _castlingRights & 0x1;
    default: return _castlingRights & 0x4;
  }
}

bool Board::getQsCastlingRights(Color color) const {
  switch (color) {
    case WHITE: return _castlingRights & 0x2;
    default: return _castlingRights & 0x8;
  }
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
      switch (squaresProcessed / 8) {
        case 1:
          stringRep += "        ";
          stringRep += getActivePlayer() == WHITE ? "White" : "Black";
          stringRep += " to Move";
          break;
        case 2:
          stringRep += "        Halfmove Clock: ";
          stringRep += std::to_string(_halfmoveClock);
          break;
        case 3:
          stringRep += "        Castling Rights: ";
          stringRep += _castlingRights & 1 ? "K" : "";
          stringRep += _castlingRights & 2 ? "Q" : "";
          stringRep += _castlingRights & 4 ? "k" : "";
          stringRep += _castlingRights & 8 ? "q" : "";
          break;
        case 4:
          stringRep += "        En Passant Square: ";
          stringRep += _enPassant == ZERO ? "-" : Move::indexToNotation(_bitscanForward(_enPassant));
          break;
      }
      stringRep += "\n" + std::to_string(--rank) + "  ";
      boardPos -= 16;
    }

    boardPos++;
  }

  stringRep += "\n\n    A  B  C  D  E  F  G  H";
  return stringRep;
}

void Board::_clearBitBoards() {
  for (Color color: {WHITE, BLACK}) {
    for (PieceType pieceType : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
      _pieces[color][pieceType] = ZERO;
    }

    _allPieces[WHITE] = ZERO;
    _allPieces[BLACK] = ZERO;
  }

  _enPassant = ZERO;

  _occupied = ZERO;
}

void Board::setToFen(std::string fenString) {
  std::istringstream fenStream(fenString);
  std::string token;

  _clearBitBoards();

  U64 boardPos = 56; // Fen string starts at a8 = index 56
  fenStream >> token;
  for (auto currChar : token) {
    switch (currChar) {
      case 'p': _pieces[BLACK][PAWN] |= (ONE << boardPos++);
        break;
      case 'r': _pieces[BLACK][ROOK] |= (ONE << boardPos++);
        break;
      case 'n': _pieces[BLACK][KNIGHT] |= (ONE << boardPos++);
        break;
      case 'b': _pieces[BLACK][BISHOP] |= (ONE << boardPos++);
        break;
      case 'q': _pieces[BLACK][QUEEN] |= (ONE << boardPos++);
        break;
      case 'k': _pieces[BLACK][KING] |= (ONE << boardPos++);
        break;
      case 'P': _pieces[WHITE][PAWN] |= (ONE << boardPos++);
        break;
      case 'R': _pieces[WHITE][ROOK] |= (ONE << boardPos++);
        break;
      case 'N': _pieces[WHITE][KNIGHT] |= (ONE << boardPos++);
        break;
      case 'B': _pieces[WHITE][BISHOP] |= (ONE << boardPos++);
        break;
      case 'Q': _pieces[WHITE][QUEEN] |= (ONE << boardPos++);
        break;
      case 'K': _pieces[WHITE][KING] |= (ONE << boardPos++);
        break;
      case '/': boardPos -= 16; // Go down one rank
        break;
      default:boardPos += static_cast<U64>(currChar - '0');
    }
  }

  // Next to move
  fenStream >> token;
  _activePlayer = token == "w" ? WHITE : BLACK;

  // Castling availability
  fenStream >> token;
  _castlingRights = 0;
  for (auto currChar : token) {
    switch (currChar) {
      case 'K': _castlingRights |= 0x1;
        break;
      case 'Q': _castlingRights |= 0x2;
        break;
      case 'k': _castlingRights |= 0x4;
        break;
      case 'q': _castlingRights |= 0x8;
        break;
    }
  }

  // En passant target square
  fenStream >> token;
  _enPassant = token == "-" ? ZERO : ONE << Move::notationToIndex(token);

  // Halfmove clock
  fenStream >> _halfmoveClock;

  _updateNonPieceBitBoards();
  _zKey = ZKey(*this);
  _pawnStructureZkey.setFromPawnStructure(*this);

  _pst = PSquareTable(*this);
}

void Board::_updateNonPieceBitBoards() {
  _allPieces[WHITE] = _pieces[WHITE][PAWN] | \
    _pieces[WHITE][ROOK] | \
    _pieces[WHITE][KNIGHT] | \
    _pieces[WHITE][BISHOP] | \
    _pieces[WHITE][QUEEN] | \
    _pieces[WHITE][KING];

  _allPieces[BLACK] = _pieces[BLACK][PAWN] | \
    _pieces[BLACK][ROOK] | \
    _pieces[BLACK][KNIGHT] | \
    _pieces[BLACK][BISHOP] | \
    _pieces[BLACK][QUEEN] | \
    _pieces[BLACK][KING];

  _occupied = _allPieces[WHITE] | _allPieces[BLACK];
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
  else
    fatal((color == WHITE ? std::string("White") : std::string("Black")) +
        " piece at square " + std::to_string(squareIndex) + " does not exist");

  return piece;
}

void Board::_movePiece(Color color, PieceType pieceType, int from, int to) {
  U64 squareMask =  (ONE << to) | (ONE << from);

  _pieces[color][pieceType] ^= squareMask;
  _allPieces[color] ^= squareMask;

  _occupied ^= squareMask;

  _zKey.movePiece(color, pieceType, from, to);
  _pst.movePiece(color, pieceType, from, to);
}

void Board::_removePiece(Color color, PieceType pieceType, int squareIndex) {
  U64 square = ONE << squareIndex;

  _pieces[color][pieceType] ^= square;
  _allPieces[color] ^= square;

  _occupied ^= square;

  _zKey.flipPiece(color, pieceType, squareIndex);
  _pst.removePiece(color, pieceType, squareIndex);
}

void Board::_addPiece(Color color, PieceType pieceType, int squareIndex) {
  U64 square = ONE << squareIndex;

  _pieces[color][pieceType] |= square;
  _allPieces[color] |= square;

  _occupied |= square;

  _zKey.flipPiece(color, pieceType, squareIndex);
  _pst.addPiece(color, pieceType, squareIndex);
}

void Board::doMove(Move move) {
  // Clear En passant info after each move if it exists
  if (_enPassant) {
    _zKey.clearEnPassant();
    _enPassant = ZERO;
  }

  // Handle move depending on what type of move it is
  unsigned int flags = move.getFlags();
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

  if (_castlingRights) {
    _updateCastlingRightsForMove(move);
  }

  // Update pawn structure ZKey if this is a pawn move
  if (move.getPieceType() == PAWN) {
    _pawnStructureZkey.movePiece(_activePlayer, PAWN, move.getFrom(), move.getTo());
  }

  _zKey.flipActivePlayer();
  _activePlayer = getInactivePlayer();
}

bool Board::_squareUnderAttack(Color color, int squareIndex) const {
  // Check for pawn, knight and king attacks
  if (Attacks::getNonSlidingAttacks(PAWN, squareIndex, getOppositeColor(color)) & getPieces(color, PAWN)) return true;
  if (Attacks::getNonSlidingAttacks(KNIGHT, squareIndex) & getPieces(color, KNIGHT)) return true;
  if (Attacks::getNonSlidingAttacks(KING, squareIndex) & getPieces(color, KING)) return true;

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
    switch (move.getTo()) {
      case a1: _castlingRights &= ~0x2;
        break;
      case h1: _castlingRights &= ~0x1;
        break;
      case a8: _castlingRights &= ~0x8;
        break;
      case h8: _castlingRights &= ~0x4;
        break;
    }
  }

  // Update castling flags if rooks or kings have moved
  switch (move.getFrom()) {
    case e1: _castlingRights &= ~0x3;
      break;
    case e8: _castlingRights &= ~0xC;
      break;
    case a1: _castlingRights &= ~0x2;
      break;
    case h1: _castlingRights &= ~0x1;
      break;
    case a8: _castlingRights &= ~0x8;
      break;
    case h8: _castlingRights &= ~0x4;
      break;
  }

  _zKey.updateCastlingRights(getKsCastlingRights(WHITE),
                             getQsCastlingRights(WHITE), getKsCastlingRights(BLACK), getQsCastlingRights(BLACK));
}

void Board::setToStartPos() {
  setToFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

U64 Board::_getWhitePawnAttacksForSquare(int square) const {
  return Attacks::getNonSlidingAttacks(PAWN, square, WHITE);
}

U64 Board::_getBlackPawnAttacksForSquare(int square) const {
  return Attacks::getNonSlidingAttacks(PAWN, square, BLACK);
}

U64 Board::_getKnightAttacksForSquare(int square, U64 own) const {
  return Attacks::getNonSlidingAttacks(KNIGHT, square) & ~own;
}

U64 Board::_getKingAttacksForSquare(int square, U64 own) const {
  return Attacks::getNonSlidingAttacks(KING, square) & ~own;
}

U64 Board::_getBishopAttacksForSquare(int square, U64 own) const {
  return Attacks::getSlidingAttacks(BISHOP, square, _occupied) & ~own;
}

U64 Board::_getRookAttacksForSquare(int square, U64 own) const {
  return Attacks::getSlidingAttacks(ROOK, square, _occupied) & ~own;
}

U64 Board::_getQueenAttacksForSquare(int square, U64 own) const {
  return Attacks::getSlidingAttacks(QUEEN, square, _occupied) & ~own;
}
