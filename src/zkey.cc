#include "zkey.h"
#include "board.h"
#include "bitutils.h"
#include <random>
#include <climits>
#include <iostream>

const unsigned int ZKey::PRNG_KEY = 0xDEADBEEF;

U64 ZKey::PIECE_KEYS[2][6][64];
U64 ZKey::EN_PASSANT_KEYS[8];

U64 ZKey::KS_CASTLE_KEYS[2];
U64 ZKey::QS_CASTLE_KEYS[2];

U64 ZKey::WHITE_TO_MOVE_KEY;

void ZKey::init() {
  std::mt19937_64 mt(PRNG_KEY);
  std::uniform_int_distribution<U64> dist(ZERO, ULLONG_MAX);

  KS_CASTLE_KEYS[WHITE] = dist(mt);
  QS_CASTLE_KEYS[WHITE] = dist(mt);
  KS_CASTLE_KEYS[BLACK] = dist(mt);
  QS_CASTLE_KEYS[BLACK] = dist(mt);

  WHITE_TO_MOVE_KEY = dist(mt);

  for (int file = 0; file < 8; file++) {
    EN_PASSANT_KEYS[file] = dist(mt);
  }

  for (int pieceType = 0; pieceType < 6; pieceType++) {
    for (int square = 0; square < 64; square++) {
      PIECE_KEYS[WHITE][pieceType][square] = dist(mt);
      PIECE_KEYS[BLACK][pieceType][square] = dist(mt);
    }
  }
}

ZKey::ZKey() {
  _key = ZERO;
  _whiteKs = false, _whiteQs = false, _blackKs = false, _blackQs = false;
  _enPassantFile = -1;
}

ZKey::ZKey(const Board &board) {
  _key = ZERO;

  if (board.getActivePlayer() == WHITE) {
    _key ^= WHITE_TO_MOVE_KEY;
  }

  PieceType pieces[6] = {PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};

  // Add white/black pieces
  for (auto piece : pieces) {
    U64 whiteBitBoard = board.getPieces(WHITE, piece);
    U64 blackBitBoard = board.getPieces(BLACK, piece);

    for (int squareIndex = 0; squareIndex < 64; squareIndex++) {
      U64 square = ONE << squareIndex;
      if (square & whiteBitBoard) {
        flipPiece(WHITE, piece, squareIndex);
      } else if (square & blackBitBoard) {
        flipPiece(BLACK, piece, squareIndex);
      }
    }
  }

  // Add en passant
  if (board.getEnPassant()) {
    _enPassantFile = (_bitscanForward(board.getEnPassant())) % 8;
    _key ^= EN_PASSANT_KEYS[_enPassantFile];
  } else {
    _enPassantFile = -1;
  }

  _whiteKs = false, _whiteQs = false, _blackKs = false, _blackQs = false;
  // Add castles
  if (board.getKsCastlingRights(WHITE)) {
    _whiteKs = true;
    _flipKsCastle(WHITE);
  }
  if (board.getQsCastlingRights(WHITE)) {
    _whiteQs = true;
    _flipQsCastle(WHITE);
  }
  if (board.getKsCastlingRights(BLACK)) {
    _blackKs = true;
    _flipKsCastle(BLACK);
  }
  if (board.getQsCastlingRights(BLACK)) {
    _blackQs = true;
    _flipQsCastle(BLACK);
  }
}

void ZKey::setFromPawnStructure(const Board &board) {
  _key = ZERO;

  // Add white/black pieces
  U64 whitePawns = board.getPieces(WHITE, PAWN);
  U64 blackPawns = board.getPieces(BLACK, PAWN);

  for (unsigned int squareIndex = 0; squareIndex < 64; squareIndex++) {
    U64 square = ONE << squareIndex;
    if (square & whitePawns) {
      flipPiece(WHITE, PAWN, squareIndex);
    } else if (square & blackPawns) {
      flipPiece(BLACK, PAWN, squareIndex);
    }
  }
}

U64 ZKey::getValue() const {
  return _key;
}

void ZKey::movePiece(Color color, PieceType piece, unsigned int from, unsigned int to) {
  flipPiece(color, piece, from);
  flipPiece(color, piece, to);
}

void ZKey::flipPiece(Color color, PieceType piece, unsigned int index) {
  _key ^= PIECE_KEYS[color][piece][index];
}

void ZKey::updateCastlingRights(bool whiteKs, bool whiteQs, bool blackKs, bool blackQs) {
  if (whiteKs != _whiteKs) {
    _whiteKs = false;
    _flipKsCastle(WHITE);
  }
  if (whiteQs != _whiteQs) {
    _whiteQs = false;
    _flipQsCastle(WHITE);
  }
  if (blackKs != _blackKs) {
    _blackKs = false;
    _flipKsCastle(BLACK);
  }
  if (blackQs != _blackQs) {
    _blackQs = false;
    _flipQsCastle(BLACK);
  }
}

void ZKey::clearEnPassant() {
  if (_enPassantFile != -1) {
    _key ^= EN_PASSANT_KEYS[_enPassantFile];
    _enPassantFile = -1;
  }
}

void ZKey::setEnPassantFile(unsigned int file) {
  _enPassantFile = file;
  _key ^= EN_PASSANT_KEYS[file];
}

void ZKey::_flipKsCastle(Color color) {
  _key ^= KS_CASTLE_KEYS[color];
}

void ZKey::_flipQsCastle(Color color) {
  _key ^= QS_CASTLE_KEYS[color];
}

void ZKey::flipActivePlayer() {
  _key ^= WHITE_TO_MOVE_KEY;
}

bool ZKey::operator==(const ZKey &other) {
  return other.getValue() == _key;
}
