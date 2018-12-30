#include "catch.hpp"
#include "movegen.h"

TEST_CASE("Rook move generation is correct") {
  Board board;
  MoveGen movegen;

  SECTION("Rook move generation is correct in the middle of an empty board") {
    board.setToFen("8/8/8/3R4/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 14);

    board.setToFen("8/8/8/3r4/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 14);
  }

  SECTION("Rook move generation is correct when blocked by opponent's pieces") {
    board.setToFen("8/3p4/8/1p1R1p2/8/3p4/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);

    board.setToFen("8/3P4/8/1P1r1P2/8/3P4/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Rook move generation is correct when blocked by own pieces") {
    board.setToFen("8/8/8/8/3P4/3R4/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 10);

    board.setToFen("8/8/3p4/3r4/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 11);
  }

  SECTION("Rook move generation is correct with 2 rooks on different ranks and files") {
    board.setToFen("8/8/8/4R3/3R4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 28);

    board.setToFen("8/8/8/4r3/3r4/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 28);
  }

  SECTION("Rook move generation is correct with 2 rooks on the same rank") {
    board.setToFen("8/8/8/8/3RR3/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 20);

    board.setToFen("8/8/8/8/3rr3/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 20);
  }

  SECTION("Rook move generation is correct with 2 rooks on the same file") {
    board.setToFen("8/8/8/3R4/3R4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 20);

    board.setToFen("8/8/8/3r4/3r4/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 20);
  }

  SECTION("Rook move generation is correct with 4 rooks on a1, a8, h1, h8") {
    board.setToFen("R6R/8/8/8/8/8/8/R6R w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 48);

    board.setToFen("r6r/8/8/8/8/8/8/r6r b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 48);
  }

  SECTION("Rooks do not capture kings") {
    board.setToFen("8/8/8/8/8/8/8/Rk6 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);

    board.setToFen("8/8/8/8/8/8/8/rK6 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);
  }
}
