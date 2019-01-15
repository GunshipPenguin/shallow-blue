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
 * @brief Array of the files neighboring each file as bitboards
 */
extern U64 NEIGHBOR_FILES[8];

/**
 * @brief Array of masks indexed by [Color][square] containing all squares that
 * must be free of enemy pawns for a pawn of the given color on the given
 * square to be considered passed
 */
extern U64 PASSED_PAWN_MASKS[2][64];

/**
 * @brief Array of masks indexed by [Color][square] containing all squares
 * considered part of the "pawn shield" for a king of the given color on the
 * given square
 */
extern U64 PAWN_SHIELD_MASKS[2][64];

/**
 * @brief Weights for each piece used to calculate the game phase based off
 * remaining material
 */
const int PHASE_WEIGHTS[6] = {
    [PAWN] = 0,
    [ROOK] = 2,
    [KNIGHT] = 1,
    [BISHOP] = 1,
    [QUEEN] = 4,
    [KING] = 0
};

/**
 * @brief Weighted sum of the values in PHASE_WEIGHTS used for calculating
 * a tapered evaluation score
 *
 * This is the sum of each item in detail::PHASE_WEIGHTS with each value multiplied
 * by how many of those pieces are initially on the board (Multiply the
 * value for pawns by 16, knights by 4, etc.).
 */
extern int PHASE_WEIGHT_SUM;
};

/**
 * @brief Bonuses given to a player having a move available (opening/endgame)
 */
const int MOBILITY_BONUS[2][6] = {
    [OPENING] = {
        [PAWN] = 0,
        [ROOK] = 0,
        [KNIGHT] = 4,
        [BISHOP] = 3,
        [QUEEN] = 0,
        [KING] = 0
    },
    [ENDGAME] = {
        [PAWN] = 1,
        [ROOK] = 1,
        [KNIGHT] = 6,
        [BISHOP] = 2,
        [QUEEN] = 1,
        [KING] = 1
    }
};

/**
 * @brief Array indexed by [Phase][PieceType] of material values (in centipawns)
 */
const int MATERIAL_VALUES[2][6] = {
    [OPENING] = {
        [PAWN] = 100,
        [ROOK] = 500,
        [KNIGHT] = 320,
        [BISHOP] = 330,
        [QUEEN] = 900,
        [KING] = 0
    },
    [ENDGAME] = {
        [PAWN] = 140,
        [ROOK] = 500,
        [KNIGHT] = 300,
        [BISHOP] = 300,
        [QUEEN] = 900,
        [KING] = 0
    }
};

/**
 * @brief Bonuses given to a player for each rook on an open file (opening/endgame)
 */
const int ROOK_OPEN_FILE_BONUS[2] = {[OPENING] = 20, [ENDGAME] = 40};

/**
 * @brief Bonuses given to a player for having a passed pawn (opening/endgame)
 */
const int PASSED_PAWN_BONUS[2] = {[OPENING] = 10, [ENDGAME] = 70};

/**
 * @brief Penalties given to a player for having a doubled pawn (opening/endgame)
 */
const int DOUBLED_PAWN_PENALTY[2] = {[OPENING] = -20, [ENDGAME] = -30};

/**
 * @brief Penalties given to a player for having an isolated pawn (opening/endgame)
 */
const int ISOLATED_PAWN_PENALTY[2] = {[OPENING] = -15, [ENDGAME] = -30};

/**
 * @brief Bonuses given to a player for having bishops on black and white squares (opening/endgame)
 */
const int BISHOP_PAIR_BONUS[2] = {[OPENING] = 45, [ENDGAME] = 55};

/**
 * @brief Bonuses given to a player for each pawn shielding their king (opening/endgame)
 */
const int KING_PAWN_SHIELD_BONUS[2] = {[OPENING] = 10, [ENDGAME] = 0};

/**
 * @brief Initializes all inner constants used by functions in the Eval namespace
 */
void init();

/**
 * @brief Returns the evaluated advantage of the given color in centipawns
 *
 * @param board Board to evaluate
 * @param color Color to evaluate advantage of
 * @return Advantage of the given color in centipawns
 */
int evaluate(const Board &, Color);

/**
 * @brief Returns a numeric representation of the given board's phase based
 * off remaining material
 *
 * The returned score will range from 0 - Eval::MAX_PHASE.
 *
 * @return A numeric representation of the game phase
 */
int getPhase(const Board &);

/**
 * @brief Use this number as an upper bound on the numerical representation of
 * the game phase when performing a tapered evaluation
 */
const int MAX_PHASE = 256;

/**
 * @brief Returns the evaluated advantage of the given color in centipawns
 * assuming the game is in the given phase
 *
 * @tparam phase Phase of game to evaluate for
 * @param board Board to evaluate
 * @param color Color to evaluate advantage of
 * @return Advantage of the given color in centipawns, assuming the given
 * game phase
 */
template<GamePhase phase>
int evaluateForPhase(const Board &, Color);

/**
 * @brief Returns the value of the given PieceType used for evaluation
 * purposes in centipawns
 *
 * This method returns the material value assuming the game is in the opening
 * and can be used to quickly determine relative capture values for move
 * ordering purposes.
 *
 * @param pieceType PieceType to get value of
 * @return The value of the given PieceType used for evaluation purposes
 */
int getMaterialValue(PieceType);

/**
 * @brief Evaluates pawn structure and returns a score in centipawns
 *
 * This function internally uses Eval::isolatedPawns(), Eval::passedPawns()
 * and Eval::doubledPawns(), weights each value according to its score
 * and returns the pawn structure score in centipawns.
 *
 * Additionally, if the board's pawn structure has been seen before, this
 * function will look up its value from the pawn structure hash table or
 * store its score in the table if it hasn't yet been seen.
 *
 * @return The score for the given color (in centipawns), considering only
 * its pawn structure
 */
int evaluatePawnStructure(const Board &, Color, GamePhase);

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
 * @brief Returns the weighted mobility score (in centipawns) for the given
 * phase and color
 *
 * This method calculates all pseudo-legal moves for the given colors, and sums
 * the number of moves, weighting the sum as per Eval::MOBILITY_BONUS.
 *
 * @param board Board to use when generating moves
 * @param phase GamePhase to evaluate board for
 * @param color Color to count pseudo-legal moves for
 * @return The number of pseudo-legal moves avaliable to the given color
 */
int evaluateMobility(const Board &board, GamePhase phase, Color color);

/**
 * @brief Returns the number of rooks on open files that the given color has on
 * the given board
 *
 * @param board Board to check for rooks on open files
 * @param color Color of player to check for rooks on open files
 * @return The number of rooks on open files that the given color has on the
 * given board
 */
int rooksOnOpenFiles(const Board &, Color);

/**
 * @brief Returns the number of passed pawns that the given color has on the
 * given board
 *
 * @param board Board to check for passed pawns
 * @param color Color of player to check for passed pawns
 * @return
 */
int passedPawns(const Board &, Color);

/**
 * @brief Returns the number doubled pawns for the given color on the
 * given board
 *
 * A doubled pawn is defined as being one of two pawns on the same file. Each
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
