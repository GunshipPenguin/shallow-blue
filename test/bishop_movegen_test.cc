#include "catch.hpp"
#include "board.h"

TEST_CASE("Bishop move generation is correct") {
  Board board;

  SECTION("Bishop move generation is correct in the middle of an empty board") {
    board.setToFen("8/8/8/3B4/8/8/8/8 w - -");
    REQUIRE(board.getWhiteBishopMoves().size() == 13);

    board.setToFen("8/8/8/3b4/8/8/8/8 w - -");
    REQUIRE(board.getBlackBishopMoves().size() == 13);
  }

  SECTION("Bishop move generation is correct when blocked by opponent's pieces") {
    board.setToFen("8/1p3p2/8/3B4/8/1p3p2/8/8 w - -");
    REQUIRE(board.getWhiteBishopMoves().size() == 8);

    board.setToFen("8/1P3P2/8/3b4/8/1P3P2/8/8 w - -");
    REQUIRE(board.getBlackBishopMoves().size() == 8);
  }

  SECTION("Bishop move generation is correct when blocked by own pieces") {
    board.setToFen("8/1P3P2/8/3B4/8/1P3P2/8/8 w - -");
    REQUIRE(board.getWhiteBishopMoves().size() == 4);

    board.setToFen("8/1p3p2/8/3b4/8/1p3p2/8/8 w - -");
    REQUIRE(board.getBlackBishopMoves().size() == 4);
  }

  SECTION("Bishop move generation is correct with 2 bishops on different diaognals") {
    board.setToFen("8/8/8/8/3BB3/8/8/8 w - -");
    REQUIRE(board.getWhiteBishopMoves().size() == 26);

    board.setToFen("8/8/8/8/3bb3/8/8/8 w - -");
    REQUIRE(board.getBlackBishopMoves().size() == 26);
  }

  SECTION("Bishop move generation is correct with 2 bishops on the same diagonal") {
    board.setToFen("8/8/5B2/8/3B4/8/8/8 w - -");
    REQUIRE(board.getWhiteBishopMoves().size() == 17);

    board.setToFen("8/8/5b2/8/3b4/8/8/8 w - -");
    REQUIRE(board.getBlackBishopMoves().size() == 17);
  }

  SECTION("Bishop move generation is correct with 4 bishops in a1, a8, h1 h8") {
    board.setToFen("B6B/8/8/8/8/8/8/B6B w - -");
    REQUIRE(board.getWhiteBishopMoves().size() == 24);

    board.setToFen("b6b/8/8/8/8/8/8/b6b w - -");
    REQUIRE(board.getBlackBishopMoves().size() == 24);
  }
}
