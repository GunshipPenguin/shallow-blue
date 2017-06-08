#include "board.h"
#include "catch.hpp"

TEST_CASE("Knight move generation is correct") {
  Board board;

  SECTION("Knight move generation is correct from the start position") {
    board.setToStartPos();

    REQUIRE(board.getWhiteKnightMoves().size() == 4);
    REQUIRE(board.getBlackKnightMoves().size() == 4);
  }

  SECTION("Knight move generation is correct in the middle of a clear board") {
    board.setToFen("8/8/8/3N4/8/8/8/8 w - -");
    REQUIRE(board.getWhiteKnightMoves().size() == 8);

    board.setToFen("8/8/8/3n4/8/8/8/8 w - -");
    REQUIRE(board.getBlackKnightMoves().size() == 8);
  }

  SECTION("Knight move generation is correct from a1") {
    board.setToFen("8/8/8/8/8/8/8/N7 w - -");
    REQUIRE(board.getWhiteKnightMoves().size() == 2);

    board.setToFen("8/8/8/8/8/8/8/n7 w - -");
    REQUIRE(board.getBlackKnightMoves().size() == 2);
  }

  SECTION("Knight move generation is correct from h8") {
    board.setToFen("7N/8/8/8/8/8/8/8 w - -");
    REQUIRE(board.getWhiteKnightMoves().size() == 2);

    board.setToFen("7n/8/8/8/8/8/8/8 w - -");
    REQUIRE(board.getBlackKnightMoves().size() == 2);
  }

  SECTION("Knights attack opposing pieces") {
    board.setToFen("8/8/3p1p2/2b3b1/4N3/2r3r1/3p1p2/8 w - -");
    REQUIRE(board.getWhiteKnightMoves().size() == 8);

    board.setToFen("8/8/3P1P2/2B3B1/4n3/2R3R1/3P1P2/8 w - -");
    REQUIRE(board.getBlackKnightMoves().size() == 8);
  }

  SECTION("Knights do not move to squares occupied by their own pieces") {
    board.setToFen("8/8/4P3/8/3N4/8/8/8 w - -");
    REQUIRE(board.getWhiteKnightMoves().size() == 7);

    board.setToFen("8/8/4p3/8/3n4/8/8/8 w - -");
    REQUIRE(board.getBlackKnightMoves().size() == 7);
  }
}
