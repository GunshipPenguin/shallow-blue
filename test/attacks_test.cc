#include "defs.h"
#include "attacks.h"
#include "catch.hpp"

TEST_CASE("AttackTable output is correct") {
  SECTION("Attack values are correct for white and black pawns") {
    REQUIRE(Attacks::getNonSlidingAttacks(PAWN, 8, WHITE) == 0x20000);
    REQUIRE(Attacks::getNonSlidingAttacks(PAWN, 9, WHITE) == 0x50000);

    REQUIRE(Attacks::getNonSlidingAttacks(PAWN, 55, BLACK) == 0x400000000000);
    REQUIRE(Attacks::getNonSlidingAttacks(PAWN, 54, BLACK) == 0xA00000000000);
  }

  SECTION("Attack values are correct for knights") {
    REQUIRE(Attacks::getNonSlidingAttacks(KNIGHT, 0) == 0x20400);
    REQUIRE(Attacks::getNonSlidingAttacks(KNIGHT, 27) == 0x142200221400);
  }

  SECTION("Attack values are correct for kings") {
    REQUIRE(Attacks::getNonSlidingAttacks(KING, 0) == 0x302);
    REQUIRE(Attacks::getNonSlidingAttacks(KING, 27) == 0x1C141C0000);
  }
}