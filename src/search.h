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
  static const int INF = std::numeric_limits<int>::max();

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
   * @brief Orders a MoveBoardList for negamax search.
   *
   * Move ordering is as follows:
   * 1. Best moves from Transposition Table (PV is implicitly placed first by this)
   * 2. Captures sorted by MVV/LVA
   * 3. Promotions sorted by value
   * 4. All other moves
   *
   * @param moveBoardList MoveBoardList to sort
   */
  void _orderMoves(MoveBoardList&);

  /**
   * @brief Orders a MoveBoardList for quiescence search
   *
   * Places captures first sorted by MVV/LVA and non captures after.
   *
   * @param moveBoardList MoveBoardList to sort
   */
  void _orderMovesQSearch(MoveBoardList&);

  /**
   * @brief Transposition table comparison function for moves
   *
   * Returns true if a is a worse better than b according to the transposition table.
   *
   * If a or b are not in the transposition table, returns false or true respectively.
   * If a and b are both not in the transposition table, returns false.
   *
   * @param  a First MoveBoard to compare
   * @param  b Second MoveBoard to compare
   * @return true if a is a better move than b according to the transposition table.
   */
  bool _compareMovesTt(MoveBoard, MoveBoard);

  /**
   * @brief MVV/LVA comparison function for moves
   *
   * Returns true if a is a better move than b according to MVV/LVA.
   *
   * @param  a First MoveBoard to compare
   * @param  b Second MoveBoard to compare
   * @return true if a is a better move than b according to MVV/LVA, false otherwise
   */
  bool _compareMovesMvvLva(MoveBoard, MoveBoard);

  /**
   * @brief Promotion comparison function for moves.
   *
   * Returns true if a is a better promotion than b according to the value of the
   * promotion piece.
   *
   * @param  a First moveboard to compare
   * @param  b Second moveboard to compare.
   * @return true if a is a better move than b according to promotion piece value.
   */
  bool _compareMovesPromotionValue(MoveBoard, MoveBoard);

  /**
   * @brief Logs info about a search according to the UCI protocol.
   *
   * @param pv        MoveBoardList representing the Principal Variation (first moves at index 0)
   * @param depth     Depth of search
   * @param bestMove  Best move obtained from search
   * @param bestScore Score corresponding to the best move
   */
  void _logUciInfo(const MoveBoardList&, int, Move, int);

  /**
   * @brief Returns the value of a pieceType that can be used for comparisons.
   *
   * @param  pieceType PieceType to get value for
   * @return Value of the given piece type.
   */
  int _getPieceValue(PieceType);

  /**
   * @brief Extracts and returns the principal variation of the last search from the transposition table
   *
   * Can only be called after a search has been called via _rootMax().
   *
   * @param  board Initial board used in the last search
   * @param  depth Depth of the last search
   * @return       MoveBoardList representing the Principal Variation of the last search
   */
  MoveBoardList _getPv(const Board&, int);
};

#endif
