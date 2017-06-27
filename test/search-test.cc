#include "search.h"
#include "catch.hpp"
#include "board.h"

TEST_CASE("Search works as expected") {
  PSquareTable::init();
  ZKey::init();

  SECTION("Search works as expected after PSquareTable and ZKey are initialized") {
    Board board;

    SECTION("Search finds a checkmate on next move") {
      // Fool's mate
      board.setToFen("rnbqkbnr/pppp1ppp/4p3/8/6P1/5P2/PPPPP2P/RNBQKBNR b KQkq -");

      Search search(board, false);
      search.perform(1);

      REQUIRE(search.getBestMove().getNotation() == "d8h4");
    }

    SECTION("Search returns the only legal move when checkmate is 1 move away") {
      board.setToFen("r4rk1/ppp2ppp/4p3/8/4p3/4PPbP/PPPB2q1/R2QKR2 w - -");

      Search search(board, false);
      search.perform(2);

      REQUIRE(search.getBestMove().getNotation() == "f1f2");
    }

    SECTION("Search recognizes when a check can be made to capture a queen") {
      board.setToFen("8/4N3/8/1k5q/8/8/8/2K2R2 w - -");

      Search search(board, false);
      search.perform(3);

      REQUIRE(search.getBestMove().getNotation() == "f1f5");
    }
  }
}
