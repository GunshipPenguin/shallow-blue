#include "catch.hpp"
#include "transptable.h"
#include <iostream>

TEST_CASE("Transposition tables work as expected") {
  ZKey::init();

  SECTION("Transposition tables work as expected after ZKey::init is called") {
    Board board;
    TranspTable tt;

    SECTION("Transposition tables store board keys") {
      board.setToStartPos();

      tt.add(board.getZKey(), 5);

      REQUIRE(tt.contains(board.getZKey()));
      REQUIRE(tt.get(board.getZKey()) == 5);
    }

    SECTION("Transposition tables store multiple board keys separately") {
      board.setToStartPos();
      ZKey key1 = board.getZKey();

      board.setToFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -");
      ZKey key2 = board.getZKey();

      tt.add(key1, 1);
      tt.add(key2, 2);

      REQUIRE(tt.contains(key1));
      REQUIRE(tt.contains(key2));
      REQUIRE(tt.get(key1) == 1);
      REQUIRE(tt.get(key2) == 2);
    }

    SECTION("Transposition tables are cleared when clear() is called") {
      board.setToStartPos();

      tt.add(board.getZKey(), 5);

      REQUIRE(tt.contains(board.getZKey()));
      tt.clear();
      REQUIRE(tt.contains(board.getZKey()) == false);
    }
  }
}
