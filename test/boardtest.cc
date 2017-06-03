#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "board.h"


TEST_CASE("Pawn move generation is correct") {
    Board board;

    SECTION("White pawn moves are generated correctly from start position") {
      board.setToStartPos();

      REQUIRE(board.getWhitePawnMoves().size() == 16);
    }

    SECTION("Black pawn moves are generated correctly from start position") {
      board.setToStartPos();

      REQUIRE(board.getBlackPawnMoves().size() == 16);
    }

    SECTION("White pawn attacks are generated correctly from start position") {
      board.setToStartPos();

      REQUIRE(board.getWhitePawnAttacks().size() == 0);
    }

    SECTION("Black pawn attacks are generated correctly from start position") {
      board.setToStartPos();

      REQUIRE(board.getBlackPawnAttacks().size() == 0);
    }
}
