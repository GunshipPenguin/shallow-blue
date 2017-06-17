#include "board.h"
#include "catch.hpp"

TEST_CASE("Board::getAttacks(WHITE) and Board::getAttacks(BLACK) work properly") {
  Board board;

  SECTION("Attack generation works correctly on an empty board") {
    board.setToFen("8/8/8/8/8/8/8/8 w - -");
    REQUIRE(board.getAttacks(WHITE) == U64(0));
    REQUIRE(board.getAttacks(BLACK) == U64(0));
  }

  SECTION("Attack generation works correctly for a single white pawn") {
    board.setToFen("8/8/8/8/8/8/3P4/8 w - -");
    REQUIRE(board.getAttacks(WHITE) == U64(0x140000));
  }

  SECTION("Attack generation works correctly for a single black pawn") {
      board.setToFen("8/3p4/8/8/8/8/8/8 w - -");
      REQUIRE(board.getAttacks(BLACK) == U64(0x140000000000));
  }

  SECTION("Attack generation works correctly for a single rook") {
    board.setToFen("8/8/8/8/3R4/8/8/8 w - -");
    REQUIRE(board.getAttacks(WHITE) == U64(0x8080808f7080808));

    board.setToFen("8/8/8/8/3r4/8/8/8 w - -");
    REQUIRE(board.getAttacks(BLACK) == U64(0x8080808f7080808));
  }

  SECTION("Attack generation works correctly for a single knight") {
    board.setToFen("8/8/8/8/3N4/8/8/8 w - -");
    REQUIRE(board.getAttacks(WHITE) == U64(0x142200221400));

    board.setToFen("8/8/8/8/3n4/8/8/8 w - -");
    REQUIRE(board.getAttacks(BLACK) == U64(0x142200221400));
  }

  SECTION("Attack generation works correctly for a single bishop") {
    board.setToFen("8/8/8/8/3B4/8/8/8 w - -");
    REQUIRE(board.getAttacks(WHITE) == U64(0x8041221400142241));

    board.setToFen("8/8/8/8/3b4/8/8/8 w - -");
    REQUIRE(board.getAttacks(BLACK) == U64(0x8041221400142241));
  }

  SECTION("Attack generation works correctly for a single Queen") {
    board.setToFen("8/8/8/8/3Q4/8/8/8 w - -");
    REQUIRE(board.getAttacks(WHITE) == U64(0x88492a1cf71c2a49));

    board.setToFen("8/8/8/8/3q4/8/8/8 w - -");
    REQUIRE(board.getAttacks(BLACK) == U64(0x88492a1cf71c2a49));
  }

  SECTION("Attack generation works correctly for a single king") {
    board.setToFen("8/8/8/8/3K4/8/8/8 w - -");
    REQUIRE(board.getAttacks(WHITE) == U64(0x1c141c0000));

    board.setToFen("8/8/8/8/3k4/8/8/8 w - -");
    REQUIRE(board.getAttacks(BLACK) == U64(0x1c141c0000));
  }

  SECTION("Attack generation works correctly from the starting position") {
    board.setToStartPos();
    REQUIRE(board.getAttacks(WHITE) == U64(0xff0000));
    REQUIRE(board.getAttacks(BLACK) == U64(0xff0000000000));
  }

  SECTION("Attack generation works for a position with multiple pieces having overlapping attacks") {
    board.setToFen("8/1R2r3/2n2B2/1p4q1/4P3/2r2P2/1Q4N1/2K1n3 w - -");
    REQUIRE(board.getAttacks(WHITE) == U64(0x825d027afa173d1b));
    REQUIRE(board.getAttacks(BLACK) == U64(0x5aeff0bdff7b4c04));
  }

  SECTION("Attack generation takes en passant into account") {
    board.setToFen("8/8/8/8/3p4/8/8/8 w - d3");
    REQUIRE(board.getAttacks(WHITE) == U64(0x80000));

    board.setToFen("8/8/8/8/3P4/8/8/8 w - d3");
    REQUIRE(board.getAttacks(BLACK) == U64(0x80000));
  }
}
