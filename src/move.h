#ifndef MOVE_H
#define MOVE_H

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
   * @param from  From square (little endian rank file mapping)
   * @param to    To square (little endian rank file mapping)
   * @param piece Type of piece
   * @param flags Flags
   */
  Move(unsigned int, unsigned int, PieceType, unsigned int= 0); // Non Null Move

  /**
   * @enum Flag
   * @brief Flags that indicate special moves.
   */
  enum Flag {
    NULL_MOVE = 1 << 0, /**< The move is a null move */
    CAPTURE = 1 << 1, /**< The move is a capture (Captured piece type must be set with setCapturedPieceType()) */
    DOUBLE_PAWN_PUSH = 1 << 2, /**< The move is a double pawn push */
    KSIDE_CASTLE = 1 << 3, /**< The move is a kingisde castle */
    QSIDE_CASTLE = 1 << 4, /**< The move is a queenside castle */
    EN_PASSANT = 1 << 5, /**< The move is an en passant capture (Do not set the CAPTURE flag additionally) */
    PROMOTION = 1 << 6 /**< The move is a promotion (Promotion piece type must be set with setPromotionPieceType()) */
  };

  /**
   * @brief Returns an int containing this moves's flags.
   * @return [description]
   */
  unsigned int getFlags() const;

  /**
   * @brief Sets the specified flag on this move.
   * @param flag Flag to set
   */
  void setFlag(Flag);

  /**
   * @brief Returns the piece type of this move.
   *
   * @return The type of piece being moved.
   */
  PieceType getPieceType() const;

  /**
   * @brief Returns The type of piece captured by this move (if it is a capture).
   *
   * Return value is undefined if move is not a capture.
   *
   * @return The type of piece captured by this move.
   */
  PieceType getCapturedPieceType() const;

  /**
   * @brief Set the type of piece captured by this move.
   *
   * Note that the CAPTURE flag should be set if this method is to be called.
   *
   * @param pieceType The type of captured piece to set
   */
  void setCapturedPieceType(PieceType);

  /**
   * @brief Returns this move's promotion piece type.
   *
   * Return value is undefined if move is not a promotion.
   *
   * @return the type of piece that this move promotes to.
   */
  PieceType getPromotionPieceType() const;

  /**
   * @brief Set the type of piece that this move promotes to.
   *
   * Note that the PROMOTION flag should be set if this method is to be called.
   *
   * @param pieceType The type of promotion piece to set
   */
  void setPromotionPieceType(PieceType);

  /**
   * @brief Get the value of this move
   *
   * @return The value of this move
   */
  int getValue();

  /**
   * @brief Set the value of this move
   *
   * @param value Value to set this move's value to
   */
  void setValue(int);

  /**
   * @brief Compare moves
   *
   * @return true if the moves are exactly the same, false otherwise
   */
  bool operator==(Move) const;

  /**
   * @brief Return the square that this move is from.
   *
   * Returned value is is 0 indexed little endian rank-file mapping.
   *
   * @return The square that this move is from.
   */
  unsigned int getFrom() const;

  /**
   * @brief Return the square that this move is to.
   *
   * Returned value is is 0 indexed little endian rank-file mapping.
   *
   * @return The square that this move is to.
   */
  unsigned int getTo() const;

  /**
   * @brief Return a UCI compliant string representation of this move.
   * @return A UCI compliant string representation of this move.
   */
  std::string getNotation() const;

  /**
   * @brief Returns the index of a square on the chess board given its algebraic notation.
   *
   * Eg. notationToIndex("g1") would return 6
   *
   * @param  notation The algebraic notation of the square on the chess board to get the index for.
   * @return The index of the square (little endian rank-file mapping)
   */
  static unsigned int notationToIndex(std::string);

  /**
   * @brief Returns the algebraic notation of a square on the chess board given its index.
   *
   * Eg. indexToNotation(6) would return "g1"
   *
   * @param  index Index to get algebraic notation for
   * @return Algebraic notation of the square at the given index.
   */
  static std::string indexToNotation(int);

 private:
  /**
   * @brief A packed integer containing all of the move data.
   *
   * 28 bits are used in total to store move information. The format is as
   * follows:
   *
   * @code
   * MSB  |----6----|---5---|---4---|-3--|-2--|-1--|  LSB
   *      28        21      15      9    6    3    0
   * @endcode
   *
   * - 6 - Flags (7 bits)
   * - 5 - To square (6 bits)
   * - 4 - From square (6 bits)
   * - 3 - Captured PieceType (if applicable) (3 bits)
   * - 2 - Promotion PieceType (if applicable) (3 bits)
   * - 1 - PieceType of moving piece (3 bits)
   *
   * Moves are stored as a packed integer consisting of 28 bits total.
   */
  unsigned int _move;

  /** @brief Value of this move */
  int _value;

  /**
   * @name Rank and file characters in algebraic notation
   *
   * @{
   */
  const static char RANKS[];
  const static char FILES[];
  /**@}*/

  /** @brief Notation that should be used to represent a null move */
  const static std::string NULL_MOVE_NOTATION;
};

#endif
