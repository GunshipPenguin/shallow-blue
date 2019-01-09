#include "board.h"
#include "movegen.h"
#include "catch.hpp"

TEST_CASE("Knight move generation is correct") {
  Board board;
  MoveGen movegen;

  SECTION("Knight move generation is correct in the middle of a clear board") {
    board.setToFen("8/8/8/3N4/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);

    board.setToFen("8/8/8/3n4/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Knight move generation is correct from a1") {
    board.setToFen("8/8/8/8/8/8/8/N7 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 2);

    board.setToFen("8/8/8/8/8/8/8/n7 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 2);
  }

  SECTION("Knight move generation is correct from h8") {
    board.setToFen("7N/8/8/8/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 2);

    board.setToFen("7n/8/8/8/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 2);
  }

  SECTION("Knights attack opposing pieces") {
    board.setToFen("8/8/3p1p2/2b3b1/4N3/2r3r1/3p1p2/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);

    board.setToFen("8/8/3P1P2/2B3B1/4n3/2R3R1/3P1P2/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Knights do not move to squares occupied by their own pieces") {
    board.setToFen("8/8/4P3/8/3N4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);

    board.setToFen("8/8/4p3/8/3n4/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Knights do not capture kings") {
    board.setToFen("8/8/8/8/8/1k6/8/N7 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 1);

    board.setToFen("8/8/8/8/8/1K6/8/n7 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 1);
  }
}
