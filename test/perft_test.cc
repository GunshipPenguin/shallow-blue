#include "board.h"
#include "movegen.h"
#include "catch.hpp"

unsigned long long perft(int depth, const Board& board) {
  if (depth == 0) {
    return 1;
  } else if (depth == 1) {
    return MoveGen(board).getLegalMoves().size();
  }

  MoveGen movegen(board);

  unsigned long long nodes = 0;
  for (auto move : movegen.getLegalMoves()) {
    Board movedBoard = board;
    movedBoard.doMove(move);

    nodes += perft(depth - 1, movedBoard);
  }

  return nodes;
}

TEST_CASE("Perft is correct", "[perft]") {
  Board board;

  SECTION("Perft is correct from the starting position") {
    board.setToStartPos();

    REQUIRE(perft(1, board) == 20);
    REQUIRE(perft(2, board) == 400);
    REQUIRE(perft(3, board) == 8902);
    REQUIRE(perft(4, board) == 197281);
    REQUIRE(perft(5, board) == 4865609);
    REQUIRE(perft(6, board) == 119060324);

    // SLOW
    // REQUIRE(perft(7, board) == 3195901860);
    // REQUIRE(perft(8, board) == 84998978956);
  }

  SECTION("Perft is correct from r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -") {
    board.setToFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");

    REQUIRE(perft(1, board) == 48);
    REQUIRE(perft(2, board) == 2039);
    REQUIRE(perft(3, board) == 97862);
    REQUIRE(perft(4, board) == 4085603);
    REQUIRE(perft(5, board) == 193690690);
  }

  SECTION("Perft is correct from 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -") {
    board.setToFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");

    REQUIRE(perft(1, board) == 14);
    REQUIRE(perft(2, board) == 191);
    REQUIRE(perft(3, board) == 2812);
    REQUIRE(perft(4, board) == 43238);
    REQUIRE(perft(5, board) == 674624);

    // SLOW
    // REQUIRE(perft(7, board) == 178633661);
    // REQUIRE(perft(6, board) == 11030083);
  }

  SECTION("Perft is correct from r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1") {
    board.setToFen("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    REQUIRE(perft(1, board) == 6);
    REQUIRE(perft(2, board) == 264);
    REQUIRE(perft(3, board) == 9467);
    REQUIRE(perft(4, board) == 422333);
    REQUIRE(perft(5, board) == 15833292);

    // SLOW
    // REQUIRE(perft(6, board) == 706045033);
  }

  SECTION("Perft is correct from rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 ") {
    board.setToFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 ");

    REQUIRE(perft(1, board) == 44);
    REQUIRE(perft(2, board) == 1486);
    REQUIRE(perft(3, board) == 62379);
    REQUIRE(perft(4, board) == 2103487);
    REQUIRE(perft(5, board) == 89941194);
  }

  SECTION("Perft is correct from r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10") {
    board.setToFen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

    REQUIRE(perft(1, board) == 46);
    REQUIRE(perft(2, board) == 2079);
    REQUIRE(perft(3, board) == 89890);
    REQUIRE(perft(4, board) == 3894594);
    REQUIRE(perft(5, board) == 164075551);

    // SLOW
    // REQUIRE(perft(6, board) == 6923051137);
    // REQUIRE(perft(7, board) == 287188994746);
    // REQUIRE(perft(8, board) == 11923589843526);
    // REQUIRE(perft(9, board) == 490154852788714);
  }

  SECTION("Perft is correct from n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1") {
    board.setToFen("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");

    REQUIRE(perft(1, board) == 24);
    REQUIRE(perft(2, board) == 496);
    REQUIRE(perft(3, board) == 9483);
    REQUIRE(perft(4, board) == 182838);
    REQUIRE(perft(5, board) == 3605103);
    REQUIRE(perft(6, board) == 71179139);
  }

  SECTION("Perft is correct from k1q4b/1qq3bR/qq3b1R/r2Rr2R/r2rR2R/r1B3QQ/rB3QQ1/B4Q1K w - -") {
    board.setToFen("k1q4b/1qQ3bR/qQ3b1R/r2RR2R/r2rr2R/r1B3qQ/rB3qQ1/B4Q1K w - -");

    REQUIRE(perft(1, board) == 78);
    REQUIRE(perft(2, board) == 5451);
    REQUIRE(perft(3, board) == 388081);
    REQUIRE(perft(4, board) == 27735148);
  }
}
