#ifndef EVAL_H
#define EVAL_H

#include "board.h"
#include "defs.h"

/**
 * @brief Represents the evaluation of a chess board at a given position for a given color.
 */
class Eval {
public:

  /**
   * @brief Evaluate the given board for the given color.
   */
  Eval(const Board&, Color);

  /**
   * @brief Get a score representing the favourability of the position for the color with which
   * this Eval was constructed.
   *
   * @return A favourability score for the board and color.
   */
  int getScore();

private:
  /**
   * @name Piece material value constants.
   *
   * @{
   */
  static const int PAWN_VALUE = 100;
  static const int KNIGHT_VALUE = 320;
  static const int BISHOP_VALUE = 330;
  static const int ROOK_VALUE = 500;
  static const int QUEEN_VALUE = 900;
  /**@}*/

  /**
   * @brief Mobility weight factored into an evaluation score.
   */
  static const int MOBILITY_WEIGHT = 1;

  int _score;
  /**
   * @brief Evaluate the given board for the given color and update this Evaluation's state appropriately
   */
  void _doEval(const Board&, Color);

  /**
   * @brief Returns an estimate for the number of legal moves for the given color on the given board.
   *
   * @param board The board to estimate the number of moves for
   * @param color The color to estimate the number of moves for
   * @return An estimate for the number of moves for the given color on the given board.
   */
  int _calcMobility(const Board&, Color);
};

#endif
