#include "catch.hpp"
#include "raytable.h"

TEST_CASE("RayTable ray generation works as expected") {
  RayTable raytable;
  int a1 = 0;
  int d4 = 27;
  int e5 = 36;
  int h8 = 63;

  SECTION("RayTable generates correct North rays for a1, d4, e5, h8") {
    REQUIRE(raytable.getRay(RayTable::NORTH, a1) == 0x101010101010100);
    REQUIRE(raytable.getRay(RayTable::NORTH, d4) == 0x808080800000000);
    REQUIRE(raytable.getRay(RayTable::NORTH, e5) == 0x1010100000000000);
    REQUIRE(raytable.getRay(RayTable::NORTH, h8) == 0x0);
  }

  SECTION("RayTable generates correct South rays for a1, d4, e5, h8") {
    REQUIRE(raytable.getRay(RayTable::SOUTH, a1) == 0x0);
    REQUIRE(raytable.getRay(RayTable::SOUTH, d4) == 0x80808);
    REQUIRE(raytable.getRay(RayTable::SOUTH, e5) == 0x10101010);
    REQUIRE(raytable.getRay(RayTable::SOUTH, h8) == 0x80808080808080);
  }

  SECTION("RayTable generates correct East rays for a1, d4, e5, h8") {
    REQUIRE(raytable.getRay(RayTable::EAST, a1) == 0xfe);
    REQUIRE(raytable.getRay(RayTable::EAST, d4) == 0xf0000000);
    REQUIRE(raytable.getRay(RayTable::EAST, e5) == 0xe000000000);
    REQUIRE(raytable.getRay(RayTable::EAST, h8) == 0x0);
  }

  SECTION("RayTable generates correct West rays for a1, d4, e5, h8") {
    REQUIRE(raytable.getRay(RayTable::WEST, a1) == 0x0);
    REQUIRE(raytable.getRay(RayTable::WEST, d4) == 0x7000000);
    REQUIRE(raytable.getRay(RayTable::WEST, e5) == 0xf00000000);
    REQUIRE(raytable.getRay(RayTable::WEST, h8) == 0x7f00000000000000);
  }

  SECTION("RayTable generates correct NorthWest rays for a1, d4, e5, h8") {
    REQUIRE(raytable.getRay(RayTable::NORTH_WEST, a1) == 0x0);
    REQUIRE(raytable.getRay(RayTable::NORTH_WEST, d4) == 0x1020400000000);
    REQUIRE(raytable.getRay(RayTable::NORTH_WEST, e5) == 0x204080000000000);
    REQUIRE(raytable.getRay(RayTable::NORTH_WEST, h8) == 0x0);
  }

  SECTION("RayTable generates correct NorthEast rays for a1, d4, e5, h8") {
    REQUIRE(raytable.getRay(RayTable::NORTH_EAST, a1) == 0x8040201008040200);
    REQUIRE(raytable.getRay(RayTable::NORTH_EAST, d4) == 0x8040201000000000);
    REQUIRE(raytable.getRay(RayTable::NORTH_EAST, e5) == 0x8040200000000000);
    REQUIRE(raytable.getRay(RayTable::NORTH_EAST, h8) == 0x0);
  }

  SECTION("RayTable generates correct SouthWest rays for a1, d4, e5, h8") {
    REQUIRE(raytable.getRay(RayTable::SOUTH_WEST, a1) == 0x0);
    REQUIRE(raytable.getRay(RayTable::SOUTH_WEST, d4) == 0x40201);
    REQUIRE(raytable.getRay(RayTable::SOUTH_WEST, e5) == 0x8040201);
    REQUIRE(raytable.getRay(RayTable::SOUTH_WEST, h8) == 0x40201008040201);
  }

  SECTION("RayTable generates correct SouthEast rays for a1, d4, e5, h8") {
    REQUIRE(raytable.getRay(RayTable::SOUTH_EAST, a1) == 0x0);
    REQUIRE(raytable.getRay(RayTable::SOUTH_EAST, d4) == 0x102040);
    REQUIRE(raytable.getRay(RayTable::SOUTH_EAST, e5) == 0x20408000);
    REQUIRE(raytable.getRay(RayTable::SOUTH_EAST, h8) == 0x0);
  }
}
