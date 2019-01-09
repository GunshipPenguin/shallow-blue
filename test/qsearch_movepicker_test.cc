#include "catch.hpp"
#include "qsearchmovepicker.h"
#include <iostream>

TEST_CASE("QSearchMovePicker works as expected") {
  Board board;

  SECTION("QSearchMovePicker does not return non captures") {
    board.setToFen("7k/8/5r2/2p5/8/P1R2n2/8/Kb6 w - -");
    MoveGen moveGen(board);
    MoveList moves = moveGen.getLegalMoves();

    QSearchMovePicker movePicker(&moves);

    // 3 captures for white on the test board
    for (int i = 0; i < 3; i++) {
      REQUIRE(movePicker.hasNext());
      movePicker.getNext();
    }

    // No more captures
    REQUIRE(!movePicker.hasNext());
  }

  SECTION("QSearchMovePicker returns captures sorted by MVV/LVA") {
    board.setToFen("7k/1p5B/4b3/8/3N4/1R3q2/6P1/K7 w - -");
    MoveGen moveGen(board);
    MoveList moves = moveGen.getLegalMoves();

    QSearchMovePicker movePicker(&moves);

    // g2xf3
    REQUIRE(movePicker.hasNext());
    Move m1(g2, f3, PAWN, Move::CAPTURE);
    m1.setCapturedPieceType(QUEEN);
    REQUIRE(movePicker.getNext() == m1);

    // d4xf3
    REQUIRE(movePicker.hasNext());
    Move m2(d4, f3, KNIGHT, Move::CAPTURE);
    m2.setCapturedPieceType(QUEEN);
    REQUIRE(movePicker.getNext() == m2);

    // b3xf3
    REQUIRE(movePicker.hasNext());
    Move m3(b3, f3, ROOK, Move::CAPTURE);
    m3.setCapturedPieceType(QUEEN);
    REQUIRE(movePicker.getNext() == m3);

    // d4xe6
    REQUIRE(movePicker.hasNext());
    Move m4(d4, e6, KNIGHT, Move::CAPTURE);
    m4.setCapturedPieceType(BISHOP);
    REQUIRE(movePicker.getNext() == m4);

    // b3xb7
    REQUIRE(movePicker.hasNext());
    Move m5(b3, b7, ROOK, Move::CAPTURE);
    m5.setCapturedPieceType(PAWN);
    REQUIRE(movePicker.getNext() == m5);

    // No more captures
    REQUIRE(!movePicker.hasNext());
  }
}