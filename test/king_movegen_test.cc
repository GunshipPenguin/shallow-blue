#include "board.h"
#include "catch.hpp"

#include <iostream>

TEST_CASE("King move generation is correct") {
  Board board;

  SECTION("Kings can move to all possible squares on an empty board") {
    board.setToFen("8/8/8/8/3K4/8/8/8 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 8);

    board.setToFen("8/8/8/8/3k4/8/8/8 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 8);
  }

  SECTION("Kings have the correct number of moves on a1") {
    board.setToFen("8/8/8/8/8/8/8/K7 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 3);

    board.setToFen("8/8/8/8/8/8/8/k7 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 3);
  }

  SECTION("Kings have the correct number of moves on h8") {
    board.setToFen("7K/8/8/8/8/8/8/8 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 3);

    board.setToFen("7k/8/8/8/8/8/8/8 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 3);
  }

  SECTION("Kings do not move to squares occupied by their own pieces") {
    board.setToFen("8/8/3P4/3K4/8/8/8/8 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 7);

    board.setToFen("8/8/3p4/3k4/8/8/8/8 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 7);
  }

  SECTION("Kings attack the other player's pieces") {
    board.setToFen("8/8/8/4r3/3K4/8/8/8 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 8);

    board.setToFen("8/8/8/4R3/3k4/8/8/8 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 8);
  }

  SECTION("Kings do not capture the other king") {
    board.setToFen("8/8/8/3Kk3/8/8/8/8 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 7);

    board.setToFen("8/8/8/3kK3/8/8/8/8 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 7);
  }

  SECTION("Castles are generated when castling is possible") {
    // White kingside only
    board.setToFen("8/8/8/8/8/8/8/R3K2R w K -");
    REQUIRE(board.getWhiteKingMoves().size() == 6);

    // White queenside only
    board.setToFen("8/8/8/8/8/8/8/R3K2R w Q -");
    REQUIRE(board.getWhiteKingMoves().size() == 6);

    // White king and queenside
    board.setToFen("8/8/8/8/8/8/8/R3K2R w KQ -");
    REQUIRE(board.getWhiteKingMoves().size() == 7);

    // Black kingside only
    board.setToFen("r3k2r/8/8/8/8/8/8/8 w k -");
    REQUIRE(board.getBlackKingMoves().size() == 6);

    // Black queenside only
    board.setToFen("r3k2r/8/8/8/8/8/8/8 w q -");
    REQUIRE(board.getBlackKingMoves().size() == 6);

    // Black king and queenside
    board.setToFen("r3k2r/8/8/8/8/8/8/8 w kq -");
    REQUIRE(board.getBlackKingMoves().size() == 7);

    // Both sides all available
    board.setToFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq -");
    REQUIRE(board.getWhiteKingMoves().size() == 7);
    REQUIRE(board.getBlackKingMoves().size() == 7);
  }
}
