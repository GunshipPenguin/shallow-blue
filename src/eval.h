#ifndef EVAL_H
#define EVAL_H

#include "defs.h"
#include "movegen.h"
#include "bitutils.h"

/**
 * @brief Namespace containing board evaluation functions
 */
namespace Eval {
namespace detail {
/**
 * @brief Array of files A-H as bitboards
 */
extern U64 FILES[8];

/**
 * @brief Array of the files neighboring each file A-H (as bitboards)
 */
extern U64 NEIGHBOR_FILES[8];

/**
 * @brief Bitboards of pawn shields for each color and position (used in kingPawnShieldSize())
 */
extern U64 PAWN_SHIELDS[2][64];
};

/**
  * @name Piece material value constants
  * @{
  */
const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 330;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;
/**@}*/

/**
 * @brief Bonus given to a player having a move avaliable
 */
const int MOBILITY_BONUS = 1;

/**
 * @brief Bonus given to a player for each rook on an open file
 */
const int ROOK_OPEN_FILE_BONUS = 15;

/**
 * @brief Penalty given to a player for having a doubled pawn
 */
const int DOUBLED_PAWN_PENALTY = -25;

/**
 * @brief Penalty given to a player for having an isolated pawn
 */
const int ISOLATED_PAWN_PENALTY = -30;

/**
 * @brief Penalty given to a player for having a backward pawn
 */
const int BACKWARD_PAWN_PENALTY = -20;

/**
 * @brief Bonus given to a player for having bishops on black and white squares
 */
const int BISHOP_PAIR_BONUS = 10;

/**
 * @brief Bonus given to a player for each pawn shielding their king
 */
const int KING_PAWN_SHIELD_BONUS = 7;

/**
 * @brief Initializes all inner constants used by functions in the Eval namespace
 */
void init();

/**
 * @brief Returns the value of the given PieceType used for evaluation purposes in centipawns
 *
 * @param pieceType PieceType to get value of
 * @return The value of the given PieceType used for evaluation purposes
 */
int getMaterialValue(PieceType);

/**
 * @brief Returns the evaluated advantage of the given color in centipawns
 *
 * @param board Board to evaluate
 * @param color Color to evaluate advantage of
 * @return Advantage of the given color in centipawns
 */
int evaluate(const Board &, Color);

/**
 * @brief Returns true if the given color has at least one bishop on black squares
 * and at least one bishop on white squares on the given board
 *
 * @param board Board to check bishops of
 * @param color Color to check for bishop pair
 * @return true if the given color has at least one bishop on a black square and
 * at least one bishop on a white square, false otherwise
 */
bool hasBishopPair(const Board &, Color);

/**
 * @brief Returns the number of pseudo-legal moves avaliable to the given color
 * on the given board (with some omissions)
 *
 * For the sake of speed, castles and promotions are not considered.
 *
 * @param board Board to use when generating moves
 * @param color Color to count pseudo-legal moves for
 * @return The number of pseudo-legal moves avaliable to the given color
 */
int mobility(const Board &, Color);

/**
 * @brief Returns the number of rooks on open files that the given color has on
 * the givne board
 *
 * @param board Board to check for rooks on open files
 * @param color Color of player to check for rooks on open files
 * @return The number of rooks on open files that the given color has on the
 * given board
 */
int rooksOnOpenFiles(const Board &, Color);

/**
 * @brief Returns the number doubled pawns for the given color on the
 * given board
 *
 * A doubled pawn is defined as having two pawns on the same file. Each
 * extra pawn on the same file is counted as another doubled pawn. For
 * example, a file with 2 pawns would count as having one doubled pawn
 * and a file with 3 pawns would count has having two.
 *
 * @param board Board to check for doubled pawns
 * @param color Color of doubled pawns to check
 * @return The number of doubled pawns that the given color has on the given
 * board
 */
int doubledPawns(const Board &, Color);

/**
 * @brief Returns the number of isolated pawns on the given board for the
 * given color
 *
 * @param board Board to check for isolated pawns
 * @param color Color of isolated pawns to check
 * @return The number of isolated pawns that the given color has on the given
 * board
 */
int isolatedPawns(const Board &, Color);

/**
 * @brief Returns the number of backward pawns on the given board for the
 * given color
 *
 * @param board Board to check for backward pawns
 * @param color Color of backward pawns to check
 * @return The number of backward pawns that the given color has on the given
 * board
 */
int backwardPawns(const Board &, Color);

/**
 * @brief Returns the number of pawns shielding the king of the given color on
 * the given board
 *
 * "Pawns shielding the king" are defined to be the three pawns to the
 * north, northeast and northwest (for white) or south, southeast and
 * southwest (for black). If the king is on the A or H files, the missing
 * square will be disregarded. Pawn shields are only considered if the pawns
 * are on rank 2 (for white) or rank 7 (for black).
 *
 * @param board Board to check shield pawns for
 * @param color Color to check shield pawns for
 * @return The number of pawns shielding the king of the given color on the
 * given board
 */
int pawnsShieldingKing(const Board &, Color);
};

#endif