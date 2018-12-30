#include "search.h"
#include "catch.hpp"

TEST_CASE("Search works as expected") {
  Board board;
  std::vector<ZKey> emptyPositionHistory;
  Search::Limits limits;
  limits.depth = 8;

  SECTION("Search finds the fool's mate checkmakte on the next move") {
    board.setToFen("rnbqkbnr/pppp1ppp/4p3/8/6P1/5P2/PPPPP2P/RNBQKBNR b KQkq -");

    Search search(board, limits, emptyPositionHistory, false);
    search.iterDeep();

    REQUIRE(search.getBestMove().getNotation() == "d8h4");
  }

  SECTION("Search returns the only legal move when checkmate is 1 move away") {
    board.setToFen("r4rk1/ppp2ppp/4p3/8/4p3/4PPbP/PPPB2q1/R2QKR2 w - -");

    Search search(board, limits, emptyPositionHistory, false);
    search.iterDeep();

    REQUIRE(search.getBestMove().getNotation() == "f1f2");
  }

  SECTION("Search recognizes when a check can be made to capture a queen") {
    board.setToFen("8/4N3/8/1k5q/8/8/8/2K2R2 w - -");

    Search search(board, limits, emptyPositionHistory, false);
    search.iterDeep();

    REQUIRE(search.getBestMove().getNotation() == "f1f5");
  }

  SECTION("Search finds a checkmate on the next move") {
    board.setToFen("2kr3r/pp4pp/4N3/q7/2K5/8/PR1b2PP/8 b - - 7 33");

    Search search(board, limits, emptyPositionHistory, false);
    search.iterDeep();

    REQUIRE(search.getBestMove().getNotation() == "a5d5");
  }

  SECTION("Bratko-Kopec test #1 is correct") {
    board.setToFen("1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -");

    Search search(board, limits, emptyPositionHistory, false);
    search.iterDeep();

    REQUIRE(search.getBestMove().getNotation() == "d6d1");
  }

  SECTION("Search recognizes when a repetition draw is the best option") {
    std::vector<ZKey> moveHistory;
    moveHistory.push_back(Board("6Q1/pp6/8/8/1kp2N2/1n2R1P1/3r4/1K6 b - -").getZKey());

    board.setToFen("6Q1/pp6/8/8/1kp2N2/1n2R1P1/K7/3r4 b - -");

    Search search(board, limits, moveHistory, false);
    search.iterDeep();

    REQUIRE(search.getBestMove().getNotation() == "d1d2");
  }

  SECTION("Search recognizes when a 50 move rule draw is the best option") {
    board.setToFen("B6k/1r6/8/8/7q/8/PP6/K7 w - - 49");

    Search search(board, limits, emptyPositionHistory, false);
    search.iterDeep();

    REQUIRE(search.getBestMove().getNotation() == "a1b1");
  }
}
