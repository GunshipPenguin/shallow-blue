#include "defs.h"
#include "attacktable.h"
#include "raytable.h"
#include "catch.hpp"

TEST_CASE("AttackTable output is correct") {
  RayTable::init();
  AttackTable::init();

  SECTION("AttackTable values are correct for white and black pawns") {
    REQUIRE(AttackTable::getAttacks(PAWN, 8, WHITE) == 0x20000);
    REQUIRE(AttackTable::getAttacks(PAWN, 9, WHITE) == 0x50000);

    REQUIRE(AttackTable::getAttacks(PAWN, 55, BLACK) == 0x400000000000);
    REQUIRE(AttackTable::getAttacks(PAWN, 54, BLACK) == 0xA00000000000);
  }

  SECTION("AttackTable values are correct for knights") {
    REQUIRE(AttackTable::getAttacks(KNIGHT, 0) == 0x20400);
    REQUIRE(AttackTable::getAttacks(KNIGHT, 27) == 0x142200221400);
  }

  SECTION("AttackTable values are correct for bishops") {
    REQUIRE(AttackTable::getAttacks(BISHOP, 0) == 0x8040201008040200);
    REQUIRE(AttackTable::getAttacks(BISHOP, 27) == 0x8041221400142241);
  }

  SECTION("AttackTable values are correct for rooks") {
    REQUIRE(AttackTable::getAttacks(ROOK, 0) == 0x1010101010101FE);
    REQUIRE(AttackTable::getAttacks(ROOK, 27) == 0x8080808F7080808);
  }

  SECTION("AttackTable values are correct for queens") {
    REQUIRE(AttackTable::getAttacks(QUEEN, 0) == 0x81412111090503FE);
    REQUIRE(AttackTable::getAttacks(QUEEN, 27) == 0x88492A1CF71C2A49);
  }

  SECTION("AttackTable values are correct for kings") {
    REQUIRE(AttackTable::getAttacks(KING, 0) == 0x302);
    REQUIRE(AttackTable::getAttacks(KING, 27) == 0x1C141C0000);
  }
}