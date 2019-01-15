#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "defs.h"

/**
 * @brief Vector of move objects
 */
typedef std::vector<Move> MoveList;

/**
 * @brief Pseudo-legal move generator.
 */
class MoveGen {
 public:
  /**
   * @brief Constructs a new MoveGen and generates moves for the given board.
   *
   * @param board Board to generate moves for.
   */
  MoveGen(const Board &board);

  /**
   * @brief Constructs a new MoveGen for an empty board.
   */
  MoveGen();

  /**
   * @brief Sets the board for this MoveGen to the specified board and generates moves for it.
   *
   * @param board Board to set and generate moves for
   */
  void setBoard(const Board &board);

  /**
   * @brief Returns all pseudo-legal moves that have been generated for the current board.
   *
   * @return A MoveList of all pseudo-legal moves that have been generated for the current board.
   */
  MoveList getMoves();

  /**
   * @brief Returns the subset of pseudo-legal moves that are legal.
   *
   * @return A MoveList of all legal moves that have been generated for the current board.
   */
  MoveList getLegalMoves();

 private:
  /**
   * @brief A vector containing generated pseudo-legal moves
   */
  MoveList _moves;

  /**
   * @brief MoveList of legal moves.
   */
  MoveList _legalMoves;

  /**
   * @brief Size of _moves to pre-reserve before generating moves.
   *
   * At the current time 218 seems to be an upper bound on the maximum number
   * of moves from any one position.
   */
  static const int MOVELIST_RESERVE_SIZE = 218;

  /**
   * @brief Generates pseudo-legal moves for the active player of the given board
   *
   * Generated pseudo-legal moves are stored in the _moves vector.
   *
   * @param board Board to generate moves for
   */
  void _genMoves(const Board &board);

  /**
   * @brief Populates the _legalMoves vector with moves from _moves that are legal.
   *
   * @param board Board to check legality of moves with.
   */
  void _genLegalMoves(const Board &board);

  /**
   * @brief Convenience function to generate pawn promotions.
   *
   * Given the from square, to square, extra flags (if applicable) and a captured
   * piece type (if this is a capture promotion), generate the 4 possible promotion moves
   * (queen, rook, bishop, knight).
   *
   * @param from              From square (little endian rank-file mapping)
   * @param to                To square (little endian rank-file mapping)
   * @param flags             Extra move flags (if applicable)
   * @param capturedPieceType Captured piece type (if applicable)
   */
  void _genPawnPromotions(unsigned int, unsigned int, unsigned int= 0, PieceType= PAWN);

  /**
   * @name White/black move generation functions
   *
   * These functions generate all pseudo-legal moves for black and white.
   *
   * @{
   */
  void _genWhiteMoves(const Board &);
  void _genBlackMoves(const Board &);
  /**@}*/

  /**
   * @name Pseudo-legal move generation functions
   *
   * These functions generate pseudo-legal moves for their piece and color for the given board.
   *
   * @{
   */
  void _genWhitePawnMoves(const Board &);
  void _genBlackPawnMoves(const Board &);

  void _genWhiteKingMoves(const Board &);
  void _genBlackKingMoves(const Board &);

  void _genWhiteKnightMoves(const Board &);
  void _genBlackKnightMoves(const Board &);

  void _genWhiteBishopMoves(const Board &);
  void _genBlackBishopMoves(const Board &);

  void _genWhiteRookMoves(const Board &);
  void _genBlackRookMoves(const Board &);

  void _genWhiteQueenMoves(const Board &);
  void _genBlackQueenMoves(const Board &);
  /**@}*/

  /**
   * @name White pawn pseudo-legal move generation functions
   *
   * These functions generate the four types of pawn moves for white for a given board.
   * @{
   */
  void _genWhitePawnSingleMoves(const Board &);
  void _genWhitePawnDoubleMoves(const Board &);
  void _genWhitePawnRightAttacks(const Board &);
  void _genWhitePawnLeftAttacks(const Board &);
  /**@}*/

  /**
   * @name Black pawn pseudo-legal generation functions
   *
   * These functions generate the four types of pawn moves for black for a given board.
   *
   * @{
   */
  void _genBlackPawnSingleMoves(const Board &);
  void _genBlackPawnDoubleMoves(const Board &);
  void _genBlackPawnRightAttacks(const Board &);
  void _genBlackPawnLeftAttacks(const Board &);
  /**@}*/

  /**
   * @name Generalized pseudo-legal generation functions for all pieces execpt pawns
   *
   * Each of these functions take a board reference, a bitboard of pieces to
   * generate moves for and a bitboard of attackable pieces.
   *
   */
  void _genKingMoves(const Board &, U64, U64);
  void _genKnightMoves(const Board &, U64, U64);
  void _genBishopMoves(const Board &, U64, U64);
  void _genRookMoves(const Board &, U64, U64);
  void _genQueenMoves(const Board &, U64, U64);
  /**@}*/

  /**
   * @brief Convenience function to add moves from a bitboard of generated moves.
   *
   * Given a board, a from square, a PieceType, a bitboard containing generated
   * moves and a bitboard containing attackable pieces, generate all possible Move
   * objects, and add them to the vector of pseudo-legal moves.
   *
   * @param board      Board to generate moves for
   * @param from       Originating square of moves
   * @param pieceType  Type of piece that is moving
   * @param moves      Bitboard containing possible destination squares
   * @param attackable Bitboard containing attackable pieces for this move
   */
  void _addMoves(const Board &, int, PieceType, U64, U64);
};

#endif
