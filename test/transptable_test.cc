#include "catch.hpp"
#include "transptable.h"
#include <iostream>

TEST_CASE("Transposition tables work as expected") {
  Board board;
  TranspTable tt;

  SECTION("Transposition tables store multiple board keys separately") {
    board.setToStartPos();
    ZKey key1 = board.getZKey();

    board.setToFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -");
    ZKey key2 = board.getZKey();

    Move move1(a1, a2, PAWN);
    TranspTableEntry ttEntry1(1, 2, TranspTableEntry::EXACT, move1);

    Move move2(a3, a4, ROOK);
    TranspTableEntry ttEntry2(3, 4, TranspTableEntry::UPPER_BOUND, move2);

    tt.set(key1, ttEntry1);
    tt.set(key2, ttEntry2);

    const TranspTableEntry *storedEntry1 = tt.getEntry(key1);
    REQUIRE(storedEntry1 != nullptr);
    REQUIRE(storedEntry1->getScore() == 1);
    REQUIRE(storedEntry1->getDepth() == 2);
    REQUIRE(storedEntry1->getFlag() == TranspTable::EXACT);
    REQUIRE(storedEntry1->getBestMove() == move1);

    const TranspTableEntry *storedEntry2 = tt.getEntry(key2);
    REQUIRE(storedEntry1 != nullptr);
    REQUIRE(storedEntry2->getScore() == 3);
    REQUIRE(storedEntry2->getDepth() == 4);
    REQUIRE(storedEntry2->getFlag() == TranspTable::UPPER_BOUND);
    REQUIRE(storedEntry2->getBestMove() == move2);
  }

  SECTION("Transposition tables return nullptr when getEntry is called for a key that does not exist") {
    ZKey key; // Not stored
    REQUIRE(tt.getEntry(key) == nullptr);
  }

  SECTION("Transposition tables are cleared when clear() is called") {
    board.setToStartPos();

    Move move(a1, a2, PAWN);
    TranspTableEntry ttEntry(5, 5, TranspTableEntry::EXACT, move);
    tt.set(board.getZKey(), ttEntry);

    REQUIRE(tt.getEntry(board.getZKey()) != nullptr);
    tt.clear();
    REQUIRE(tt.getEntry(board.getZKey()) == nullptr);
  }
}
