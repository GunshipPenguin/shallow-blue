#include "movepicker.h"

// Indexed by [victimValue][attackerValue]
int MovePicker::_mvvLvaTable[5][6];

void MovePicker::init() {
  // Build the MVV LVA table
  int currScore = 0;
  PieceType victimsLoToHi[] = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN};
  PieceType attackersHiToLo[] = {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};

  // Iterate over victims (low to high)
  for (auto victim : victimsLoToHi) {
    // Iterate over attackers (high to low)
    for (auto attacker : attackersHiToLo) {
      _mvvLvaTable[victim][attacker] = currScore++;
    }
  }
}

MovePicker::MovePicker(MoveList *moveList) {
  _moves = moveList;
}