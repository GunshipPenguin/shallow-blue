#include "catch.hpp"
#include "movegen.h"

TEST_CASE("Pawn move generation is correct") {
  Board board;
  MoveGen movegen;

  SECTION("White pawn moves are correct for a piece on rank 2") {
    board.setToFen("8/8/8/8/8/8/4P3/8 w - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 2);
  }

  SECTION("Black pawn moves are correct for a piece on rank 7") {
    board.setToFen("8/3p4/8/8/8/8/8/8 b - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 2);
  }

  SECTION("1 square White pawn moves are not generated if piece is blocked") {
    board.setToFen("8/8/8/3p4/3P4/8/8/8 w - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 0);
  }

  SECTION("1 square Black pawn moves are not generated if piece is blocked") {
    board.setToFen("8/8/8/3p4/3P4/8/8/8 b - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 0);
  }

  SECTION("2 square white pawn moves are not generated if a piece is blocked") {
    board.setToFen("8/8/8/8/8/4b3/4P3/8 w - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 0);
  }

  SECTION("2 square black pawn moves are not generated if a piece is blocked") {
    board.setToFen("8/4p3/4B3/8/8/8/8/8 b - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 0);
  }

  SECTION("White pawn promotion moves are generated upon reaching rank 8") {
    board.setToFen("8/4P3/8/8/8/8/8/8 w - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 4);
  }

  SECTION("Black pawn promotion moves are generated upon reaching rank 1") {
    board.setToFen("8/8/8/8/8/8/4p3/8 b - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 4);
  }

  SECTION("Black pawn attacks are generated correctly for one attackable piece") {
    board.setToFen("8/8/8/8/3p4/2N5/8/8 b - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 2);
  }

  SECTION("Black pawn attacks are generated correctly for two attackable pieces") {
    board.setToFen("8/8/8/8/3p4/2N1N3/8/8 b - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 3);
  }

  SECTION("White pawn attacks to rank 8 result in a promotion") {
    board.setToFen("2r5/3P4/8/8/8/8/8/8 w - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Black pawn attacks to rank 1 result in a promotion") {
    board.setToFen("8/8/8/8/8/8/3p4/2R5 b - -");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("White pawn moves are generated correctly for en passant") {
    board.setToFen("8/8/1pP5/8/8/8/8/8 w - b7");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 2);
  }

  SECTION("Black pawn moves are generated correctly for en passant") {
    board.setToFen("8/8/8/8/1pP5/8/8/8 b - c3");
    movegen.setBoard(board);

    REQUIRE(movegen.getMoves().size() == 2);
  }
}
