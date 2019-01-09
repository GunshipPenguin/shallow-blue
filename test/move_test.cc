#include "catch.hpp"
#include "move.h"

TEST_CASE("Move representation is correct") {
  SECTION("Getters in Move work as expected") {
    Move move(g7, h8, PAWN, Move::CAPTURE | Move::PROMOTION);
    move.setCapturedPieceType(KNIGHT);
    move.setPromotionPieceType(QUEEN);

    REQUIRE(move.getFrom() == g7);
    REQUIRE(move.getTo() == h8);

    REQUIRE(move.getPieceType() == PAWN);

    REQUIRE(move.getFlags() == (Move::CAPTURE | Move::PROMOTION));

    REQUIRE(move.getCapturedPieceType() == KNIGHT);
    REQUIRE(move.getPromotionPieceType() == QUEEN);
  }

  SECTION("indexToNotation works correctly") {
    // Corners
    REQUIRE(Move::indexToNotation(0) == "a1");
    REQUIRE(Move::indexToNotation(7) == "h1");
    REQUIRE(Move::indexToNotation(63) == "h8");
    REQUIRE(Move::indexToNotation(56) == "a8");

    // Center Squares
    REQUIRE(Move::indexToNotation(27) == "d4");
    REQUIRE(Move::indexToNotation(28) == "e4");
    REQUIRE(Move::indexToNotation(35) == "d5");
    REQUIRE(Move::indexToNotation(36) == "e5");
  }

  SECTION("notationToIndex works correctly") {
    // Corners
    REQUIRE(Move::notationToIndex("a1") == 0);
    REQUIRE(Move::notationToIndex("h1") == 7);
    REQUIRE(Move::notationToIndex("h8") == 63);
    REQUIRE(Move::notationToIndex("a8") == 56);

    // Center Squares
    REQUIRE(Move::notationToIndex("d4") == 27);
    REQUIRE(Move::notationToIndex("e4") == 28);
    REQUIRE(Move::notationToIndex("d5") == 35);
    REQUIRE(Move::notationToIndex("e5") == 36);
  }
}

TEST_CASE("Move.getNotation works properly") {
  SECTION("getNotation works for a non capture, non promotion move") {
    Move move(8, 16, PAWN);

    REQUIRE(move.getNotation() == "a2a3");
  }

  SECTION("getNotation works for a capture move") {
    Move move(8, 16, PAWN, Move::CAPTURE);

    REQUIRE(move.getNotation() == "a2a3");
  }

  SECTION("getNotation works for all pawn promotion types") {
    Move queenPromotion(55, 63, PAWN, Move::PROMOTION);
    queenPromotion.setPromotionPieceType(QUEEN);

    Move knightPromotion(55, 63, PAWN, Move::PROMOTION);
    knightPromotion.setPromotionPieceType(KNIGHT);

    Move rookPromotion(55, 63, PAWN, Move::PROMOTION);
    rookPromotion.setPromotionPieceType(ROOK);

    Move bishopPromotion(55, 63, PAWN, Move::PROMOTION);
    bishopPromotion.setPromotionPieceType(BISHOP);

    REQUIRE(queenPromotion.getNotation() == "h7h8q");
    REQUIRE(knightPromotion.getNotation() == "h7h8n");
    REQUIRE(rookPromotion.getNotation() == "h7h8r");
    REQUIRE(bishopPromotion.getNotation() == "h7h8b");
  }

  SECTION("getNotation works for all pawn promotion types when making a capture") {
    Move queenPromotion(54, 63, PAWN, Move::PROMOTION | Move::CAPTURE);
    queenPromotion.setPromotionPieceType(QUEEN);
    queenPromotion.setCapturedPieceType(ROOK);

    Move knightPromotion(54, 63, PAWN, Move::PROMOTION | Move::CAPTURE);
    knightPromotion.setPromotionPieceType(KNIGHT);
    queenPromotion.setCapturedPieceType(ROOK);

    Move rookPromotion(54, 63, PAWN, Move::PROMOTION | Move::CAPTURE);
    rookPromotion.setPromotionPieceType(ROOK);
    queenPromotion.setCapturedPieceType(ROOK);

    Move bishopPromotion(54, 63, PAWN, Move::PROMOTION | Move::CAPTURE);
    bishopPromotion.setPromotionPieceType(BISHOP);
    queenPromotion.setCapturedPieceType(ROOK);

    REQUIRE(queenPromotion.getNotation() == "g7h8q");
    REQUIRE(knightPromotion.getNotation() == "g7h8n");
    REQUIRE(rookPromotion.getNotation() == "g7h8r");
    REQUIRE(bishopPromotion.getNotation() == "g7h8b");
  }

  SECTION("getNotation works for kingside castles") {
    Move move(4, 6, KING, Move::KSIDE_CASTLE);

    REQUIRE(move.getNotation() == "e1g1");
  }

  SECTION("getNotation works for queenside castles") {
    Move move(4, 2, KING, Move::QSIDE_CASTLE);

    REQUIRE(move.getNotation() == "e1c1");
  }
}
