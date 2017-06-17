#include "zkey.h"
#include "defs.h"
#include "board.h"
#include <random>
#include <climits>
#include <iostream>

const unsigned int ZKey::PRNG_KEY = 0xDEADBEEF;

U64 ZKey::WHITE_PIECE_KEYS[6][64];
U64 ZKey::BLACK_PIECE_KEYS[6][64];
U64 ZKey::EN_PASSANT_KEYS[64];

U64 ZKey::WHITE_KS_CASTLE_KEY;
U64 ZKey::WHITE_QS_CASTLE_KEY;
U64 ZKey::BLACK_KS_CASTLE_KEY;
U64 ZKey::BLACK_QS_CASTLE_KEY;

void ZKey::init() {
  std::mt19937_64 mt(PRNG_KEY);
  std::uniform_int_distribution<U64> dist(ZERO, ULLONG_MAX);

  WHITE_KS_CASTLE_KEY = dist(mt);
  WHITE_QS_CASTLE_KEY = dist(mt);
  BLACK_KS_CASTLE_KEY = dist(mt);
  BLACK_QS_CASTLE_KEY = dist(mt);

  for (int square=0;square<64;square++) {
    EN_PASSANT_KEYS[square] = dist(mt);
  }

  for (int pieceType=0;pieceType<=6;pieceType++) {
    for (int square=0;square<64;square++) {
      WHITE_PIECE_KEYS[pieceType][square] = dist(mt);
      BLACK_PIECE_KEYS[pieceType][square] = dist(mt);
    }
  }
}

ZKey::ZKey() {
  _key = ZERO;
}

ZKey::ZKey(Board board) {
  _key = ZERO;

  PieceType pieces[6] = {PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};

  // Add white/black pieces
  for (auto piece : pieces) {
    U64 whiteBitBoard = board.getWhitePieces(piece);
    U64 blackBitBoard = board.getBlackPieces(piece);

    for (int squareIndex=0;squareIndex<64;squareIndex++) {
      U64 square = ONE << squareIndex;
      if (square & whiteBitBoard) {
        setWhitePiece(piece, squareIndex);
      } else if (square & blackBitBoard) {
        setBlackPiece(piece, squareIndex);
      }
    }
  }

  // Add en passants
  U64 enPassant = board.getEnPassant();
  for (int squareIndex=0;squareIndex<64;squareIndex++) {
    U64 square = ONE << squareIndex;
    if (square & enPassant) {
      setEnPassant(squareIndex);
      break;
    }
  }

  // Add castles
  if (board.whiteCanCastleKs()) {
    setWhiteCastleKs();
  }
  if (board.whiteCanCastleQs()) {
    setWhiteCastleQs();
  }
  if (board.blackCanCastleKs()) {
    setBlackCastleKs();
  }
  if (board.blackCanCastleQs()) {
    setBlackCastleQs();
  }
}

U64 ZKey::getValue() {
  return _key;
}

void ZKey::setWhitePiece(PieceType piece, unsigned int index) {
  _key ^= WHITE_PIECE_KEYS[piece][index];
}

void ZKey::setBlackPiece(PieceType piece, unsigned int index) {
  _key ^= BLACK_PIECE_KEYS[piece][index];
}

void ZKey::setEnPassant(unsigned int index) {
  _key ^= EN_PASSANT_KEYS[index];
}

void ZKey::setWhiteCastleKs() {
  _key ^= WHITE_KS_CASTLE_KEY;
}

void ZKey::setWhiteCastleQs() {
  _key ^= WHITE_QS_CASTLE_KEY;
}

void ZKey::setBlackCastleKs() {
  _key ^= BLACK_KS_CASTLE_KEY;
}

void ZKey::setBlackCastleQs() {
  _key ^= BLACK_QS_CASTLE_KEY;
}
