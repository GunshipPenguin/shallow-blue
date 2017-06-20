#ifndef PSQUARETABLE_H
#define PSQUARETABLE_H

#include "defs.h"
#include <vector>


class PSquareTable {
public:
  PSquareTable();
  static void init();

  void addPiece(Color, PieceType, SquareIndex);
  void removePiece(Color, PieceType, SquareIndex);
  void movePiece(Color, PieceType, SquareIndex, SquareIndex);

  int getScore(Color);

private:
  static int PIECE_VALUES[2][6][64];
  static void _setValues(std::vector<int>, PieceType);
  static std::vector<int> _mirrorList(std::vector<int>);

  int _scores[2];

};

#endif
