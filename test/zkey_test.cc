#include "zkey.h"
#include "defs.h"
#include "board.h"
#include "catch.hpp"

TEST_CASE("Zobrist hashing works properly") {
  ZKey::init();

  SECTION("Zorbrist hashing works after ZKey::init() is called") {
    Board board;

    SECTION("Zorbrist hash generates a nonzero hash for the starting position") {
      board.setToStartPos();

      ZKey key(board);

      REQUIRE(key.getValue() != 0);
    }

    SECTION("Keys constructed with different boards have different values") {
      board.setToFen("rn1qkbnr/pp2pppp/2p5/5b2/3PN3/8/PPP2PPP/R1BQKBNR w KQkq -");
      ZKey key1(board);

      board.setToFen("r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R w KQkq -");
      ZKey key2(board);

      REQUIRE(key1.getValue() != key2.getValue());
    }
  }
}
