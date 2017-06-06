#include "catch.hpp"
#include "cmove.h"

TEST_CASE("Moves representation is correct") {
  SECTION("Getters in CMove work as expected") {
    CMove move(1, 63, CMove::CAPTURE);

    REQUIRE(move.getFrom() == 1);
    REQUIRE(move.getTo() == 63);
    REQUIRE(move.getFlags() == CMove::CAPTURE);
  }

  SECTION("indexToNotation works correctly") {
    // Corners
    REQUIRE(CMove::indexToNotation(0) == "a1");
    REQUIRE(CMove::indexToNotation(7) == "h1");
    REQUIRE(CMove::indexToNotation(63) == "h8");
    REQUIRE(CMove::indexToNotation(56) == "a8");

    // Center Squares
    REQUIRE(CMove::indexToNotation(27) == "d4");
    REQUIRE(CMove::indexToNotation(28) == "e4");
    REQUIRE(CMove::indexToNotation(35) == "d5");
    REQUIRE(CMove::indexToNotation(36) == "e5");
  }

  SECTION("notationToIndex works correctly") {
    // Corners
    REQUIRE(CMove::notationToIndex("a1") == 0);
    REQUIRE(CMove::notationToIndex("h1") == 7);
    REQUIRE(CMove::notationToIndex("h8") == 63);
    REQUIRE(CMove::notationToIndex("a8") == 56);

    // Center Squares
    REQUIRE(CMove::notationToIndex("d4") == 27);
    REQUIRE(CMove::notationToIndex("e4") == 28);
    REQUIRE(CMove::notationToIndex("d5") == 35);
    REQUIRE(CMove::notationToIndex("e5") == 36);
  }
}
