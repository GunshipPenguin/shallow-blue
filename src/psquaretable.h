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
  /**
   * @brief Construct a new empty piece square table.
   */
  PSquareTable();

  /**
   * @brief Construct a new piece square table from the given board.
   *
   * @param Board Board to construct PSquareTable for
   */
  PSquareTable(const Board&);

  /**
   * @brief Initialize PSquareTable square values.
   *
   * This must be called once prior to using a PSquareTable.
   */
  static void init();

  /**
   * @brief Add a piece at the given square.
   *
   * @param Color     Color of piece to add
   * @param PieceType Type of piece to add
   * @param int       Square to add piece at (little endian rank-file mapping)
   */
  void addPiece(Color, PieceType, unsigned int);

  /**
   * @brief Remove a piece from the given square.
   *
   * @param Color     Color of piece to remove
   * @param PieceType Type of piece to remove
   * @param int       Square to remove piece from (little endian rank-file mapping)
   */
  void removePiece(Color, PieceType, unsigned int);

  /**
   * @brief Move a piece between the given squares.
   *
   * @param Color     Color of piece to add
   * @param PieceType Type of piece to add
   * @param int       Square to add piece at (little endian rank-file mapping)
   */
  void movePiece(Color, PieceType, unsigned int, unsigned int);

  /**
   * @brief Get the piece square table score of the given player.
   *
   * @param  Color Color to get score for
   * @return       The piece square table score for the given player
   */
  int getScore(Color);

private:
  /**
   * @brief Array indexed by [Color][PieceType][SquareIndex] of square values for each  piece, square and color.
   */
  static int PIECE_VALUES[2][6][64];

  /**
   * @brief Set PIECE_VALUES for white and black.
   *
   * Note that this function must be given the values for black and will set
   * the entry in PIECE_VALUES for white and black.
   *
   * @param std::vector<int> Square values for black.
   * @param PieceType Piece type to set values for.
   */
  static void _setValues(std::vector<int>, PieceType);

  /**
   * @brief Mirror the list of square values along the x axis.
   *
   * @param  std::vector<int> Square values to mirror
   * @return Mirrored square values
   */
  static std::vector<int> _mirrorList(std::vector<int>);

  /**
   * @brief Array indexed by color of each color's piece square table score.
   */
  int _scores[2];
};

#endif
