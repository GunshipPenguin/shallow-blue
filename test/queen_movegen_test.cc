#include "catch.hpp"
#include "board.h"

TEST_CASE("Queen move generation is corect") {
  Board board;

  SECTION("Queen move generation is correct in the middle of an empty board") {
    board.setToFen("8/8/8/3Q4/8/8/8/8 w - -");
    REQUIRE(board.getWhiteQueenMoves().size() == 27);

    board.setToFen("8/8/8/3q4/8/8/8/8 w - -");
    REQUIRE(board.getBlackQueenMoves().size() == 27);
  }

  SECTION("Queen move generation is correct when blocked by an opponent's piece") {
    board.setToFen("8/1p1p1p2/8/1p1Q1p2/8/1p1p1p2/8/8 w - -");
    REQUIRE(board.getWhiteQueenMoves().size() == 16);

    board.setToFen("8/1P1P1P2/8/1P1q1P2/8/1P1P1P2/8/8 w - -");
    REQUIRE(board.getBlackQueenMoves().size() == 16);
  }

  SECTION("Queen move generation is correct when blocked by own piece") {
    board.setToFen("8/1P1P1P2/8/1P1Q1P2/8/1P1P1P2/8/8 w - -");
    REQUIRE(board.getWhiteQueenMoves().size() == 8);

    board.setToFen("8/1p1p1p2/8/1p1q1p2/8/1p1p1p2/8/8 w - -");
    REQUIRE(board.getBlackQueenMoves().size() == 8);
  }

  SECTION("Queen move generation is correct with 2 queens on different ranks/files/diagonals") {
    board.setToFen("8/8/8/5Q2/2Q5/8/8/8 w - -");
    REQUIRE(board.getWhiteQueenMoves().size() == 50);

    board.setToFen("8/8/8/5q2/2q5/8/8/8 w - -");
    REQUIRE(board.getBlackQueenMoves().size() == 50);
  }
}
