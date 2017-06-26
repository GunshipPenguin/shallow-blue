#include "catch.hpp"
#include "cmove.h"
#include "defs.h"

TEST_CASE("Move representation is correct") {
  SECTION("Getters in CMove work as expected") {
    CMove move(g7, h8, PAWN, CMove::CAPTURE | CMove::PROMOTION);
    move.setCapturedPieceType(KNIGHT);
    move.setPromotionPieceType(QUEEN);

    REQUIRE(move.getFrom() == g7);
    REQUIRE(move.getTo() == h8);

    REQUIRE(move.getPieceType() == PAWN);

    REQUIRE(move.getFlags() == (CMove::CAPTURE | CMove::PROMOTION));

    REQUIRE(move.getCapturedPieceType() == KNIGHT);
    REQUIRE(move.getPromotionPieceType() == QUEEN);
  }

  SECTION("indexToNotation works correctly") {
    // Corners
    REQUIRE(CMove::indexToNotation(0) == "a1");
    REQUIRE(CMove::indexToNotation(7) == "h1");
    REQUIRE(CMove::indexToNotation(63) == "h8");
    REQUIRE(CMove::indexToNotation(56) == "a8");

    // Center Squares
    REQUIRE(CMove::indexToNotation(27) == "d4");
    REQUIRE(CMove::indexToNotation(28) == "e4");
    REQUIRE(CMove::indexToNotation(35) == "d5");
    REQUIRE(CMove::indexToNotation(36) == "e5");
  }

  SECTION("notationToIndex works correctly") {
    // Corners
    REQUIRE(CMove::notationToIndex("a1") == 0);
    REQUIRE(CMove::notationToIndex("h1") == 7);
    REQUIRE(CMove::notationToIndex("h8") == 63);
    REQUIRE(CMove::notationToIndex("a8") == 56);

    // Center Squares
    REQUIRE(CMove::notationToIndex("d4") == 27);
    REQUIRE(CMove::notationToIndex("e4") == 28);
    REQUIRE(CMove::notationToIndex("d5") == 35);
    REQUIRE(CMove::notationToIndex("e5") == 36);
  }
}

TEST_CASE("CMove.getNotation works properly") {
  SECTION("getNotation works for a non capture, non promotion move") {
    CMove move(8, 16, PAWN);

    REQUIRE(move.getNotation() == "a2a3");
  }

  SECTION("getNotation works for a capture move") {
    CMove move(8, 16, PAWN, CMove::CAPTURE);

    REQUIRE(move.getNotation() == "a2a3");
  }

  SECTION("getNotation works for all pawn promotion types") {
    CMove queenPromotion(55, 63, PAWN, CMove::PROMOTION);
    queenPromotion.setPromotionPieceType(QUEEN);

    CMove knightPromotion(55, 63, PAWN, CMove::PROMOTION);
    knightPromotion.setPromotionPieceType(KNIGHT);

    CMove rookPromotion(55, 63, PAWN, CMove::PROMOTION);
    rookPromotion.setPromotionPieceType(ROOK);

    CMove bishopPromotion(55, 63, PAWN, CMove::PROMOTION);
    bishopPromotion.setPromotionPieceType(BISHOP);


    REQUIRE(queenPromotion.getNotation() == "h7h8q");
    REQUIRE(knightPromotion.getNotation() == "h7h8n");
    REQUIRE(rookPromotion.getNotation() == "h7h8r");
    REQUIRE(bishopPromotion.getNotation() == "h7h8b");
  }

  SECTION("getNotation works for all pawn promotion types when making a capture") {
    CMove queenPromotion(54, 63, PAWN, CMove::PROMOTION | CMove::CAPTURE);
    queenPromotion.setPromotionPieceType(QUEEN);
    queenPromotion.setCapturedPieceType(ROOK);

    CMove knightPromotion(54, 63, PAWN, CMove::PROMOTION | CMove::CAPTURE);
    knightPromotion.setPromotionPieceType(KNIGHT);
    queenPromotion.setCapturedPieceType(ROOK);

    CMove rookPromotion(54, 63, PAWN, CMove::PROMOTION | CMove::CAPTURE);
    rookPromotion.setPromotionPieceType(ROOK);
    queenPromotion.setCapturedPieceType(ROOK);

    CMove bishopPromotion(54, 63, PAWN, CMove::PROMOTION | CMove::CAPTURE);
    bishopPromotion.setPromotionPieceType(BISHOP);
    queenPromotion.setCapturedPieceType(ROOK);

    REQUIRE(queenPromotion.getNotation() == "g7h8q");
    REQUIRE(knightPromotion.getNotation() == "g7h8n");
    REQUIRE(rookPromotion.getNotation() == "g7h8r");
    REQUIRE(bishopPromotion.getNotation() == "g7h8b");
  }

  SECTION("getNotation works for kingside castles") {
    CMove move(4, 6, KING, CMove::KSIDE_CASTLE);

    REQUIRE(move.getNotation() == "e1g1");
  }

  SECTION("getNotation works for queenside castles") {
    CMove move(4, 2, KING, CMove::QSIDE_CASTLE);

    REQUIRE(move.getNotation() == "e1c1");
  }
}
