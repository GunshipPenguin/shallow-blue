#ifndef ATTACKTABLE_H
#define ATTACKTABLE_H

#include "defs.h"

/**
 * @brief A table of preinitialized piece attacks for every square and piece type
 */
class AttackTable {
public:
  /**
   * @brief Gets a bitboard containing all possible attacks for the specified non sliding piece.
   * 
   * @param pieceType Type of piece to get attacks for
   * @param square LERF square to get attacks for
   * @param color Color of piece (defaults to white, only has an effect when looking up pawn attacks)
   * @return A bitboard containing all possible attacks for the given pieceType, square and color 
   */
  static U64 getAttacks(PieceType, int, Color=WHITE);

  /**
   * @brief Initializes all non sliding piece attacks for later lookup.
   */
  static void init();
private:
  /**
   * @brief Fills all internal attack tables with empty bitboards.
   */
  static void _zeroTables();

  /**
   * @brief 3D array indexed by [color][pieceType][square] of non sliding piece attacks
   */
  static U64 ATTACKS[2][6][64];

  /**
   * @name Attack generation functions
   * @brief Calculates attacks for each piece and loads them into the internal attacks array.
   *
   * @{
   */
  static void _calcPawn();
  static void _calcKnight();
  static void _calcBishop();    
  static void _calcRook();
  static void _calcQueen();
  static void _calcKing();
  /**@}*/
};

#endif