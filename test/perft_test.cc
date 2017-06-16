#include "board.h"
#include "movegen.h"
#include "catch.hpp"

int perft(int depth, Board board) {
  if (depth == 0) {
    return 1;
  }

  MoveGen movegen(board);

  int nodes = 0;
  for (auto move : movegen.getMoves()) {
    Board tempBoard = board;
    tempBoard.doMove(move);

    if (!tempBoard.WHITE_TO_MOVE && tempBoard.whiteIsInCheck()) {
      continue;
    } else if (tempBoard.WHITE_TO_MOVE && tempBoard.blackIsInCheck()) {
      continue;
    }

    nodes += perft(depth - 1, tempBoard);
  }

  return nodes;
}

TEST_CASE("Perft is correct") {
  Board board;

  SECTION("Perft is correct from the starting position") {
    board.setToStartPos();
    REQUIRE(perft(1, board) == 20);
    REQUIRE(perft(2, board) == 400);
    REQUIRE(perft(3, board) == 8902);
    REQUIRE(perft(4, board) == 197281);
  }

  SECTION("Perft is correct from r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -") {
    board.setToFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");

    REQUIRE(perft(1, board) == 48);
    REQUIRE(perft(2, board) == 2039);
    REQUIRE(perft(3, board) == 97862);
    REQUIRE(perft(4, board) == 4085603);
  //  REQUIRE(perft(5, board) == 193690690); // SLOW
  }

  SECTION("Perft is correct from 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -") {
    board.setToFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");

    REQUIRE(perft(1, board) == 14);
    REQUIRE(perft(2, board) == 191);
    REQUIRE(perft(3, board) == 2812);
    REQUIRE(perft(4, board) == 43238);
    REQUIRE(perft(5, board) == 674624);
  //  REQUIRE(perft(6, board) == 11030083); // SLOW
  }

  SECTION("Perft is correct from r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1") {
    board.setToFen("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    REQUIRE(perft(1, board) == 6);
    REQUIRE(perft(2, board) == 264);
    REQUIRE(perft(3, board) == 9467);
    REQUIRE(perft(4, board) == 422333);
    // REQUIRE(perft(5, board) == 15833292); // SLOW
  }

  SECTION("Perft is correct from rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 ") {
    board.setToFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 ");

    REQUIRE(perft(1, board) == 44);
    REQUIRE(perft(2, board) == 1486);
    REQUIRE(perft(3, board) == 62379);
    REQUIRE(perft(4, board) == 2103487);
    // REQUIRE(perft(5, board) == 89941194); // SLOW
  }

  SECTION("Perft is correct from r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10") {
    board.setToFen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

    REQUIRE(perft(1, board) == 46);
    REQUIRE(perft(2, board) == 2079);
    REQUIRE(perft(3, board) == 89890);
    REQUIRE(perft(4, board) == 3894594);
  }
}