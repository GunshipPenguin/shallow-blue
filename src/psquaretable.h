#ifndef PSQUARETABLE_H
#define PSQUARETABLE_H

#include "defs.h"
#include <vector>

class Board;

/**
 * @brief Represents a Piece Square Table.
 */
class PSquareTable {
 public:
  PSquareTable();

  /**
   * @brief Constructs a new piece square table from the given board.
   *
   * @param board Board to construct this PSquareTable for
   */
  PSquareTable(const Board&);

  /**
   * @brief Initializes PSquareTable square values.
   *
   * This must be called once prior to using a PSquareTable.
   */
  static void init();

  /**
   * @brief Adds a piece at the given square.
   *
   * @param color     Color of piece to add
   * @param pieceType Type of piece to add
   * @param square    Square to add piece at (little endian rank-file mapping)
   */
  void addPiece(Color, PieceType, unsigned int);

  /**
   * @brief Removes a piece from the given square.
   *
   * @param color     Color of piece to remove
   * @param pieceType Type of piece to remove
   * @param square    Square to remove piece from (little endian rank-file mapping)
   */
  void removePiece(Color, PieceType, unsigned int);

  /**
   * @brief Moves a piece between the given squares.
   *
   * @param color      Color of piece to add
   * @param pieceType  Type of piece to add
   * @param fromSquare Square to add piece at (little endian rank-file mapping)
   * @param toSquare   Square to move piece to (little endian rank-file mapping)
   */
  void movePiece(Color, PieceType, unsigned int, unsigned int);

  /**
   * @brief Gets the piece square table score of the given player in the
   * given phase
   *
   * @param phase GamePhase to get score
   * @param  color Color to get score for
   * @return The piece square table score for the given player
   */
  int getScore(GamePhase, Color);

 private:
  /**
   * @brief Array indexed by [GamePhase][Color][PieceType][SquareIndex] of square values for each  piece,
   * square and color.
   */
  static int PIECE_VALUES[2][2][6][64];

  /**
   * @brief Sets PIECE_VALUES for white and black for the given game phase
   *
   * Note that this function must be given the values for black and will set
   * the entry in PIECE_VALUES for white and black.
   *
   * @param list Square values for black.
   * @param pieceType Piece type to set values for.
   * @param phase GamePhase to set values for
   */
  static void _setValues(std::vector<int>, PieceType, GamePhase);

  /**
   * @brief Mirrors the list of square values along the x axis.
   *
   * @param  list Square values to mirror
   * @return Mirrored square values
   */
  static std::vector<int> _mirrorList(std::vector<int>);

  /**
   * @brief Array indexed by [GamePhase][Color] of each color's piece square table score.
   */
  int _scores[2][2] = {{0}};
};

#endif
