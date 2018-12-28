#include "eval.h"
#include "catch.hpp"

TEST_CASE("Evaluation functions work properly") {
  Board board;

  SECTION("Eval returns 0 for black and white from the start pos") {
    board.setToStartPos();

    REQUIRE(Eval::evaluate(board, WHITE) == 0);
    REQUIRE(Eval::evaluate(board, BLACK) == 0);
  }

  SECTION("White's eval is the negative of black's eval") {
    board.setToFen("rnbqkb2/pp3ppp/4pn2/3p2B1/3PP3/2N5/PPP2PPP/R2QKB1R b KQkq -");

    REQUIRE(Eval::evaluate(board, WHITE) == -Eval::evaluate(board, BLACK));
  }

  SECTION("Doubled pawn calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::doubledPawns(board, WHITE) == 0);
    REQUIRE(Eval::doubledPawns(board, BLACK) == 0);

    // One doubled pawn for white
    board.setToFen("rnbqkbnr/pppp1ppp/8/4p3/2B1P3/4P3/PPPP2PP/RNBQK1NR b KQkq -");
    REQUIRE(Eval::doubledPawns(board, WHITE) == 1);
    REQUIRE(Eval::doubledPawns(board, BLACK) == 0);

    // Two doubled pawns for black
    board.setToFen("rnbqkbnr/1ppp2pp/1p2p3/4p3/2B1P3/8/PPPP1PPP/RNBQK1NR b KQkq -");
    REQUIRE(Eval::doubledPawns(board, WHITE) == 0);
    REQUIRE(Eval::doubledPawns(board, BLACK) == 2);

    // Three pawns on one file for white (counts as two doubled pawns)
    board.setToFen("rnbqkbnr/pppp1ppp/8/4p3/2B1P3/4P3/PPP1P1PP/RNBQK1NR b KQkq -");
    REQUIRE(Eval::doubledPawns(board, WHITE) == 2);
    REQUIRE(Eval::doubledPawns(board, BLACK) == 0);
  }

  SECTION("Isolated pawn calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::isolatedPawns(board, WHITE) == 0);
    REQUIRE(Eval::isolatedPawns(board, BLACK) == 0);

    // One isolated pawn for white
    board.setToFen("rnbqkbnr/pppp1ppp/8/8/3pP3/2P5/PP4PP/RNBQKBNR b KQkq -");
    REQUIRE(Eval::isolatedPawns(board, WHITE) == 1);
    REQUIRE(Eval::isolatedPawns(board, BLACK) == 0);

    // Two isolated pawns for black
    board.setToFen("rnbqkbnr/pp4p1/8/8/3pP3/2P5/PP3PPP/RNBQKBNR b KQkq -");
    REQUIRE(Eval::isolatedPawns(board, WHITE) == 0);
    REQUIRE(Eval::isolatedPawns(board, BLACK) == 2);
  }

  SECTION("Mobility evaluations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::evaluateMobility(board, OPENING, WHITE) == Eval::evaluateMobility(board, OPENING, BLACK));

    // Queen moves should carry some weight in the endgame
    board.setToFen("7k/8/8/8/4q3/8/8/K7 w - -");
    REQUIRE(Eval::evaluateMobility(board, ENDGAME, WHITE) < Eval::evaluateMobility(board, ENDGAME, BLACK));

    board.setToFen("7k/8/8/3Rr3/8/8/8/K7 w - -");
    REQUIRE(Eval::evaluateMobility(board, OPENING, WHITE) == Eval::evaluateMobility(board, OPENING, BLACK));
  }

  SECTION("Bishop pair calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::hasBishopPair(board, WHITE));
    REQUIRE(Eval::hasBishopPair(board, BLACK));

    // White and black have one bishop on black
    board.setToFen("7k/4b3/8/8/8/2B5/8/K7 w - -");
    REQUIRE(!Eval::hasBishopPair(board, WHITE));
    REQUIRE(!Eval::hasBishopPair(board, BLACK));

    // White and black have two bishops, each on the same color
    board.setToFen("3b3k/4b3/8/8/8/2B5/3B4/K7 w - -");
    REQUIRE(!Eval::hasBishopPair(board, WHITE));
    REQUIRE(!Eval::hasBishopPair(board, BLACK));

    // White has one bishop, black has two on different colors
    board.setToFen("3bb2k/8/8/8/8/2B5/8/K7 w - -");
    REQUIRE(!Eval::hasBishopPair(board, WHITE));
    REQUIRE(Eval::hasBishopPair(board, BLACK));
  }

  SECTION("Rook on open file calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::rooksOnOpenFiles(board, WHITE) == 0);
    REQUIRE(Eval::rooksOnOpenFiles(board, BLACK) == 0);

    // White has one rook on an open file
    board.setToFen("7k/8/8/8/8/2R5/8/K7 w - -");
    REQUIRE(Eval::rooksOnOpenFiles(board, WHITE) == 1);
    REQUIRE(Eval::rooksOnOpenFiles(board, BLACK) == 0);

    // White has one rook on a non open file, black has two on open files
    board.setToFen("1r5k/6r1/2N5/2p5/8/2R5/8/K7 w - -");
    REQUIRE(Eval::rooksOnOpenFiles(board, WHITE) == 0);
    REQUIRE(Eval::rooksOnOpenFiles(board, BLACK) == 2);
  }

  SECTION("Pawn shield calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::pawnsShieldingKing(board, WHITE) == 3);
    REQUIRE(Eval::pawnsShieldingKing(board, BLACK) == 3);

    // Kings on A and H files
    board.setToFen("7k/5ppp/8/8/8/8/PPP5/K7 w - -");
    REQUIRE(Eval::pawnsShieldingKing(board, WHITE) == 2);
    REQUIRE(Eval::pawnsShieldingKing(board, BLACK) == 2);

    // 2 pawns each
    board.setToFen("6k1/5pp1/8/8/8/8/3PP3/3K4 w - -");
    REQUIRE(Eval::pawnsShieldingKing(board, WHITE) == 2);
    REQUIRE(Eval::pawnsShieldingKing(board, BLACK) == 2);

    // 1 pawn shielding white, 2 shielding black
    board.setToFen("2k5/2pp4/8/8/8/8/2P5/2K5 w - -");
    REQUIRE(Eval::pawnsShieldingKing(board, WHITE) == 1);
    REQUIRE(Eval::pawnsShieldingKing(board, BLACK) == 2);

    // No pawn shield if not on rank 2 (white) or rank 7 (black)
    board.setToFen("8/2k5/1ppp4/8/8/6PP/6K1/8 w KQkq -");
    REQUIRE(Eval::pawnsShieldingKing(board, WHITE) == 0);
    REQUIRE(Eval::pawnsShieldingKing(board, BLACK) == 0);
  }

  SECTION("Passed pawn calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::passedPawns(board, WHITE) == 0);
    REQUIRE(Eval::passedPawns(board, BLACK) == 0);

    board.setToFen("rnbqkbnr/ppppp3/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    REQUIRE(Eval::passedPawns(board, WHITE) == 2);
    REQUIRE(Eval::passedPawns(board, BLACK) == 0);

    board.setToFen("k7/4p3/8/8/8/1P6/4P3/7K w KQkq -");
    REQUIRE(Eval::passedPawns(board, WHITE) == 1);
    REQUIRE(Eval::passedPawns(board, BLACK) == 0);

    board.setToFen("k7/4p3/p6P/8/8/8/8/7K w KQkq -");
    REQUIRE(Eval::passedPawns(board, WHITE) == 1);
    REQUIRE(Eval::passedPawns(board, BLACK) == 2);

    board.setToFen("4k3/1p2p3/1p6/8/7P/2P4P/8/4K3 w KQkq -");
    REQUIRE(Eval::passedPawns(board, WHITE) == 1);
    REQUIRE(Eval::passedPawns(board, BLACK) == 1);
  }

  SECTION("Phase calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::getPhase(board) == 0);

    board.setToFen("r1bq1rk1/ppp3pp/2np1p2/4p3/3PP3/N1P2P2/PP4PP/R1BQ1RK1 b KQkq -");
    REQUIRE(Eval::getPhase(board) < Eval::MAX_PHASE);
    REQUIRE(Eval::getPhase(board) > 0);

    board.setToFen("6k1/8/8/8/8/8/1K6/8 b KQkq -");
    REQUIRE(Eval::getPhase(board) == Eval::MAX_PHASE);
  }
}
