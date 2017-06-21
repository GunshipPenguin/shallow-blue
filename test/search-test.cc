#include "search.h"
#include "catch.hpp"
#include "board.h"

TEST_CASE("Search works as expected") {
  Board board;

  SECTION("Search finds a checkmate on next move") {
    // Fool's mate
    board.setToFen("rnbqkbnr/pppp1ppp/4p3/8/6P1/5P2/PPPPP2P/RNBQKBNR b KQkq -");

    Search search(board);

    REQUIRE(search.getBestMove().getNotation() == "d8h4");
  }
}
