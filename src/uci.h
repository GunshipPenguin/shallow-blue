#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "move.h"
#include <sstream>
#include <fstream>

/**
 * @brief Class for handling UCI input/output.
 */
class Uci {
public:
  /**
   * @brief Starts listening for UCI input and responds to it when received.
   */
  void start();

private:
  /**
   * @brief Current board being used.
   */
  Board _board;

  /**
   * @brief Default depth to search to upon receiving the go command.
   */
  static const int DEFAULT_DEPTH = 4;

  /**
   * @brief Handles the ucinewgame command
   *
   * Sets the board to the starting position.
   */
  void _uciNewGame();

  /**
   * @brief Handles the position command
   *
   * The passed istringstream should contain the received line with everything following the `position` command.
   *
   * @param is istringstream contining position information
   */
  void _setPosition(std::istringstream&);

  /**
   * @brief Handles the go command
   *
   * The passed istringstream should contain the received line with everything following the `go` command
   *
   * @param is istringstream containing go parameters
   */
  void _go(std::istringstream&);

  /**
   * @brief Performs a search to the given depth and outputs the best move.
   * @param maxDepth Depth to search to
   */
  void _pickBestMove(int);

  /**
   * @brief Calculates perft for each legal move of the current board and prints each perft value.
   *
   * @param depth Depth to run perft to
   */
  void _perftDivide(int);

  /**
   * @brief Returns the perft value for the given board to the given depth.
   *
   * @param  board Board to calculate perft of
   * @param  depth   Depth to search to
   * @return The perft of the board for the given depth
   */
  unsigned long long _perft(const Board&, int);
};

#endif
