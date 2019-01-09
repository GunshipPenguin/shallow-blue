#include "board.h"
#include "catch.hpp"
#include <iostream>

TEST_CASE("Board::doMove works properly") {
  Board board;

  SECTION("doMove moves pawns up from the starting position for white") {
    board.setToStartPos();

    Move move(a2, a4, PAWN);

    board.doMove(move);

    REQUIRE((board.getPieces(WHITE, PAWN) & (ONE << a4)));
  }

  SECTION("doMove moves pawns up from the starting position for black") {
    board.setToFen("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq -");

    Move move(a7, a5, PAWN);

    board.doMove(move);

    REQUIRE((board.getPieces(BLACK, PAWN) & (ONE << a5)));
  }

  SECTION("doMove handles captures") {
    board.setToFen("8/8/8/4n3/8/2B5/8/8 w - -");

    Move move(c3, e5, BISHOP, Move::CAPTURE);
    move.setCapturedPieceType(KNIGHT);

    board.doMove(move);

    REQUIRE((board.getPieces(WHITE, BISHOP) & (ONE << e5)));
    REQUIRE(board.getPieces(BLACK, KNIGHT) == ZERO);
  }

  SECTION("doMove handles en passant for black") {
    board.setToFen("8/8/8/8/2Pp4/8/8/8 b kq c3");

    Move move(d4, c3, PAWN, Move::EN_PASSANT);

    board.doMove(move);

    REQUIRE((board.getPieces(BLACK, PAWN) & (ONE << c3)));
    REQUIRE(board.getPieces(WHITE, PAWN) == ZERO);
    REQUIRE(board.getEnPassant() == ZERO);
  }

  SECTION("doMove handles en passant for white") {
    board.setToFen("8/8/8/2pP4/8/8/8/8 w kq c6");

    Move move(d5, c6, PAWN, Move::EN_PASSANT);

    board.doMove(move);

    REQUIRE((board.getPieces(WHITE, PAWN) & (ONE << c6)));
    REQUIRE(board.getPieces(BLACK, PAWN) == ZERO);
    REQUIRE(board.getEnPassant() == ZERO);
  }

  SECTION("doMove handles white kingside castles") {
    board.setToFen("8/8/8/8/8/8/8/4K2R w - -");
    Move move(e1, g1, KING, Move::KSIDE_CASTLE);

    board.doMove(move);
    REQUIRE((board.getPieces(WHITE, KING) & (ONE << g1)));
    REQUIRE((board.getPieces(WHITE, ROOK) & (ONE << f1)));
  }

  SECTION("doMove handles white queenside castles") {
    board.setToFen("8/8/8/8/8/8/8/4K2R w - -");
    Move move(e1, c1, KING, Move::QSIDE_CASTLE);

    board.doMove(move);

    REQUIRE((board.getPieces(WHITE, KING) & (ONE << c1)));
    REQUIRE((board.getPieces(WHITE, ROOK) & (ONE << d1)));
  }

  SECTION("doMove handles black kingside castles") {
    board.setToFen("4k2r/8/8/8/8/8/8/8 b - -");
    Move move(e8, g8, KING, Move::KSIDE_CASTLE);

    board.doMove(move);

    REQUIRE((board.getPieces(BLACK, KING) & (ONE << g8)));
    REQUIRE((board.getPieces(BLACK, ROOK) & (ONE << f8)));
  }

  SECTION("doMove handles black queenside castles") {
    board.setToFen("r3k3/8/8/8/8/8/8/8 b - -");
    Move move(e8, c8, KING, Move::QSIDE_CASTLE);

    board.doMove(move);

    REQUIRE((board.getPieces(BLACK, KING) & (ONE << c8)));
    REQUIRE((board.getPieces(BLACK, ROOK) & (ONE << d8)));
  }

  SECTION("White cannot castle after moving its king") {
    board.setToFen("8/8/8/8/8/8/8/4K2R w KQ -");

    REQUIRE(board.whiteCanCastleKs() == true);
    REQUIRE(board.whiteCanCastleQs() == true);

    Move move(e1, e2, KING);
    board.doMove(move);

    REQUIRE(board.whiteCanCastleKs() == false);
    REQUIRE(board.whiteCanCastleQs() == false);
  }

  SECTION("Black cannot castle after moving its king") {
    board.setToFen("4k2r/8/8/8/8/8/8/8 w kq -");

    REQUIRE(board.blackCanCastleKs() == true);
    REQUIRE(board.blackCanCastleQs() == true);

    Move move(e8, e7, KING);
    board.doMove(move);

    REQUIRE(board.blackCanCastleKs() == false);
    REQUIRE(board.blackCanCastleQs() == false);
  }

  SECTION("doMove works with white promotions") {
    board.setToFen("8/3P4/8/8/8/8/8/8 w - -");

    Move move(d7, d8, PAWN, Move::PROMOTION);
    move.setPromotionPieceType(QUEEN);

    board.doMove(move);

    REQUIRE(board.getPieces(WHITE, PAWN) == ZERO);
    REQUIRE(board.getPieces(WHITE, QUEEN) == (ONE << d8));
  }

  SECTION("doMove works with black promotions") {
    board.setToFen("8/8/8/8/8/8/3p4/8 b - -");

    Move move(d2, d1, PAWN, Move::PROMOTION);
    move.setPromotionPieceType(QUEEN);

    board.doMove(move);

    REQUIRE(board.getPieces(BLACK, PAWN) == ZERO);
    REQUIRE(board.getPieces(BLACK, QUEEN) == (ONE << d1));
  }

  SECTION("doMove works with white capture promotions") {
    board.setToFen("2q5/3P4/8/8/8/8/8/8 w - -");

    Move move(d7, c8, PAWN, Move::CAPTURE | Move::PROMOTION);
    move.setCapturedPieceType(QUEEN);
    move.setPromotionPieceType(KNIGHT);

    board.doMove(move);

    REQUIRE(board.getPieces(WHITE, PAWN) == ZERO);
    REQUIRE(board.getPieces(BLACK, QUEEN) == ZERO);
    REQUIRE(board.getPieces(WHITE, KNIGHT) == (ONE << c8));
  }

  SECTION("doMove works with black capture promotions") {
    board.setToFen("8/8/8/8/8/8/3p4/2Q5 b - -");

    Move move(d2, c1, PAWN, Move::CAPTURE | Move::PROMOTION);
    move.setCapturedPieceType(QUEEN);
    move.setPromotionPieceType(KNIGHT);

    board.doMove(move);

    REQUIRE(board.getPieces(BLACK, PAWN) == ZERO);
    REQUIRE(board.getPieces(BLACK, KNIGHT) == (ONE << c1));
    REQUIRE(board.getPieces(WHITE, QUEEN) == ZERO);
  }

  SECTION("doMove should update the en passant square after a double pawn push for white") {
    board.setToStartPos();

    Move move(a2, a4, PAWN, Move::DOUBLE_PAWN_PUSH);
    board.doMove(move);

    REQUIRE(board.getEnPassant() == (ONE << a3));
  }

  SECTION("doMove should update the en passant square after a double pawn push for black") {
    board.setToFen("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq -");

    Move move(a7, a5, PAWN, Move::DOUBLE_PAWN_PUSH);
    board.doMove(move);

    REQUIRE(board.getEnPassant() == (ONE << a6));
  }
}
