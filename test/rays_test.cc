#include "rays.h"
#include "catch.hpp"

TEST_CASE("Ray generation is correct") {
  Rays::init();

  SECTION("North rays are correct") {
    REQUIRE(Rays::getRay(Rays::NORTH, 0) == 0x0101010101010100ULL);
    REQUIRE(Rays::getRay(Rays::NORTH, 14) == 0x4040404040400000ULL);
  }
};
