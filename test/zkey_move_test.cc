#include "zkey.h"
#include "board.h"
#include "catch.hpp"

TEST_CASE("Boards update ZKeys correctly after moves") {
  Board board;

  SECTION("A board at the start position will have a nonzero key") {
    board.setToStartPos();

    REQUIRE(board.getZKey().getValue() != 0);
  }

  SECTION("ZKeys are updated properly after quiet pawn moves") {
    board.setToStartPos();

    board.doMove(Move(e2, e3, PAWN));
    U64 initValue = board.getZKey().getValue();

    board.setToFen("rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq -");
    REQUIRE(initValue == board.getZKey().getValue());
  }

  SECTION("ZKeys are updated after quiet bishop moves") {
    board.setToFen("8/8/8/8/8/4B3/8/8 w - -");

    board.doMove(Move(e3, f2, BISHOP));
    U64 initValue = board.getZKey().getValue();

    board.setToFen("8/8/8/8/8/8/5B2/8 b - -");
    REQUIRE(initValue == board.getZKey().getValue());
  }

  SECTION("ZKeys are updated properly after captures") {
    board.setToFen("8/8/8/2n5/8/4B3/8/8 w - -");

    Move move(e3, c5, BISHOP, Move::CAPTURE);
    move.setCapturedPieceType(KNIGHT);

    board.doMove(move);

    U64 initValue = board.getZKey().getValue();

    board.setToFen("8/8/8/2B5/8/8/8/8 b - -");
    REQUIRE(board.getZKey().getValue() == initValue);
  }

  SECTION("ZKeys are updated properly after double pawn pushes (for en passant)") {
    board.setToStartPos();

    board.doMove(Move(e2, e4, PAWN, Move::DOUBLE_PAWN_PUSH));
    U64 initValue = board.getZKey().getValue();

    board.setToFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3");
    REQUIRE(board.getZKey().getValue() == initValue);
  }

  SECTION("ZKeys are updated properly after castling rights are changed by a move") {
    board.setToFen("8/8/8/8/8/6n1/8/4K2R b K -");

    Move move(g3, h1, KNIGHT, Move::CAPTURE);
    move.setCapturedPieceType(ROOK);

    board.doMove(move);

    U64 initValue = board.getZKey().getValue();

    board.setToFen("8/8/8/8/8/8/8/4K2n w - -");
    REQUIRE(board.getZKey().getValue() == initValue);
  }
}
