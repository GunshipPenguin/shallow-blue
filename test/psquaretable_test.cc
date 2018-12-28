#include "psquaretable.h"
#include "board.h"
#include "catch.hpp"

TEST_CASE("Piece square tables work as expected") {
  PSquareTable pst;

  SECTION("PSquareTables initially have score 0 for white and black") {
    REQUIRE(pst.getScore(OPENING, WHITE) == 0);
    REQUIRE(pst.getScore(OPENING, BLACK) == 0);
  }

  SECTION("PSquareTables add and remove value for the correct color") {
    REQUIRE(pst.getScore(OPENING, WHITE) == 0);
    REQUIRE(pst.getScore(OPENING, BLACK) == 0);

    pst.addPiece(WHITE, PAWN, e4);
    REQUIRE(pst.getScore(OPENING, WHITE) > 0);
    REQUIRE(pst.getScore(OPENING, BLACK) == 0);

    pst.removePiece(WHITE, PAWN, e4);
    REQUIRE(pst.getScore(OPENING, WHITE) == 0);
    REQUIRE(pst.getScore(OPENING, BLACK) == 0);
  }

  SECTION("PSquareTables move pieces with movePiece()") {
    REQUIRE(pst.getScore(OPENING, WHITE) == 0);

    pst.addPiece(WHITE, BISHOP, e4);
    int initScore = pst.getScore(OPENING, WHITE);

    pst.movePiece(WHITE, BISHOP, e4, a8);

    REQUIRE(pst.getScore(OPENING, WHITE) != initScore);
  }

  SECTION("PSquareTables apply different tables for different game phases") {
    // This is an objectively better position for white than black, especially
    // in the endgame. It should still be an advantage for white under an
    // opening evaluation, however, the evaluated advantage should be lower
    Board endgameBoard("4k3/3pp2P/P7/8/8/8/8/4K3 w KQkq -");

    PSquareTable pst(endgameBoard);

    REQUIRE(pst.getScore(ENDGAME, WHITE) > pst.getScore(ENDGAME, BLACK));
    REQUIRE(pst.getScore(OPENING, WHITE) > pst.getScore(OPENING, BLACK));
    REQUIRE(pst.getScore(ENDGAME, WHITE) > pst.getScore(OPENING, WHITE));
  }
}
