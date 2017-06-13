#include "board.h"
#include "defs.h"
#include "cmove.h"
#include "catch.hpp"
#include <iostream>

TEST_CASE("Board::doMove works properly") {
  Board board;

  SECTION("doMove moves pawns up from the starting position for white") {
    board.setToStartPos();

    CMove move(a2, a4);

    board.doMove(move);

    REQUIRE( (board.WHITE_PAWNS & (ONE << a4)) );
  }

  SECTION("doMove moves pawns up from the starting position for black") {
    board.setToFen("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq -");

    CMove move(a7, a5);

    board.doMove(move);

    REQUIRE( (board.BLACK_PAWNS & (ONE << a5)) );
  }

  SECTION("doMove handles captures") {
    board.setToFen("8/8/8/4n3/8/2B5/8/8 w - -");

    CMove move(c3, e5, CMove::CAPTURE);

    board.doMove(move);

    REQUIRE( (board.WHITE_BISHOPS & (ONE << e5)) );
    REQUIRE(board.BLACK_KNIGHTS == 0ull);
  }

  SECTION("doMove handles en passant for black") {
    board.setToFen("8/8/8/8/2Pp4/8/8/8 b kq c3");

    CMove move(d4, c3, CMove::EN_PASSANT);

    board.doMove(move);

    REQUIRE( (board.BLACK_PAWNS & (ONE << c3)) );
    REQUIRE(board.WHITE_PAWNS == 0ull);
    REQUIRE(board.EN_PASSANT == 0ull);
  }

  SECTION("doMove handles en passant for white") {
    board.setToFen("8/8/8/2pP4/8/8/8/8 w kq c6");

    CMove move(d5, c6, CMove::EN_PASSANT);

    board.doMove(move);

    REQUIRE( (board.WHITE_PAWNS & (ONE << c6)) );
    REQUIRE(board.BLACK_PAWNS == 0ull);
    REQUIRE(board.EN_PASSANT == 0ull);
  }

  SECTION("doMove handles white kingside castles") {
    board.setToFen("8/8/8/8/8/8/8/4K2R w - -");
    CMove move(e1, g1, CMove::KSIDE_CASTLE);

    board.doMove(move);
    REQUIRE( (board.WHITE_KING & (ONE << g1)) );
    REQUIRE( (board.WHITE_ROOKS & (ONE << f1)) );
  }

  SECTION("doMove handles white queenside castles") {
    board.setToFen("8/8/8/8/8/8/8/4K2R w - -");
    CMove move(e1, c1, CMove::QSIDE_CASTLE);

    board.doMove(move);

    REQUIRE( (board.WHITE_KING & (ONE << c1)) );
    REQUIRE( (board.WHITE_ROOKS & (ONE << d1)) );
  }

  SECTION("doMove handles black kingside castles") {
    board.setToFen("4k2r/8/8/8/8/8/8/8 b - -");
    CMove move(e8, g8, CMove::KSIDE_CASTLE);

    board.doMove(move);

    REQUIRE( (board.BLACK_KING & (ONE << g8)) );
    REQUIRE( (board.BLACK_ROOKS & (ONE << f8)) );
  }

  SECTION("doMove handles black queenside castles") {
    board.setToFen("r3k3/8/8/8/8/8/8/8 b - -");
    CMove move(e8, c8, CMove::QSIDE_CASTLE);

    board.doMove(move);

    REQUIRE( (board.BLACK_KING & (ONE << c8)) );
    REQUIRE( (board.BLACK_ROOKS & (ONE << d8)) );
  }

  SECTION("White cannot castle after moving its king") {
    board.setToFen("8/8/8/8/8/8/8/4K2R w KQ -");

    REQUIRE(board.whiteCanCastleKs() == true);
    REQUIRE(board.whiteCanCastleQs() == true);

    CMove move(e1, e2);
    board.doMove(move);

    REQUIRE(board.whiteCanCastleKs() == false);
    REQUIRE(board.whiteCanCastleQs() == false);
  }

  SECTION("White cannot castle after moving its king") {
    board.setToFen("4k2r/8/8/8/8/8/8/8 w kq -");

    REQUIRE(board.blackCanCastleKs() == true);
    REQUIRE(board.blackCanCastleQs() == true);

    CMove move(e8, e7);
    board.doMove(move);

    REQUIRE(board.blackCanCastleKs() == false);
    REQUIRE(board.blackCanCastleQs() == false);
  }

  SECTION("doMove works with white promotions") {
    board.setToFen("8/3P4/8/8/8/8/8/8 w - -");

    CMove move(d7, d8, CMove::QUEEN_PROMOTION);

    board.doMove(move);

    REQUIRE(board.WHITE_PAWNS == 0ull);
    REQUIRE(board.WHITE_QUEENS == (ONE << d8));
  }

  SECTION("doMove works with black promotions") {
    board.setToFen("8/8/8/8/8/8/3p4/8 b - -");

    CMove move(d2, d1, CMove::QUEEN_PROMOTION);

    board.doMove(move);

    REQUIRE(board.BLACK_PAWNS == 0ull);
    REQUIRE(board.BLACK_QUEENS == (ONE << d1));
  }

  SECTION("doMove works with white capture promotions") {
    board.setToFen("2q5/3P4/8/8/8/8/8/8 w - -");

    CMove move(d7, c8, CMove::CAPTURE | CMove::KNIGHT_PROMOTION);
    board.doMove(move);

    REQUIRE(board.WHITE_PAWNS == 0ull);
    REQUIRE(board.BLACK_QUEENS == 0ull);
    REQUIRE(board.WHITE_KNIGHTS == (ONE << c8));
  }

  SECTION("doMove works with black capture promotions") {
    board.setToFen("8/8/8/8/8/8/3p4/2Q5 b - -");

    CMove move(d2, c1, CMove::CAPTURE | CMove::KNIGHT_PROMOTION);
    board.doMove(move);

    REQUIRE(board.BLACK_PAWNS == 0ull);
    REQUIRE(board.BLACK_KNIGHTS == (ONE << c1));
    REQUIRE(board.WHITE_QUEENS == 0ull);
  }
}
