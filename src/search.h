#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "defs.h"
#include "movegen.h"
#include "transptable.h"
#include <limits>

/**
 * @brief Represents a search through a minmax tree.
 *
 * Searches are performed using recursive alpha-beta search, followed by a
 * quiescence search. The Search class contains no iterative deepening
 * functionality, that must be implemented by the caller via multiple calls
 * to perform().
 *
 */
class Search {
public:
  /**
   * @brief Constructs a new Search for the given board.
   *
   * @param board The board to search
   * @param logUci If logUci is set, UCI info commands about the search will be printed
   * to standard output in real time.
   */
  Search(const Board&, bool=true);

  /**
   * @brief Performs a search to the given depth.
   *
   * @param depth Depth to search to
   */
  void perform(int);

  /**
   * @brief Returns the best move obtained through the last search performed.
   * @return The best move obtained through the last search
   */
  Move getBestMove();

  /**
   * @brief Returns the best score obtained through the last search performed.
   * @return The best score obtained through the last search performed.
   */
  int getBestScore();

private:
  /** @brief Positive infinity to be used during search (eg. as a return value for winning) */
  static const int INF = std::numeric_limits<int>::max();

  /**
   * @brief Principal variation of the last search performed.
   */
  MoveList _pv;

  /**
   * @brief Number of nodes searched in the last search.
   */
  int _nodes;

  /**
   * @brief Initial board being used in this search.
   */
  Board _board;

  /**
   * @brief True if UCI output should be logged.
   */
  bool _logUci;

  /**
   * @brief Transposition Table used while searching.
   */
  TranspTable _tt;

  /**
   * @brief Best move found on last search.
   */
  Move _bestMove;

  /**
   * @brief Score corresponding to _bestMove
   */
  int _bestScore;

  /**
   * @brief Root negamax function.
   *
   * Starts performing a search to the given depth using recursive minimax
   * with alpha-beta pruning.
   *
   * @param board Board to search through
   * @param depth Maximum depth to search to
   */
  void _rootMax(const Board&, int);

  /**
   * @brief Non root negamax function, should only be called by _rootMax()
   *
   *
   * @param  board Board to search
   * @param  depth Plys remaining to search
   * @param  alpha Alpha value
   * @param  beta  Beta value
   * @param  ppv   Reference to principal variation 1 ply up (passed recursively)
   * @return The score of the given board
   */
  int _negaMax(const Board&, int, int, int, MoveList&);

  /**
   * @brief Performs a quiescence search
   *
   * _qSearch only takes into account captures (checks, promotions are not
   * considered)
   *
   * @param  board Board to perform a quiescence search on
   * @param  alpha Alpha value
   * @param  beta  Beta value
   * @return The score of the given board
   */
  int _qSearch(const Board&, int=-INF, int=INF);

  /**
   * @brief Assign a value to each move in the MoveList for move ordering during search.
   *
   * This method assigns each move a value using Move::setValue. Values are
   * assigned according to the following:
   * - Transposition table information
   * - Capture values (MVV/LVA)
   * - Promotion values
   *
   * Moves without any scoreable quantities are assigned a value of negative infinity.
   *
   * @param board Board that moves in the given MoveList correspond to
   * @param moveList List of moves to score
   */
  void _scoreMoves(const Board&, MoveList&);

  /**
   * @brief Assign a value to each move in the MoveList for move ordering during quiescence search.
   *
   * This method assigns each move a value using Move::setValue. Values are
   * assigned according to Capture values (MVV/LVA)
   *
   * Non capture moves are assigned a value of negative infinity.
   *
   * @param moveList List of moves to score
   */
  void _scoreMovesQsearch(MoveList&);

  /**
   * @brief Compare the value of two moves.
   *
   * @param  a Move a to compare
   * @param  b Move b to compare
   * @return true if a has a higher score than b, false otherwise
   */
  bool _compareMovesValue(Move, Move);

  /**
   * @brief Sorts a movelist high to low by move value.
   *
   * @param moveList MoveList to sort
   */
  void _sortMovesByValue(MoveList&);

  /**
   * @brief Logs info about a search according to the UCI protocol.
   *
   * @param pv        MoveList representing the Principal Variation (first moves at index 0)
   * @param depth     Depth of search
   * @param bestMove  Best move obtained from search
   * @param bestScore Score corresponding to the best move
   * @param nodes     Number of nodes searched
   */
  void _logUciInfo(const MoveList&, int, Move, int, int);
};

#endif
