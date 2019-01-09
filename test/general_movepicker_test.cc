#include "catch.hpp"
#include "generalmovepicker.h"

TEST_CASE("GeneralMovePicker works as expected") {
  Board board;
  TranspTable tt;
  OrderingInfo orderingInfo(const_cast<TranspTable *>(&tt));

  SECTION("GeneralMovePicker returns the hash move first") {
    board.setToFen("7k/8/8/8/4p3/8/5N2/K7 w - -");

    MoveGen moveGen(board);
    MoveList moves = moveGen.getLegalMoves();

    // Set NxP as the has move
    Move hashMove(f2, e4, KNIGHT, Move::CAPTURE);
    TranspTableEntry ttEntry(20, 2, TranspTableEntry::EXACT, hashMove);
    tt.set(board.getZKey(), ttEntry);

    GeneralMovePicker movePicker(const_cast<OrderingInfo *>(&orderingInfo), const_cast<Board *>(&board), &moves);

    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == hashMove);
  }

  SECTION("GeneralMovePicker returns captures after the hash move in MVV/LVA order") {
    board.setToFen("7k/1R6/1p6/8/4r3/8/5N2/K2b4 w - -");

    MoveGen movegen(board);
    MoveList moves = movegen.getLegalMoves();
    GeneralMovePicker movePicker(const_cast<OrderingInfo *>(&orderingInfo), const_cast<Board *>(&board), &moves);

    // f2 x e4
    Move m1(f2, e4, KNIGHT, Move::CAPTURE);
    m1.setCapturedPieceType(ROOK);
    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == m1);

    // f2 x d1
    Move m2(f2, d1, KNIGHT, Move::CAPTURE);
    m2.setCapturedPieceType(BISHOP);
    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == m2);

    // b7 x b6
    Move m3(b7, b6, ROOK, Move::CAPTURE);
    m3.setCapturedPieceType(PAWN);
    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == m3);
  }

  SECTION("GeneralMovePicker returns promotions after captures sorted by promotion value") {
    board.setToFen("7k/2P5/8/8/8/8/8/K7 w - -");

    MoveGen movegen(board);
    MoveList moves = movegen.getLegalMoves();
    GeneralMovePicker movePicker(const_cast<OrderingInfo *>(&orderingInfo), const_cast<Board *>(&board), &moves);

    // Queen promotion
    Move m1(c7, c8, PAWN, Move::PROMOTION);
    m1.setPromotionPieceType(QUEEN);
    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == m1);

    // Rook promotion
    Move m2(c7, c8, PAWN, Move::PROMOTION);
    m2.setPromotionPieceType(ROOK);
    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == m2);

    // Bishop promotion
    Move m3(c7, c8, PAWN, Move::PROMOTION);
    m3.setPromotionPieceType(BISHOP);
    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == m3);

    // Knight promotion
    Move m4(c7, c8, PAWN, Move::PROMOTION);
    m4.setPromotionPieceType(KNIGHT);
    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == m4);
  }

  SECTION("GeneralMovePicker returns killer 1 and 2 (in that order) after promotions") {
    board.setToStartPos();

    MoveGen movegen(board);
    MoveList moves = movegen.getLegalMoves();

    Move killer1(e2, e4, PAWN, Move::DOUBLE_PAWN_PUSH);
    Move killer2(f2, f4, PAWN, Move::DOUBLE_PAWN_PUSH);

    orderingInfo.updateKillers(0, killer2);
    orderingInfo.updateKillers(0, killer1);

    GeneralMovePicker movePicker(const_cast<OrderingInfo *>(&orderingInfo), const_cast<Board *>(&board), &moves);

    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == killer1);

    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == killer2);
  }

  SECTION("GeneralMovePicker returns quiets last sorted by history") {
    board.setToFen("7k/8/8/8/2P5/8/8/K6N w - -");

    MoveGen moveGen(board);
    MoveList moves = moveGen.getLegalMoves();

    // History values h1g3 < h1f2 < c4c5
    orderingInfo.incrementHistory(WHITE, c4, c5, 3);
    orderingInfo.incrementHistory(WHITE, h1, f2, 2);
    orderingInfo.incrementHistory(WHITE, h1, g3, 1);

    GeneralMovePicker movePicker(const_cast<OrderingInfo *>(&orderingInfo), const_cast<Board *>(&board), &moves);

    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == Move(c4, c5, PAWN));

    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == Move(h1, f2, KNIGHT));

    REQUIRE(movePicker.hasNext());
    REQUIRE(movePicker.getNext() == Move(h1, g3, KNIGHT));
  }
}