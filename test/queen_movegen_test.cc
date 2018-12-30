#include "catch.hpp"
#include "movegen.h"

TEST_CASE("Queen move generation is corect") {
  Board board;
  MoveGen movegen;

  SECTION("Queen move generation is correct in the middle of an empty board") {
    board.setToFen("8/8/8/3Q4/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 27);

    board.setToFen("8/8/8/3q4/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 27);
  }

  SECTION("Queen move generation is correct when blocked by an opponent's piece") {
    board.setToFen("8/1p1p1p2/8/1p1Q1p2/8/1p1p1p2/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 16);

    board.setToFen("8/1P1P1P2/8/1P1q1P2/8/1P1P1P2/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 16);
  }

  SECTION("Queen move generation is correct when blocked by own piece") {
    board.setToFen("8/8/8/8/3P4/4Q3/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 22);

    board.setToFen("8/8/8/8/3p4/4q3/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 22);
  }

  SECTION("Queen move generation is correct with 2 queens on different ranks/files/diagonals") {
    board.setToFen("8/8/8/5Q2/2Q5/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 50);

    board.setToFen("8/8/8/5q2/2q5/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 50);
  }

  SECTION("Queens do not capture kings") {
    board.setToFen("8/8/8/8/8/8/1k6/Q7 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 14);

    board.setToFen("8/8/8/8/8/8/1K6/q7 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 14);
  }
}
