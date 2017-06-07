#include "board.h"
#include "catch.hpp"

TEST_CASE("King move generation is correct") {
  Board board;

  SECTION("White and black kings can move to all possible squares on an empty board") {
    board.setToFen("8/8/8/8/3K4/8/8/8 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 8);

    board.setToFen("8/8/8/8/3k4/8/8/8 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 8);
  }

  SECTION("White and black kings have the correct number of moves on a1") {
    board.setToFen("8/8/8/8/8/8/8/K7 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 3);

    board.setToFen("8/8/8/8/8/8/8/k7 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 3);
  }

  SECTION("White and black kings have the correct number of moves on h8") {
    board.setToFen("7K/8/8/8/8/8/8/8 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 3);

    board.setToFen("7k/8/8/8/8/8/8/8 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 3);
  }

  SECTION("White and black kings attack the other player's pieces") {
    board.setToFen("8/8/8/4r3/3K4/8/8/8 w - -");
    REQUIRE(board.getWhiteKingMoves().size() == 8);

    board.setToFen("8/8/8/4R3/3k4/8/8/8 w - -");
    REQUIRE(board.getBlackKingMoves().size() == 8);
  }
}
