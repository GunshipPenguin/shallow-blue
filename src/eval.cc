#include "defs.h"
#include "movegen.h"
#include "bitutils.h"
#include "eval.h"

U64 Eval::detail::FILES[8] = {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};
U64 Eval::detail::NEIGHBOR_FILES[8] {
  FILE_B,
  FILE_A | FILE_C,
  FILE_B | FILE_D,
  FILE_C | FILE_E,
  FILE_D | FILE_F,
  FILE_E | FILE_G,
  FILE_F | FILE_H,
  FILE_G
};
U64 Eval::detail::PAWN_SHIELDS[2][64];

void Eval::init() {
  // Fill king pawn shield bitboard
  for (int i=0;i<64;i++) {
    U64 square = ONE << i;

    detail::PAWN_SHIELDS[WHITE][i] = (square << 8) | ((square << 7) & ~FILE_H) | ((square << 9) & ~FILE_A);
    detail::PAWN_SHIELDS[BLACK][i] = (square >> 8) | ((square >> 7) & ~FILE_A) | ((square >> 9) & ~FILE_H);
  }
}

int Eval::getMaterialValue(PieceType pieceType) {
  int value = 0;
  switch (pieceType) {
    case PAWN: value = PAWN_VALUE;
      break;
    case KNIGHT: value = KNIGHT_VALUE;
      break;
    case BISHOP: value = BISHOP_VALUE;
      break;
    case ROOK: value = ROOK_VALUE;
      break;
    case QUEEN: value = QUEEN_VALUE;
      break;
    default: value = 0;
      break;
  }
  return value;
}

bool Eval::hasBishopPair(const Board& board, Color color) {
  return ((board.getPieces(color, BISHOP) & BLACK_SQUARES) != ZERO) && ((board.getPieces(color, BISHOP) & WHITE_SQUARES) != ZERO);
}

int Eval::mobility(const Board& board, Color color) {
  int moves = 0;

  // Special case for pawn moves
  U64 pawns = board.getPieces(color, PAWN);
  U64 singlePawnPushes, doublePawnPushes, pawnAttacks;
  if (color == WHITE) {
    singlePawnPushes = (pawns << 8) & board.getNotOccupied();
    doublePawnPushes = ((singlePawnPushes & RANK_3) << 8) & board.getNotOccupied();
    pawnAttacks = ((pawns << 7) & ~FILE_H) | ((pawns << 9) & ~FILE_A);
  } else {
    singlePawnPushes = (pawns >> 8) & board.getNotOccupied();
    doublePawnPushes = ((singlePawnPushes & RANK_6) >> 8) & board.getNotOccupied();
    pawnAttacks = ((pawns >> 7) & ~FILE_A) | ((pawns >> 9) & ~FILE_H);
  }
  
  pawnAttacks &= board.getAttackable(getOppositeColor(color));

  moves += _popCount(singlePawnPushes | doublePawnPushes | pawnAttacks);

  // All other pieces
  for (auto pieceType : {ROOK, KNIGHT, BISHOP, QUEEN, KING}) {
    U64 pieces = board.getPieces(color, pieceType);

    while (pieces) {
      int square = _popLsb(pieces);

      U64 attackBitBoard = board.getAttacksForSquare(pieceType, color, square);

      moves += _popCount(attackBitBoard);
    }
  }

  return moves;
}

int Eval::rooksOnOpenFiles(const Board& board, Color color) {
  int numRooks = 0;

  for (U64 file : detail::FILES) {
    if (_popCount(file & board.getPieces(color, ROOK) & board.getAllPieces(color)) == 1) {
      numRooks++;
    }
  }
  return numRooks;
}

int Eval::doubledPawns(const Board& board, Color color) {
  int doubled = 0;

  for (U64 file : detail::FILES) {
    int pawnsOnFile = _popCount(file & board.getPieces(color, PAWN));

    if (pawnsOnFile > 1) doubled += (pawnsOnFile - 1);
  }

  return doubled;
}

int Eval::isolatedPawns(const Board& board, Color color) {
  int isolated = 0;

  U64 pawns = board.getPieces(color, PAWN);

  for (int fileNumber=0;fileNumber<8;fileNumber++) {
    U64 fileMask = detail::FILES[fileNumber];
    U64 neighborFileMask = detail::NEIGHBOR_FILES[fileNumber];

    if ((fileMask & pawns) && !(neighborFileMask & pawns)) isolated++;
  }

  return isolated;
}

int Eval::backwardPawns(const Board& board, Color color) {
  // Pawns whose stop square is under attack by our opponent's
  // pawns but not by us are backward
  U64 pawns = board.getPieces(color, PAWN);
  U64 opponentPawns = board.getPieces(getOppositeColor(color), PAWN);

  U64 stopSquares, ownPawnAttacks, opponentPawnAttacks;
  if (color == WHITE) {
    stopSquares = (pawns << 8) & board.getNotOccupied();
    ownPawnAttacks = ((pawns << 7) & ~FILE_H) | ((pawns << 9) & ~FILE_A);
    opponentPawnAttacks = ((opponentPawns >> 7) & ~FILE_A) | ((opponentPawns >> 9) & ~FILE_H);
  } else {
    stopSquares = (pawns >> 8) & board.getNotOccupied();
    ownPawnAttacks = ((pawns >> 7) & ~FILE_A) | ((pawns >> 9) & ~FILE_H);
    opponentPawnAttacks = ((opponentPawns << 7) & ~FILE_H) | ((opponentPawns << 9) & ~FILE_A);
  }

  return _popCount(stopSquares & ~ownPawnAttacks & opponentPawnAttacks);
}

int Eval::pawnsShieldingKing(const Board& board, Color color) {
  int kingSquare = _bitscanForward(board.getPieces(color, KING));

  return _popCount(detail::PAWN_SHIELDS[color][kingSquare] & board.getPieces(color, PAWN));
}

int Eval::evaluate(const Board& board, Color color) {
  int score = 0;

  Color otherColor = getOppositeColor(color);

  // Material value
  score += PAWN_VALUE * (_popCount(board.getPieces(color, PAWN)) - _popCount(board.getPieces(otherColor, PAWN)));
  score += KNIGHT_VALUE * (_popCount(board.getPieces(color, KNIGHT)) - _popCount(board.getPieces(otherColor, KNIGHT)));
  score += BISHOP_VALUE * (_popCount(board.getPieces(color, BISHOP)) - _popCount(board.getPieces(otherColor, BISHOP)));
  score += ROOK_VALUE * (_popCount(board.getPieces(color, ROOK)) - _popCount(board.getPieces(otherColor, ROOK)));
  score += QUEEN_VALUE * (_popCount(board.getPieces(color, QUEEN)) - _popCount(board.getPieces(otherColor, QUEEN)));
  
  // Piece square tables
  score += (board.getPSquareTable().getScore(color) - board.getPSquareTable().getScore(otherColor));

  // Mobility
  score += MOBILITY_BONUS * (mobility(board, color) - mobility(board, otherColor));

  // Rook on open file
  score += ROOK_OPEN_FILE_BONUS * (rooksOnOpenFiles(board, color) - rooksOnOpenFiles(board, otherColor));

  // Bishop pair
  score += hasBishopPair(board, color) ? BISHOP_PAIR_BONUS : 0;
  score -= hasBishopPair(board, otherColor) ? BISHOP_PAIR_BONUS : 0;

  // Doubled pawns
  score += DOUBLED_PAWN_PENALTY * (doubledPawns(board, color) - doubledPawns(board, otherColor));

  // Isolated pawns
  score += ISOLATED_PAWN_PENALTY * (isolatedPawns(board, color) - isolatedPawns(board, otherColor));

  // Backward pawns
  score += BACKWARD_PAWN_PENALTY * (backwardPawns(board, color) - backwardPawns(board, otherColor));

  // King shield
  score += KING_PAWN_SHIELD_BONUS * (pawnsShieldingKing(board, color) - pawnsShieldingKing(board, otherColor));

  return score;
}
