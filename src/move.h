#ifndef CMOVE_H
#define CMOVE_H

#include "defs.h"
#include "board.h"
#include <string>
#include <vector>

/**
 * @brief Represents a move.
 */
class Move {
public:
  /**
   * @brief Construct a null move.
   */
  Move();

  /**
   * @brief Construct a move from and to the specified squares, and with the specified piece type and flags.
   *
   * @param int       From square (little endian rank file mapping)
   * @param int       To square (little endian rank file mapping)
   * @param PieceType Type of piece
   * @param int       Flags
   */
  Move(unsigned int, unsigned int, PieceType, unsigned int=0); // Non Null Move

  /**
   * @enum
   * @brief Move flags (indicate special moves)
   */
  enum Flag {
    NULL_MOVE = 1<<0,
    CAPTURE = 1<<1,
    DOUBLE_PAWN_PUSH = 1<<2,
    KSIDE_CASTLE = 1<<3,
    QSIDE_CASTLE = 1<<4,
    EN_PASSANT = 1<<5,
    PROMOTION = 1<<6
  };

  /**
   * @brief Returns an int containing this moves's flags.
   * @return [description]
   */
  unsigned int getFlags();

  /**
   * @brief Sets the specified flag on this move.
   * @param Flag Flag to set
   */
  void setFlag(Flag);

  /**
   * @brief Returns the piece type of this move.
   *
   * @return The type of piece being moved.
   */
  PieceType getPieceType();

  /**
   * @brief Returns The type of piece captured by this move (if it is a capture).
   *
   * Return value is undefined if move is not a capture.
   *
   * @return The type of piece captured by this move.
   */
  PieceType getCapturedPieceType();

  /**
   * @brief Set the type of piece captured by this move.
   *
   * Note that the CAPTURE flag should be set if this method is to be called.
   *
   * @param PieceType The type of captured piece to set
   */
  void setCapturedPieceType(PieceType);

  /**
   * @brief Returns this move's promotion piece type.
   *
   * Return value is undefined if move is not a promotion.
   *
   * @return the type of piece that this move promotes to.
   */
  PieceType getPromotionPieceType();

  /**
   * @brief Set the type of piece that this move promotes to.
   *
   * Note that the PROMOTION flag should be set if this method is to be called.
   *
   * @param PieceType The type of promotion piece to set
   */
  void setPromotionPieceType(PieceType);

  /**
   * @brief Return the square that this move is from.
   *
   * Returned value is is 0 indexed little endian rank-file mapping.
   *
   * @return The square that this move is from.
   */
  unsigned int getFrom();

  /**
   * @brief Return the square that this move is to.
   *
   * Returned value is is 0 indexed little endian rank-file mapping.
   *
   * @return The square that this move is to.
   */
  unsigned int getTo();

  /**
   * @brief Return a UCI compliant string representation of this move.
   * @return A UCI compliant string representation of this move.
   */
  std::string getNotation();

  /**
   * @brief Returns the index of a square on the chess board given its algebraic notation.
   *
   * Eg. notationToIndex("g1") would return 6
   *
   * @param  std::string The algebraic notation of the square on the chess board to get the index for.
   * @return             The index of the square (little endian rank-file mapping)
   */
  static unsigned int notationToIndex(std::string);

  /**
   * @brief Returns the algebraic notation of a square on the chess board given its index.
   *
   * Eg. indexToNotation(6) would return "g1"
   *
   * @param  int Index to get algebraic notation for
   * @return Algebraic notation of the square at the given index.
   */
  static std::string indexToNotation(int);

private:
  // Format of _move (28 bits total):
  // MSB  |----6----|---5---|---4---|-3--|-2--|-1--|  LSB
  //      28        21      15      9    6    3    0
  // 6 - Flags (7 bits)
  // 5 - To square (6 bits)
  // 4 - From square (6 bits)
  // 3 - Capture piece type (if applicable) (3 bits)
  // 2 - Promotion piece type (if applicable) (3 bits)
  // 1 - Piece type (3 bits)

  /**
   * @brief Move data
   *
   * Moves are stored as a packed integer consisting of 28 bits total.
   */
  unsigned int _move;

  /**
   * @name Rank and file characters in algebraic notation
   *
   * @{
   */
  const static char RANKS[];
  const static char FILES[];
  /**@}*/
};

#endif
