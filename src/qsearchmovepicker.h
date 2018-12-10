#ifndef QSEARCHMOVEPICKER_H
#define QSEARCHMOVEPICKER_H

#include "movepicker.h"

/**
 * @brief MovePicker that returns moves in an optimal order for quiescense 
 * search
 * 
 * Specifically, the QSearchMovePicker returns only capture moves in MVV/LVA
 * order, followed by promotions by value of promotion piece, ignoring all
 * other moves.
 */
class QSearchMovePicker : MovePicker {
 public:
  /**
   * @brief Constructs a new QSearchMovePicker for the given MoveList
   * 
   * @param moveList MoveList to pick moves from
   */
  QSearchMovePicker(MoveList *);

  bool hasNext() const override;

  /**
  * @brief Returns the next best move in this QSearchMovePicker's MoveList for quiescense search.
  *
  * Note that internally, this method performs a selection sort for one unpicked capture move of the
  * highest value in this QSearchMovePicker's internal MoveList and thus has time complexity
  * O(n) with respect to the size of the MoveList. Provided move ordering is close to optimal though,
  * this should be the optimal behaviour as quiescense search should hit a beta cutoff after a small
  * number of good captures.
  *
  * @return the next best move in this QSearchMovePicker's MoveList for quiescense search
  */
  Move getNext() override;

 private:
  /**
   * @brief Assigns a value to each capture move in this QSearchMovePicker representing desirability
   * according to MVV/LVA.
   */
  void _scoreMoves();

  /**
   * @brief Head of the current sorted part of the MoveList
   */
  size_t _currHead;

  /**
   * @brief Number of captures in this QSearchMovePicker's MoveList
   */
  size_t _numCaptures;
};

#endif