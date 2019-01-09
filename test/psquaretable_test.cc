#include "psquaretable.h"
#include "catch.hpp"

TEST_CASE("Piece square tables work as expected") {
  PSquareTable pst;

  SECTION("PSquareTables initially have score 0 for white and black") {
    REQUIRE(pst.getScore(WHITE) == 0);
    REQUIRE(pst.getScore(BLACK) == 0);
  }

  SECTION("PSquareTables add and remove value for the correct color") {
    REQUIRE(pst.getScore(WHITE) == 0);
    REQUIRE(pst.getScore(BLACK) == 0);

    pst.addPiece(WHITE, PAWN, e4);
    REQUIRE(pst.getScore(WHITE) > 0);
    REQUIRE(pst.getScore(BLACK) == 0);

    pst.removePiece(WHITE, PAWN, e4);
    REQUIRE(pst.getScore(WHITE) == 0);
    REQUIRE(pst.getScore(BLACK) == 0);
  }

  SECTION("PSquareTables move pieces with movePiece()") {
    REQUIRE(pst.getScore(WHITE) == 0);

    pst.addPiece(WHITE, BISHOP, e4);
    int initScore = pst.getScore(WHITE);

    pst.movePiece(WHITE, BISHOP, e4, a8);

    REQUIRE(pst.getScore(WHITE) != initScore);
  }
}
