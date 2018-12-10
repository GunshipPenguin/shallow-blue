#ifndef GENERALMOVEPICKER_H
#define GENERALMOVEPICKER_H

#include "movepicker.h"
#include "orderinginfo.h"
#include "board.h"

/**
 * @brief MovePicker that returns moves in an optimal order for negamax search.
 * 
 * Specifically, the GeneralMovePicker returns moves in the following order:
 * - Hash move from the transposition table (if it exists)
 * - Captures sorted by MVV/LVA
 * - Promotions
 * - Killer moves
 * - Quiet moves sorted by the history heuristic
 */
class GeneralMovePicker : MovePicker {
 public:
  /**
   * @brief Constructs a new GeneralMovePicker for the given OrderingInfo, Board and MoveList.
   *  
   * @param orderingInfo OrderingInfo object containing information about the current state of the search
   * @param board Current board state for all moves in the provided MoveList
   * @param moveList Pointer to the MoveList to pick moves from
   */
  GeneralMovePicker(const OrderingInfo *, const Board *, MoveList *);

  bool hasNext() const override;

  /**
   * @brief Returns the next best move in this GeneralMovePicker's MoveList for negamax search.
   * 
   * Note that internally, this method performs a selection sort for one unpicked move of the
   * highest value in this GeneralMovePicker's internal MoveList and thus has time complexity
   * O(n) with respect to the size of the MoveList. Provided move ordering is close to optimal though,
   * this should be the optimal behaviour as negamax search should hit a beta cutoff after a small
   * number of good moves.
   * 
   * @return the next best move in this GeneralMovePicker's MoveList for negamax search.
   */
  Move getNext() override;

 private:
  /**
   * @brief Assigns a value to each move in this GeneralMovePicker's MoveList representing desirability
   * in a negamax search.
   */
  void _scoreMoves();

  /**
   * @brief Position of the first unpicked move in this GeneralMovePicker's MoveList
   */
  size_t _currHead;

  /**
   * @brief OrderingInfo object containing search related information used by this GeneralMovePicker
   */
  const OrderingInfo *_orderingInfo;

  /**
   * @brief Board for all moves in the provided MoveList
   */
  const Board *_board;
};

#endif