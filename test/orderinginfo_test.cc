#include "catch.hpp"
#include "orderinginfo.h"

TEST_CASE("OrderingInfo class works correctly") {
  // Empty transposition table to use in OrderingInfo constructor when a TT is not required
  const TranspTable emptyTt;
  const TranspTable *emptyTtPointer = const_cast<TranspTable *>(&emptyTt);

  SECTION("OrderingInfo stores and adjusts ply information correctly") {
    OrderingInfo orderingInfo(emptyTtPointer);

    REQUIRE(orderingInfo.getPly() == 0);

    orderingInfo.incrementPly();
    REQUIRE(orderingInfo.getPly() == 1);

    orderingInfo.deincrementPly();
    REQUIRE(orderingInfo.getPly() == 0);
  }

  SECTION("OrderingInfo stores transposition table information correctly") {
    TranspTable tt;
    TranspTable *ttPointer = const_cast<TranspTable *>(&tt);

    // Dummy Transposition table entry and values
    int score = 1;
    int depth = 2;
    TranspTableEntry::Flag flag = TranspTableEntry::UPPER_BOUND;
    Move bestMove(a1, a2, ROOK);
    TranspTableEntry entry(1, 2, TranspTableEntry::UPPER_BOUND, bestMove);

    // Add an entry to tt for an empty board with the dummy tt entry
    Board board;
    ZKey zkey(board);
    ttPointer->set(zkey, entry);

    OrderingInfo orderingInfo(ttPointer);

    REQUIRE(orderingInfo.getTt()->getEntry(zkey)->getScore() == 1);
    REQUIRE(orderingInfo.getTt()->getEntry(zkey)->getDepth() == 2);
    REQUIRE(orderingInfo.getTt()->getEntry(zkey)->getFlag() == TranspTable::UPPER_BOUND);
    REQUIRE(orderingInfo.getTt()->getEntry(zkey)->getBestMove() == bestMove);
  }

  SECTION("OrderingInfo stores killer moves correctly") {
    OrderingInfo orderingInfo(emptyTtPointer);

    // 2 killer moves at ply 1 and 2
    Move killer1Ply1(a1, a2, ROOK);
    Move killer2Ply1(h8, h7, ROOK);

    Move killer1Ply2(a2, a3, QUEEN);
    Move killer2Ply2(h7, h6, QUEEN);

    // Insert killers
    orderingInfo.updateKillers(1, killer2Ply1);
    orderingInfo.updateKillers(1, killer1Ply1);

    orderingInfo.updateKillers(2, killer2Ply2);
    orderingInfo.updateKillers(2, killer1Ply2);

    // Test returned killers
    REQUIRE(orderingInfo.getKiller1(1) == killer1Ply1);
    REQUIRE(orderingInfo.getKiller2(1) == killer2Ply1);

    REQUIRE(orderingInfo.getKiller1(2) == killer1Ply2);
    REQUIRE(orderingInfo.getKiller2(2) == killer2Ply2);
  }

  SECTION("OrderingInfo increments history information correctly") {
    OrderingInfo orderingInfo(emptyTtPointer);

    orderingInfo.incrementHistory(WHITE, 1, 2, 1);
    REQUIRE(orderingInfo.getHistory(WHITE, 1, 2) > 0);
  }

  SECTION("OrderingInfo increments history values more for shallower depths") {
    OrderingInfo orderingInfo(emptyTtPointer);

    // Square 1 -> Square 2
    orderingInfo.incrementHistory(WHITE, 1, 2, 1);
    int deepHistory = orderingInfo.getHistory(WHITE, 1, 2);

    // Square 3 -> Square 4
    orderingInfo.incrementHistory(WHITE, 2, 3, 5);
    int shallowHistory = orderingInfo.getHistory(WHITE, 2, 3);

    REQUIRE(shallowHistory > deepHistory);
  }
}

