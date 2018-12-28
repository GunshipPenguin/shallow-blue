#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include "psquaretable.h"
#include "zkey.h"
#include "move.h"
#include <string>

class Move;

/**
 * @brief Represents a chess board.
 *
 * Chess boards are represented using bitboards (of U64 type). Internally, each
 * Board maintains 12 bitboards (one for each piece type and each color), as well
 * as bitboards containing the occupancy, inverse of the occupancy and en passant
 * target square.
 *
 */
class Board {
 public:
  /**
   * @brief Constructs a new board set to the starting position
   */
  Board();

  /**
   * @brief Constructs a new board set to the specified fen string
   *
   * @param fen Fen string to set the board to
   */
  Board(std::string);

  /**
   * @brief Returns a pretty human readable string representation of this board.
   *
   * White pieces are represented as upper case letters and black pieces are
   * represented as lower case letters. Standard piece symbols are used
   * (pnbrqk) and empty squares are represented with dots.
   *
   * The string is returned with square a1 in the bottom left hand corner.
   *
   * @return A pretty human readable string representation of this board.
   */
  std::string getStringRep() const;

  /**
   * @brief Sets this board to the starting position
   */
  void setToStartPos();

  /**
   * @brief Sets this board to the specified FEN string.
   *
   * @param fenString FEN string to set the board to.
   */
  void setToFen(std::string);

  /**
   * @brief Performs the specified move on this board.
   *
   * @param move Move to perform on the board.
   */
  void doMove(Move);

  /**
   * @brief Returns true if white can castle kingside, false otherwise.
   *
   * @return true if white can castle kingside, false otherwise.
   */
  bool whiteCanCastleKs() const;

  /**
   * @brief Returns true if white can castle queenside, false otherwise.
   *
   * @return true if white can castle queenside, false otherwise.
   */
  bool whiteCanCastleQs() const;

  /**
   * @brief Returns true if black can castle kingside, false otherwise.
   *
   * @return true if black can castle queenside, false otherwise.
   */
  bool blackCanCastleKs() const;

  /**
   * @brief Returns true if black can castle queenside, false otherwise.
   *
   * @return true if black can castle queenside, false otherwise.
   */
  bool blackCanCastleQs() const;

  /**
    * @name Methods to query castling rights
    * @brief Returns true/false if the given color has the castling right
    *
    * These methods return true if the provided color has the given castling
    * right. Note that these return values represent potential future
    * castling and don't necessarily indicate that white/black can castle
    * immediately.
    *
    * For example, getKsCastlingRight(WHITE) returning true implies that
    * white has the right to castle given that white is not in check,
    * there are no pieces between the white king and white kingside rook
    * and none of the squares between the white king and white kingside
    * rook are under attack.
    *
    * Phrased another way, getKsCastlingRight(WHITE) returning true means
    * that neither the white king nor the white kingside rook have yet moved.
    *
    * @{
    */
  bool getKsCastlingRights(Color) const;
  bool getQsCastlingRights(Color) const;
  /**@}*/

  /**
   * @brief Returns true if the given color is in check, false otherwise.
   *
   * @param color Color to check for being in check
   * @return true if the given color is in check, false otherwise.
   */
  bool colorIsInCheck(Color) const;

  /**
   * @brief Gets the number of halfmoves since the last capture or pawn move
   *
   * @return int the number of halfmoves since the last capture or pawn move
   */
  int getHalfmoveClock() const;

  /**
   * @brief Returns a bitboard containing all of the pieces of the specified
   * color and piece type.
   *
   * @param  color color of pieces to return
   * @param  pieceType Type of pieces to return
   * @return A bitboard containing all of the pieces of the specified color and piece type.
   */
  U64 getPieces(Color, PieceType) const;

  /**
   * @brief Returns a bitboard containing all of the pieces of the specified color.

   * @param  color Color to return piece bitboard for.
   * @return A bitboard containing all of the pieces of the specified color.
   */
  U64 getAllPieces(Color) const;

  /**
   * @brief Returns a bitboard containing all of the pieces of the specified color that are attackable.
   *
   * Attackable pieces are any piece that is not a king.
   *
   * @param  color Color to return attackable piece bitboard for.
   * @return A bitboard containing all of the pieces of the specified color that are attackable.
   */
  U64 getAttackable(Color) const;

  /**
   * @brief Returns the type of the piece at the given square. Color must be provided.
   *
   * Throws an std::logic_error if no piece of the given color exists at the square.
   *
   * @param  color        Color of piece to lookup type.
   * @param  squareIndex  Little endian rank file index of square to lookup.
   * @return The PieceType at the specified square.
   */
  PieceType getPieceAtSquare(Color, int) const;

  /**
   * @brief Returns a bitboard containing all of the occupied squares on this board.
   *
   * @return A bitboard containing all occupied squares on this board.
   */
  U64 getOccupied() const;

  /**
   * @brief Returns a bitboard containing all of the unoccpuied squares on this board.
   *
   * @return A bitboard containing all of the unoccpuied squares on this board.
   */
  U64 getNotOccupied() const;

  /**
   * @brief Returns a bitboard containing the en passant target square of this board.
   *
   * This method will return an empty bitboard if there is no en passant target square.
   *
   * @return A bitboard containing the en passant target square of this board.
   */
  U64 getEnPassant() const;

  /**
   * @brief Returns the Zobrist Key of this board for its current state.
   *
   * @return The Zobrist Key of this board for its current state.
   */
  ZKey getZKey() const;

  /**
   * @brief Returns a Zobrist Key for this board that only takes into account
   * pawn positions
   *
   * @return A Zobrist Key for this board that only takes into account pawn
   * positions
   */
  ZKey getPawnStructureZKey() const;

  /**
   * @brief Returns the Piece Square Table of this board for its current state.
   *
   * @return The Piece Square Table of this board for its current state.
   */
  PSquareTable getPSquareTable() const;

  /**
   * @brief Returns the color whose turn it is to move.
   *
   * @return The color whose turn it is to move.
   */
  Color getActivePlayer() const;

  /**
   * @brief Returns the opposite of the color whose turn it is to move.
   *
   * @return The opposite of the color whose turn it is to move.
   */
  Color getInactivePlayer() const;

  /**
   * @brief Returns a bitboard containing all of the squares that a piece of the specified
   * piece type and color could move to if it was on the specified square.
   *
   * @param  pieceType The piece type to lookup move locations for
   * @param  color     The color of the piece to lookup move locations for.
   * @param  square       The little endian rank file mapping of the destination square.
   * @return           A bitboard containing all of the squares that a piece of the specified piece type and color could move to.
   */
  U64 getAttacksForSquare(PieceType, Color, int) const;

 private:
  /**
   * @name Attack bitboard generation functions.
   * @brief Generate attack bitboards for specified squares.
   *
   * These methods generate attack bitboards given specific squares and bitboards
   * of friendly pieces (with the exception of the 2 pawn methods which have
   * to be special cased).
   * @{
   */
  U64 _getWhitePawnAttacksForSquare(int) const;
  U64 _getBlackPawnAttacksForSquare(int) const;
  U64 _getKingAttacksForSquare(int, U64) const;
  U64 _getKnightAttacksForSquare(int, U64) const;
  U64 _getBishopAttacksForSquare(int, U64) const;
  U64 _getRookAttacksForSquare(int, U64) const;
  U64 _getQueenAttacksForSquare(int, U64) const;
  /**@}*/

  /**
   * @brief Array indexed by [color][piecetype] of piece bitboards
   */
  U64 _pieces[2][6];

  /**
   * @brief Array indexed by [color] of bitboards containing all pieces of each color.
   */
  U64 _allPieces[2];

  /**
   * @brief Bitboard containing all occupied squares.
   */
  U64 _occupied;

  /**
   * @brief Bitboard containing the en passant target square.
   */
  U64 _enPassant;

  /**
   * @brief Player whose turn it is to move.
   */
  Color _activePlayer = WHITE;

  /**
   * @brief Zobrist key for this board in its current state.
   */
  ZKey _zKey;

  /**
   * @brief Zobrist key taking into account only pawn positions
   */
  ZKey _pawnStructureZkey;

 private:

  /**
   * @brief Piece Square table for this board in its current state.
   */
  PSquareTable _pst;

  /**
   * @brief Halfmove clock, used to determine draws by the 50 move rule
   */
  int _halfmoveClock;

  /**
   * @brief Castling rights
   *
   * Stored as 4 bits:
   * - Bit 0 - White kingside
   * - Bit 1 - White queenside
   * - Bit 2 - Black kingside
   * - Bit 3 - Black queenside
   */
  unsigned char _castlingRights;


  /**
   * @brief Determines if the given square is under attack by the given color.
   *
   * Returns true if the square (given as a little endian rank file map index)
   * is under attack by the given color.
   *
   * @param  color        Color to check
   * @param  squareIndex  Square to check (little endian rank file mapping)
   * @return True if the square is under attack, false otherwise
   */
  bool _squareUnderAttack(Color, int) const;

  /**
   * @brief Update the castling rights for the given move.
   *
   * This method updates the correct castling rights if a rook is captured,
   * moves, or the king moves.
   *
   * @param move The move to update castling rights for.
   */
  void _updateCastlingRightsForMove(Move);

  /**
   * @brief Updates the _occupied and _allPieces bitboards based on the _pieces bitboards.
   */
  void _updateNonPieceBitBoards();

  /**
   * @brief Moves a piece between the given squares.
   *
   * Moves the piece from the from square to the to square and updates the
   * board state appropriately. Note that this function is more efficient than
   * calling _addPiece() and _removePiece() separately.
   *
   * @param color     Color of piece to move
   * @param pieceType Type of piece to move
   * @param from      Square to move from (little endian rank file mapping)
   * @param to        Square to move to (little endian rank file mapping)
   */
  void _movePiece(Color, PieceType, int, int);

  /**
   * @brief Adds a piece of the specified color and piece type at the given square.
   *
   * Adds a piece of the given piece type and color to the given square and
   * updates the board state appropriately.
   *
   * @param color     Color of piece to add
   * @param pieceType Type of piece to add
   * @param squareIndex    Square to add piece at (little endian rank file mapping)
   */
  void _addPiece(Color, PieceType, int);

  /**
   * @brief Removes a piece of the specified color and piece type from the given square.
   *
   * Removes a piece of the given piece type and color from the given square and
   * updates the board state appropriately.
   *
   * @param color     Color of piece to remove
   * @param pieceType Type of piece to remove
   * @param squareIndex    Square to remoe piece from (little endian rank file mapping)
   */
  void _removePiece(Color, PieceType, int);

  /**
   * @brief Clears all bitboards.
   */
  void _clearBitBoards();
};

#endif
