#include "psquaretable.h"
#include "defs.h"
#include <vector>
#include <vector>
#include <algorithm>

int PSquareTable::PIECE_VALUES[2][6][64];

std::vector<int> PSquareTable::_mirrorList(std::vector<int> list) {
  std::reverse(list.begin(), list.end());
  return list;
}

void PSquareTable::_setValues(std::vector<int> list, PieceType pieceType) {
  std::copy(list.begin(), list.end(), PIECE_VALUES[BLACK][pieceType]);

  std::vector<int> mirrored = _mirrorList(list);
  std::copy(mirrored.begin(), mirrored.end(), PIECE_VALUES[WHITE][pieceType]);
}

// Values taken from https://chessprogramming.wikispaces.com/Simplified+evaluation+function
void PSquareTable::init() {
  _setValues(std::vector<int>({
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
  }), PAWN);

  _setValues(std::vector<int>({
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
  }), KNIGHT);

  _setValues(std::vector<int>({
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
  }), BISHOP);

  _setValues(std::vector<int>({
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
  }), ROOK);

  _setValues(std::vector<int>({
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
  }), QUEEN);

  _setValues(std::vector<int>({
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
  }), KING);
}

PSquareTable::PSquareTable() {
  _scores[WHITE] = 0;
  _scores[BLACK] = 0;
}

void PSquareTable::addPiece(Color color, PieceType pieceType, SquareIndex square) {
  _scores[color] += PIECE_VALUES[color][pieceType][square];
}

void PSquareTable::removePiece(Color color, PieceType pieceType, SquareIndex square) {
  _scores[color] -= PIECE_VALUES[color][pieceType][square];
}

int PSquareTable::getScore(Color color) {
  return _scores[color];
}
