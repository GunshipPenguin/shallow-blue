#include "eval.h"
#include "catch.hpp"
#include "defs.h"

TEST_CASE("Evaluation functions work properly") {
  Eval::init();
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

  SECTION("Backward pawn calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::isolatedPawns(board, WHITE) == 0);
    REQUIRE(Eval::isolatedPawns(board, BLACK) == 0);

    // One backward pawn for white
    board.setToFen("rnbqkbnr/pppp1ppp/8/4p3/4P3/3P4/PPP2PPP/RNBQKBNR b KQkq -");
    REQUIRE(Eval::backwardPawns(board, WHITE) == 1);
    REQUIRE(Eval::backwardPawns(board, BLACK) == 0);

    // Two backward pawns for black
    board.setToFen("rnbqkbnr/pp4pp/3p1p2/2p1p3/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -");
    REQUIRE(Eval::backwardPawns(board, WHITE) == 0);
    REQUIRE(Eval::backwardPawns(board, BLACK) == 2);
  }

  SECTION("Mobility calculations are correct") {
    board.setToStartPos();
    REQUIRE(Eval::mobility(board, WHITE) == 20);
    REQUIRE(Eval::mobility(board, BLACK) == 20);

    board.setToFen("7k/8/4b3/8/3N4/8/4P3/K7 w - -");
    REQUIRE(Eval::mobility(board, WHITE) == 12);
    REQUIRE(Eval::mobility(board, BLACK) == 14);

    board.setToFen("7k/8/8/8/4q3/8/8/K7 w - -");
    REQUIRE(Eval::mobility(board, WHITE) == 3);
    REQUIRE(Eval::mobility(board, BLACK) == 30);

    board.setToFen("7k/8/8/3Rr3/8/8/8/K7 w - -");
    REQUIRE(Eval::mobility(board, WHITE) == 14);
    REQUIRE(Eval::mobility(board, BLACK) == 14);
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
}
