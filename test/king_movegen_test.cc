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

    SECTION("Castles are generated when castling is possible as specified in the fen string") {
      // White kingside only
      board.setToFen("8/8/8/8/8/8/8/4K2R w K -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 15);

      // White queenside only
      board.setToFen("8/8/8/8/8/8/8/R3K3 w Q -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 16);

      // White king and queenside
      board.setToFen("8/8/8/8/8/8/8/R3K2R w KQ -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 26);

      // Black kingside only
      board.setToFen("4k2r/8/8/8/8/8/8/8 b k -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 15);

      // Black queenside only
      board.setToFen("r3k3/8/8/8/8/8/8/8 b q -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 16);

      // Black king and queenside
      board.setToFen("r3k2r/8/8/8/8/8/8/8 b kq -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 26);
    }

    SECTION("Castles are not generated when the king is in check") {
      board.setToFen("8/8/8/8/8/3n4/8/R3K2R w KQ -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 24);

      board.setToFen("r3k2r/8/3N4/8/8/8/8/8 b kq -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 24);
    }

    SECTION("White ks castles are not generated when squares that the king passes through are attacked") {
      board.setToFen("8/8/8/8/5r2/8/8/R3K2R w KQ -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 25);

      board.setToFen("8/8/8/2b5/8/8/8/R3K2R w KQ -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 25);
    }

    SECTION("White qs castles are not generated when squares that the king passes through are attacked") {
      board.setToFen("8/8/8/8/2r5/8/8/R3K2R w KQ -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 25);

      board.setToFen("8/8/8/8/3r4/8/8/R3K2R w KQ -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 25);
    }

    SECTION("Black ks castles are not generated when squares that the king passes through are attacked") {
      board.setToFen("r3k2r/8/8/5R2/8/8/8/8 b kq -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 25);

      board.setToFen("r3k2r/8/8/3B4/8/8/8/8 b kq -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 25);
    }

    SECTION("Black qs castles are not generated when squares that the king passes through are attacked") {
      board.setToFen("r3k2r/8/8/3R4/8/8/8/8 b kq -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 25);

      board.setToFen("r3k2r/8/8/5B2/8/8/8/8 b kq -");
      movegen.setBoard(board);
      REQUIRE(movegen.getMoves().size() == 25);
    }
}
