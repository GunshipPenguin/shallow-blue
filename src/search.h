#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "defs.h"
#include "movegen.h"
#include "transptable.h"
#include "orderinginfo.h"

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
   * @return The score of the given board
   */
  int _negaMax(const Board&, int, int, int);

  /**
   * @brief OrderingInfo object containing information about the current state
   * of this search
   */
  OrderingInfo _orderingInfo;

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
   * @brief Logs info about a search according to the UCI protocol.
   *
   * @param pv        MoveList representing the Principal Variation (first moves at index 0)
   * @param depth     Depth of search
   * @param bestMove  Best move obtained from search
   * @param bestScore Score corresponding to the best move
   * @param nodes     Number of nodes searched
   */
  void _logUciInfo(const MoveList&, int, Move, int, int);

  /**
   * @brief Returns the principal variation for the last performed search.
   * 
   * Internally, this method probes the transposition table for the PV of the last
   * performed search.
   * 
   * @param length Length of the principal variation
   * @return MoveList The principal variation for the last performed search
   */
  MoveList _getPv(int);
};

#endif
