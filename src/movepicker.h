#ifndef MOVEPICKER_H
#define MOVEPICKER_H

#include "board.h"
#include "orderinginfo.h"
#include <cmath>

/**
 * @brief Abstract base class for an object that picks moves from a move list in an optimal order.
 * 
 * Implementations must define hasNext(), which returns true if there are remaining moves
 * to be processed and getNext(), which gets the next move from the provided MoveList.
 * 
 * Note that instances of a MovePicker modify their provided MoveList instances in place.
 */
class MovePicker {
public:
  /**
   * @brief Constructs a new MovePicker with the given MoveList.
   * 
   * @param moveList list of moves to pick from.
   */
  MovePicker(MoveList*);
  
  /**
   * @brief Returns the next best move from this MovePicker's MoveList.
   * 
   * @return Move the next best move from this MovePicker's MoveList
   */
  virtual Move getNext() = 0;

  /**
   * @brief Returns true if there are more moves to be picked from this MovePicker's MoveList
   * 
   * @return true if there are more moves to be picked from this MovePicker's MoveList, false otherwise.
   */
  virtual bool hasNext() const = 0;

  /**
   * @brief Initializes constants used in picking moves.
   * 
   * Currently this initialized the internal _mvvLvaTable array only.
   */
  static void init();
protected:
  /**
   * @brief List of moves this MovePicker picks from
   */
  MoveList* _moves;

  /**
   * @brief Table mapping [victimValue][attackerValue] to an integer represnting move desirability
   * according to MVV/LVA.
   */
  static int _mvvLvaTable[5][6];

  /**
   * @brief Bonuses applied to specific move types.
   * @{
   */
  static constexpr int CAPTURE_BONUS = std::pow(10, 5);
  static constexpr int PROMOTION_BONUS = std::pow(10, 4);
  static constexpr int KILLER1_BONUS = std::pow(10, 3);
  static constexpr int KILLER2_BONUS = std::pow(10, 2);
  static constexpr int QUIET_BONUS = std::pow(10, 1);
  /**@}*/
};

#endif