#ifndef ZKEY_H
#define ZKEY_H

#include "defs.h"

class Board;

/**
 * @brief Represents a Zobrist key.
 *
 * Zobrist keys are represented as unsigned 64 bit integers (same as bitboards).
 */
class ZKey {
 public:
  /**
   * @brief Constructs a new empty ZKey.
   */
  ZKey();

  /**
   * @brief Constructs a new ZKey from the given board.
   * @param board Board to construct this ZKey from
   */
  ZKey(const Board &board);

  /**
   * @brief Initialize the pseudo-random values used to generate a ZKey.
   *
   * This must be called once prior to using the ZKey class.
   */
  static void init();

  /**
   * @brief Returns The value of this ZKey.
   *
   * @return The value of this ZKey
   */
  U64 getValue() const;

  /**
   * @brief Updates the ZKey such that the given piece has been moved between the given squares.
   *
   * @param color  Color of piece to move
   * @param piece  Type of piece to move
   * @param from   Square to move from
   * @param to     Square to move to
   */
  void movePiece(Color, PieceType, unsigned int, unsigned int);

  /**
   * @brief Flips the value of the piece with the given color and PieceType on the given square in the ZKey.
   *
   * If the piece is set, this method will unset it and vica-versa.
   *
   * @param color  Color of piece to flip
   * @param piece  Type of piece to flip
   * @param index  Square to flip piece on (little endian rank-file mapping)
   */
  void flipPiece(Color, PieceType, unsigned int);

  /**
   * @brief Flips the value of the active player in the ZKey
   *
   * If the active color is black, this method changes it to white, and vica-versa.
   */
  void flipActivePlayer();

  /**
   * @brief Clears all en passant information in the ZKey.
   */
  void clearEnPassant();

  /**
   * @brief Sets the en passant file to the given file.
   *
   * File numbers are 0 indexed and start at a.
   *
   * @param file file number to set en passant file to
   */
  void setEnPassantFile(unsigned int);

  /**
   * @brief Updates the castling rights in the ZKey.
   *
   * @param whiteKs White kingside castling right
   * @param whiteQs White queenside castling right
   * @param blackKs Black kingside castling right
   * @param blackQs Black queenside castling right
   */
  void updateCastlingRights(bool, bool, bool, bool);

  /**
   * @brief Sets this ZKey from the given board, taking into account only pawn
   * positions
   */
  void setFromPawnStructure(const Board&);

  /**
   * @brief Compare Zobrist keys 
   * 
   * @param other ZKey to compare
   * @return true if the two ZKeys have the same value, false otherwise
   */
  bool operator==(const ZKey &);

 private:
  /**
   * @brief The current value of the Zobrist key.
   */
  U64 _key;

  /**
   * @name Functions that flip castling rights for each color.
   *
   * @{
   */
  void _flipKsCastle(Color);
  void _flipQsCastle(Color);
  /**@}*/

  /**
   * @brief Current en passant file
   */
  int _enPassantFile;

  /**
   * @name Current castling rights
   *
   * @{
   */
  bool _whiteKs;
  bool _whiteQs;
  bool _blackKs;
  bool _blackQs;
  /**@}*/

  /**
   * @brief Array indexed by [Color][PieceType][SquareIndex] of pseudo-random
   * values to xor into _key for each color, piece type and square.
   */
  static U64 PIECE_KEYS[2][6][64];

  /**
   * @brief Pseudo-random keys to xor into _key for each en passant file
   */
  static U64 EN_PASSANT_KEYS[8];

  /**
   * @name King and queenside castling keys indexed by [Color]
   *
   * @{
   */
  static U64 KS_CASTLE_KEYS[2];
  static U64 QS_CASTLE_KEYS[2];
  /**@}*/

  /**
   * @brief Key to xor into _key when it's white's turn to move.
   */
  static U64 WHITE_TO_MOVE_KEY;

  /**
   * @brief Seed used by the PRNG.
   */
  static const unsigned int PRNG_KEY;
};

#endif
