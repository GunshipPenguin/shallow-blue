#include "catch.hpp"
#include "transptable.h"
#include <iostream>

TEST_CASE("Transposition tables work as expected") {
  ZKey::init();

  SECTION("Transposition tables work as expected after ZKey::init is called") {
    Board board;
    TranspTable tt;

    SECTION("Transposition tables store depth, score and flags") {
      board.setToStartPos();

      tt.set(board.getZKey(), 1, 2, TranspTable::EXACT);

      REQUIRE(tt.contains(board.getZKey()));
      REQUIRE(tt.getScore(board.getZKey()) == 1);
      REQUIRE(tt.getDepth(board.getZKey()) == 2);
      REQUIRE(tt.getFlag(board.getZKey()) == TranspTable::EXACT);
    }

    SECTION("Transposition tables store multiple board keys separately") {
      board.setToStartPos();
      ZKey key1 = board.getZKey();

      board.setToFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -");
      ZKey key2 = board.getZKey();

      tt.set(key1, 1, 1, TranspTable::EXACT);
      tt.set(key2, 2, 2, TranspTable::UPPER_BOUND);

      REQUIRE(tt.contains(key1));
      REQUIRE(tt.contains(key2));

      REQUIRE(tt.getScore(key1) == 1);
      REQUIRE(tt.getDepth(key1) == 1);
      REQUIRE(tt.getFlag(key1) == TranspTable::EXACT);


      REQUIRE(tt.getScore(key2) == 2);
      REQUIRE(tt.getDepth(key2) == 2);
      REQUIRE(tt.getFlag(key2) == TranspTable::UPPER_BOUND);
    }

    SECTION("Transposition tables are cleared when clear() is called") {
      board.setToStartPos();

      tt.set(board.getZKey(), 5, 5, TranspTable::EXACT);

      REQUIRE(tt.contains(board.getZKey()));
      tt.clear();
      REQUIRE(tt.contains(board.getZKey()) == false);
    }
  }
}
