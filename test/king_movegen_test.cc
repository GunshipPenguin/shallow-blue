#include "board.h"
#include "movegen.h"
#include "catch.hpp"

TEST_CASE("King move generation is correct") {
  Board board;
  MoveGen movegen;

  SECTION("Kings can move to all possible squares on an empty board") {
    board.setToFen("8/8/8/8/3K4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);

    board.setToFen("8/8/8/8/3k4/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Kings have the correct number of moves on a1") {
    board.setToFen("8/8/8/8/8/8/8/K7 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 3);

    board.setToFen("8/8/8/8/8/8/8/k7 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 3);
  }

  SECTION("Kings have the correct number of moves on h8") {
    board.setToFen("7K/8/8/8/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 3);

    board.setToFen("7k/8/8/8/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 3);
  }

  SECTION("Kings do not move to squares occupied by their own pieces") {
    board.setToFen("8/8/8/3K4/3P4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);

    board.setToFen("8/8/3p4/3k4/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);
  }

  SECTION("Kings attack the other player's pieces") {
    board.setToFen("8/8/8/4r3/3K4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);

    board.setToFen("8/8/8/4R3/3k4/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Kings do not capture the other king") {
    board.setToFen("8/8/8/3Kk3/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);

    board.setToFen("8/8/8/3kK3/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);
  }
}
