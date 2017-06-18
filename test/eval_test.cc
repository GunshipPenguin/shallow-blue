#include "eval.h"
#include "catch.hpp"
#include "defs.h"

TEST_CASE("Evaluation works as expected") {
  Board board;

  SECTION("Eval returns 0 for black and white from the start pos") {
    board.setToStartPos();

    Eval whiteEval(board, WHITE);
    Eval blackEval(board, BLACK);

    REQUIRE(whiteEval.getScore() == 0);
    REQUIRE(whiteEval.getScore() == 0);
  }

  SECTION("White's eval is the negative of black's eval") {
    board.setToFen("rnbqkb2/pp3ppp/4pn2/3p2B1/3PP3/2N5/PPP2PPP/R2QKB1R b KQkq -");

    Eval whiteEval(board, WHITE);
    Eval blackEval(board, BLACK);

    REQUIRE(whiteEval.getScore() == -blackEval.getScore());
  }
}
