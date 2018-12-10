#ifndef TRANSPTABLEENTRY_H
#define TRANSPTABLEENTRY_H

#include "move.h"

/**
 * @brief Represents an entry in a transposition table.
 *
 * Stores score, depth, upper/lower bound information and the best move found
 */
class TranspTableEntry {
 public:
  /**
   * @enum Flag
   * @brief Flag stored with each transposition table entry indicating its type.
   */
  enum Flag {
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
  };

  /**
   * @brief Construct a new transposition table entry with the given score, depth,
   * type flag and best move.
   *
   * @param score Score at node
   * @param depth Depth node was searched to
   * @param flag Type flag for this entry
   * @param bestMove Best move found at this node
   */
  TranspTableEntry(int score, int depth, Flag flag, Move bestMove)
      : _score(score), _depth(depth), _flag(flag), _bestMove(bestMove) {}

  /**
   * @brief Get the score stored in this transposition table entry.
   *
   * @return The score stored in this transposition table entry
   */
  int getScore() const { return _score; }

  /**
   * @brief Get the depth of this transposition table entry.
   *
   * @return The depth of this transposition table entry
   */
  int getDepth() const { return _depth; }

  /**
   * @brief Get the type flag of this transposition table entry.
   *
   * @return The type flag of this transposition table entry
   */
  int getFlag() const { return _flag; }

  /**
   * @brief Get the best move of this transposition table entry.
   *
   * @return The best move of this transposition table entry.
   */
  Move getBestMove() const { return _bestMove; }

 private:

  /** @brief Score of this transposition table entry */
  int _score;

  /** @brief Depth of this transposition table entry */
  int _depth;

  /** @brief Type flag of this transposition table entry */
  Flag _flag;

  /** @brief Best move of this transposition table entry */
  Move _bestMove;
};

#endif
