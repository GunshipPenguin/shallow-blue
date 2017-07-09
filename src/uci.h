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
   * @brief Start listening for UCI input and respond to it when received.
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
   * @brief Handle the ucinewgame command
   *
   * Sets the board to the starting position.
   */
  void _uciNewGame();

  /**
   * @brief Handle the position command
   *
   * The passed istringstream should contain the received line with everything following the `position` command.
   *
   * @param std::istringstream istringstream contining position information
   */
  void _setPosition(std::istringstream&);

  /**
   * @brief Handle the go command
   *
   * The passed istringstream should contain the received line with everything following the `go` command
   */
  void _go(std::istringstream&);

  /**
   * @brief Performs a search to the given depth and outputs the best move.
   * @param int Depth to search to
   */
  void _pickBestMove(int);
};

#endif
