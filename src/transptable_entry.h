#include "move.h"

class TranspTableEntry {
public:
  enum Flag {
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
  };

  TranspTableEntry(int score, int depth, Flag flag, Move bestMove)
    : _score(score), _depth(depth), _flag(flag), _bestMove(bestMove) {}

  int getScore() const { return _score; }
  int getDepth() const { return _depth; }
  int getFlag() const { return _flag; }
  Move getBestMove() const { return _bestMove; }
private:
  int _score;
  int _depth;
  Flag _flag;
  Move _bestMove;
};
