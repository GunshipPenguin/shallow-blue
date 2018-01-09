#include "catch.hpp"
#include "book.h"

TEST_CASE("Opening book works as expected") {
  Board board;

  SECTION("hashBoard calculates PolyGlot board hashes correctly") {
    board.setToStartPos();
    REQUIRE(Book::hashBoard(board) == 0x463b96181691fc9c);

    board.setToFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    REQUIRE(Book::hashBoard(board) == 0x823c9b50fd114196);

    board.setToFen("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2");
    REQUIRE(Book::hashBoard(board) == 0x0756b94461c50fb0);

    board.setToFen("rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2");
    REQUIRE(Book::hashBoard(board) == 0x662fafb965db29d4);

    board.setToFen("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
    REQUIRE(Book::hashBoard(board) == 0x22a48b5a8e47ff78);

    board.setToFen("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 3");
    REQUIRE(Book::hashBoard(board) == 0x652a607ca3f242c1);

    board.setToFen("rnbq1bnr/ppp1pkpp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR w - - 0 4");
    REQUIRE(Book::hashBoard(board) == 0x00fdd303c946bdd9);

    board.setToFen("rnbqkbnr/p1pppppp/8/8/PpP4P/8/1P1PPPP1/RNBQKBNR b KQkq c3 0 3");
    REQUIRE(Book::hashBoard(board) == 0x3c8123ea7b067637);

    board.setToFen("rnbqkbnr/p1pppppp/8/8/P6P/R1p5/1P1PPPP1/1NBQKBNR b Kkq - 0 4");
    REQUIRE(Book::hashBoard(board) == 0x5c3f9b829b279560);
  }

  SECTION("decodeMove decodes PolyGlot moves to Shallow Blue moves correctly") {
    // e2e4
    board.setToStartPos();
    Move e2e4(e2, e4, PAWN, Move::DOUBLE_PAWN_PUSH);
    REQUIRE(Book::decodeMove(board, 0x31c) == e2e4);

    // e2e4 g8f6
    board.setToFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3");
    Move g8f6(g8, f6, KNIGHT);
    REQUIRE(Book::decodeMove(board, 0xfad) == g8f6);

    // e2e4 g8f6 f1b5
    board.setToFen("rnbqkb1r/pppppppp/5n2/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq -");
    Move f1b5(f1, b5, BISHOP);
    REQUIRE(Book::decodeMove(board, 0x161) == f1b5);
  }

  SECTION("decodeMove decodes PolyGlot moves to Shallow Blue moves correctly (en passant)") {
    board.setToFen("rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6");
    Move e5d6(e5, d6, PAWN, Move::EN_PASSANT);
    REQUIRE(Book::decodeMove(board, 0x92b) == e5d6);
  }

  SECTION("decodeMove decodes PolyGlot moves to Shallow Blue moves correctly (promotions)") {
    board.setToFen("7k/3P4/8/8/8/8/8/K7 w - -");
    Move d7d8q(d7, d8, PAWN, Move::PROMOTION);
    Move d7d8r(d7, d8, PAWN, Move::PROMOTION);
    Move d7d8b(d7, d8, PAWN, Move::PROMOTION);
    Move d7d8n(d7, d8, PAWN, Move::PROMOTION);
    d7d8q.setPromotionPieceType(QUEEN);
    d7d8r.setPromotionPieceType(ROOK);
    d7d8b.setPromotionPieceType(BISHOP);
    d7d8n.setPromotionPieceType(KNIGHT);

    REQUIRE(Book::decodeMove(board, 0x4cfb) == d7d8q);
    REQUIRE(Book::decodeMove(board, 0x3cfb) == d7d8r);
    REQUIRE(Book::decodeMove(board, 0x2cfb) == d7d8b);
    REQUIRE(Book::decodeMove(board, 0x1cfb) == d7d8n);
  }

  SECTION("decodeMove decodes PolyGlot moves to Shallow Blue moves correctly (castling)") {
    Move whiteKs = Move(e1, g1, KING, Move::KSIDE_CASTLE);
    Move whiteQs = Move(e1, c1, KING, Move::QSIDE_CASTLE);
    Move blackKs = Move(e8, g8, KING, Move::KSIDE_CASTLE);
    Move blackQs = Move(e8, c8, KING, Move::QSIDE_CASTLE);

    board.setToFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq -");
    REQUIRE(Book::decodeMove(board, 0x107) == whiteKs);
    REQUIRE(Book::decodeMove(board, 0x100) == whiteQs);
    REQUIRE(Book::decodeMove(board, 0xf3f) == blackKs);
    REQUIRE(Book::decodeMove(board, 0xf38) == blackQs);
  }
}