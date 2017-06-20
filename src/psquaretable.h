#ifndef PSQUARETABLE_H
#define PSQUARETABLE_H

#include "defs.h"
#include <vector>


class PSquareTable {
public:
  PSquareTable();
  static void init();

  void addPiece(Color, PieceType, unsigned int);
  void removePiece(Color, PieceType, unsigned int);
  void movePiece(Color, PieceType, unsigned int, unsigned int);

  int getScore(Color);

private:
  static int PIECE_VALUES[2][6][64];
  static void _setValues(std::vector<int>, PieceType);
  static std::vector<int> _mirrorList(std::vector<int>);

  int _scores[2];

};

#endif
