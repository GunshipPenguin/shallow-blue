#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include "raytable.h"
#include "psquaretable.h"
#include "zkey.h"
#include "move.h"
#include <string>

class Move;

/**
 * @brief Represents a chess board.
 *
 */
class Board {
  public:
    /**
     * @brief Construct a new empty chess board.
     */
    Board();

    /**
     * @brief Returns a string representation of the chess board.
     * @return A string representation of the chess board.
     */
    std::string getStringRep() const;

    /**
     * @brief Set the chess board to the starting position
     */
    void setToStartPos();

    /**
     * @brief Set the chess board to the specified FEN string.
     * @param std::string FEN string to set the board to.
     */
    void setToFen(std::string);

    /**
     * @brief Perform the specified move on the board.
     * @param Move Move to perform on the board.
     */
    void doMove(Move);

    /**
     * @brief Returns true if white can castle kingside, false otherwise.
     * @return true if white can castle kingside, false otherwise.
     */
    bool whiteCanCastleKs() const;

    /**
     * @brief Returns true if white can castle queenside, false otherwise.
     * @return true if white can castle queenside, false otherwise.
     */
    bool whiteCanCastleQs() const;

    /**
     * @brief Returns true if black can castle kingside, false otherwise.
     * @return true if black can castle queenside, false otherwise.
     */
    bool blackCanCastleKs() const;

    /**
     * @brief Returns true if black can castle queenside, false otherwise.
     * @return true if black can castle queenside, false otherwise.
     */
    bool blackCanCastleQs() const;

    /**
     * @brief Returns true if white has kingside castling rights, false otherwise.
     *
     * This method returning true does not imply that white can castle kingside,
     * A return of true from this method means that white has the right to castle
     * kingside provided it is a legal move (ie. king and kingside rook have
     * not moved)
     *
     * @return True if white has a kingside castling right, false otherwise.
     */
    bool whiteKsCastlingRight() const;

    /**
     * @brief Returns true if white has queenside castling rights, false otherwise.
     *
     * This method returning true does not imply that white can castle queenside,
     * A return of true from this method means that white has the right to castle
     * queenside provided it is a legal move (ie. king and queenside rook have
     * not moved)
     *
     * @return True if white has a queenside castling right, false otherwise.
     */
    bool whiteQsCastlingRight() const;

    /**
     * @brief Returns true if black has kingside castling rights, false otherwise.
     *
     * This method returning true does not imply that black can castle kingside,
     * A return of true from this method means that black has the right to castle
     * kingside provided it is a legal move (ie. king and kingside rook have
     * not moved)
     *
     * @return True if white has a queenside castling right, false otherwise.
     */
    bool blackKsCastlingRight() const;

    /**
     * @brief Returns true if black has queenside castling rights, false otherwise.
     *
     * This method returning true does not imply that black can castle queenside,
     * A return of true from this method means that black has the right to castle
     * queenside provided it is a legal move (ie. king and queenside rook have
     * not moved)
     *
     * @return True if white has a queenside castling right, false otherwise.
     */
    bool blackQsCastlingRight() const;

    /**
     * @brief Returns true if the given color is in check, false otherwise.
     *
     * @param Color Color to check for being in check
     * @return true if the given color is in check, false otherwise.
     */
    bool colorIsInCheck(Color) const;

    /**
     * @brief Returns a bitboard containing all of the pieces of the specified
     * color and piece type.
     *
     * @param  Color color of pieces to return
     * @param  PieceType Type of pieces to return
     * @return A bitboard containing all of the pieces of the specified color and piece type.
     */
    U64 getPieces(Color, PieceType) const;

    /**
     * @brief Returns a bitboard containing all of the pieces of the specified color.
     *
     * @param  Color Color to return piece bitboard for.
     * @return A bitboard containing all of the pieces of the specified color.
     */
    U64 getAllPieces(Color) const;

    /**
     * @brief Returns a bitboard containing all of the pieces of the specified color that are attackable.
     * asdfadfef
     * Attackable pieces are any piece that is not a king.
     *
     * @param  Color Color to return attackable piece bitboard for.
     * @return A bitboard containing all of the pieces of the specified color that are attackable.
     */
    U64 getAttackable(Color) const;

    /**
     * @brief Returns the type of the piece at the given sqare. Color must be provided.
     *
     * @param  Color Color of piece to lookup type.
     * @param  int Little endian rank file index of square to lookup.
     * @return The PieceType at the specified square.
     */
    PieceType getPieceAtSquare(Color, int) const;

    /**
     * @brief Returns a bitboard containing all of the occupied squares on the chess board.
     *
     * @return A bitboard containing all occupied squares on the chess board.
     */
    U64 getOccupied() const;

    /**
     * @brief Returns a bitboard containing all of the unoccpuied squares on the chess board.
     * @return A bitboard containing all of the unoccpuied squares on the chess board.
     */
    U64 getNotOccupied() const;

    /**
     * @brief Returns a bitboard containing the En Passant target square of the chess board.
     *
     * This method will return an empty bitboard if there is no En Passant target square.
     * @return A bitboard containing the En Passant target square of the chess board.
     */
    U64 getEnPassant() const;

    /**
     *
     * @brief Returns the Zobrist Key of the chess board for its current state.
     *
     * @return The Zobrist Key of the chess board for its current state.
     */
    ZKey getZKey() const;

    /**
     * @brief Returns the Piece Square Table of the chess board for its current state.
     *
     * @return The Piece Square Table of the chess board for its current state.
     */
    PSquareTable getPSquareTable() const;

    /**
     * @brief Returns the color whose turn it is to move.
     *
     * @return The color whose turn it is to move.
     */
    Color getActivePlayer() const;

    /**
     * @brief returns the opposite of the color whose turn it is to move.
     *
     * @return The opposite of the color whose turn it is to move.
     */
    Color getInactivePlayer() const;

    /**
     * @brief Returns a bitboard containing all of the squares that a piece of the specified
     * piece type and color could move to if it was on the specified square.
     *
     * @param  PieceType The piece type to lookup move locations for
     * @param  Color     The color of the piece to lookup move locations for.
     * @param  int       The little endian rank file mapping of the destination square.
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
     * @brief Array indexed by [color] of bitboards containing all attackable pieces.
     */
    U64 _attackable[2];

    /**
     * @name Check information
     *
     * Calculating checks is an expensive process, so they are calculated on demand
     * and the result is cached until a move has been made.
     *
     * _checks is an array indexed by color indicating if a color is in check.
     *
     * _checksCalculated is an array indexed by color indicating if the information
     * in _checks has already been calculated and cached or needs to be calculated.
     * @{
     */
    mutable bool _checks[2];
    mutable bool _checksCalculated[2];
    /**@}*/

    /**
     * @brief Resets all check information.
     */
    void _resetChecks();

    /**
     * @brief Bitboard containing all occupied squares.
     */
    U64 _occupied;

    /**
     * @brief Bitboard containing all unoccupied squares.
     */
    U64 _notOccupied;

    /**
     * @brief Bitboard containing the en passant target square.
     */
    U64 _enPassant;

    /**
     * @brief Player whose turn it is to move.
     */
    Color _activePlayer;

    /**
     * @brief Zobrist key for the board in its current state.
     */
    ZKey _zKey;

    /**
     * @brief Piece Square table for the board in its current state.
     */
    PSquareTable _pst;

    /**
     * @brief Static RayTable object used to calculate sliding piece attacks.
     */
    static RayTable _raytable;

    /**
     * @name Castling right booleans.
     * @{
     */
    bool _whiteCanCastleKs;
    bool _whiteCanCastleQs;
    bool _blackCanCastleKs;
    bool _blackCanCastleQs;
    /**@}*/

    /**
     * @brief Determine if the given square is under attack by the given color.
     *
     * Returns true if the square (given as a little endian rank file map index)
     * is under attack by the given color.
     *
     * @param  Color Color to check
     * @param  int   Square to check (little endian rank file mapping)
     * @return True if the square is under attack, false otherwise
     */
    bool _squareUnderAttack(Color, int) const;

    /**
     * @brief Update the castling rights for the given move.
     *
     * This method updates the correct castling rights if a rook is captured,
     * moves, or the king moves.
     *
     * @param Move The move to update castling rights for.
     */
    void _updateCastlingRightsForMove(Move);

    /**
     * @brief Update the _occupied, _notOccupied and _allPieces bitboards based on the _pieces bitboards.
     */
    void _updateNonPieceBitBoards();

    /**
     * @brief Move a piece between the given squares.
     *
     * Moves the piece from the from square to the to square and updates the
     * board state appropriately.
     *
     * @param Color     Color of piece to move
     * @param PieceType Type of piece to move
     * @param int       Square to move from (little endian rank file mapping)
     * @param int       Square to move to (little endian rank file mapping)
     */
    void _movePiece(Color, PieceType, int, int);

    /**
     * @brief Add a piece of the specified color and piece type at the given square.
     *
     * Adds a piece of the given piece type and color to the given square and
     * updates the board state appropriately.
     *
     * @param Color     Color of piece to add
     * @param PieceType Type of piece to add
     * @param int       Square to add piece at (little endian rank file mapping)
     */
    void _addPiece(Color, PieceType, int);

    /**
     * @brief Remove a piece of the specified color and piece type from the given square.
     *
     * Removes a piece of the given piece type and color from the given square and
     * updates the board state appropriately.
     *
     * @param Color     Color of piece to remove
     * @param PieceType Type of piece to remove
     * @param int       Square to remoe piece from (little endian rank file mapping)
     */
    void _removePiece(Color, PieceType, int);

    /**
     * @brief Clears all bitboards.
     */
    void _clearBitBoards();
};

#endif
