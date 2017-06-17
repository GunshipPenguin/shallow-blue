#include "zkey.h"
#include "defs.h"
#include "board.h"
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

  for (int file=0;file<8;file++) {
    EN_PASSANT_KEYS[file] = dist(mt);
  }

  for (int pieceType=0;pieceType<=6;pieceType++) {
    for (int square=0;square<64;square++) {
      PIECE_KEYS[WHITE][pieceType][square] = dist(mt);
      PIECE_KEYS[BLACK][pieceType][square] = dist(mt);
    }
  }
}

ZKey::ZKey() {
  _key = ZERO;
}

ZKey::ZKey(Board board) {
  _key = ZERO;

  if (board.getActivePlayer() == WHITE) {
    _key ^= WHITE_TO_MOVE_KEY;
  }

  PieceType pieces[6] = {PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};

  // Add white/black pieces
  for (auto piece : pieces) {
    U64 whiteBitBoard = board.getPieces(WHITE, piece);
    U64 blackBitBoard = board.getPieces(BLACK,  piece);

    for (int squareIndex=0;squareIndex<64;squareIndex++) {
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
    _enPassantFile = (__builtin_ffsll(board.getEnPassant()) - 1) % 8;
    _key ^= EN_PASSANT_KEYS[_enPassantFile];
  } else {
    _enPassantFile = -1;
  }

  _whiteKs = false, _whiteQs = false, _blackKs = false, _blackQs = false;
  // Add castles
  if (board.whiteKsCastlingRight()) {
    _whiteKs = true;
    _flipKsCastle(WHITE);
  }
  if (board.whiteQsCastlingRight()) {
    _whiteQs = true;
    _flipQsCastle(WHITE);
  }
  if (board.blackKsCastlingRight()) {
    _blackKs = true;
    _flipKsCastle(BLACK);
  }
  if (board.blackQsCastlingRight()) {
    _blackQs = true;
    _flipQsCastle(BLACK);
  }
}

U64 ZKey::getValue() {
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
  if (whiteQs != _blackQs) {
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
