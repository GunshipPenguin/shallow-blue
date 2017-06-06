#include "catch.hpp"
#include "cmove.h"

TEST_CASE("Move representation is correct") {
  SECTION("Getters in CMove work as expected") {
    CMove move(1, 63, CMove::CAPTURE);

    REQUIRE(move.getFrom() == 1);
    REQUIRE(move.getTo() == 63);
    REQUIRE(move.getFlags() == CMove::CAPTURE);
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
    CMove move(8, 16);

    REQUIRE(move.getNotation() == "a2a3");
  }

  SECTION("getNotation works for a capture move") {
    CMove move(8, 16, CMove::CAPTURE);

    REQUIRE(move.getNotation() == "a2xa3");
  }

  SECTION("getNotation works for all pawn promotion types") {
    CMove queenPromotion(55, 63, CMove::QUEEN_PROMOTION);
    CMove knightPromotion(55, 63, CMove::KNIGHT_PROMOTION);
    CMove rookPromotion(55, 63, CMove::ROOK_PROMOTION);
    CMove bishopPromotion(55, 63, CMove::BISHOP_PROMOTION);

    REQUIRE(queenPromotion.getNotation() == "h7h8Q");
    REQUIRE(knightPromotion.getNotation() == "h7h8N");
    REQUIRE(rookPromotion.getNotation() == "h7h8R");
    REQUIRE(bishopPromotion.getNotation() == "h7h8B");
  }

  SECTION("getNotation works for all pawn promotion types when making a capture") {
    CMove queenPromotion(54, 63, CMove::QUEEN_PROMOTION);
    CMove knightPromotion(54, 63, CMove::KNIGHT_PROMOTION);
    CMove rookPromotion(54, 63, CMove::ROOK_PROMOTION);
    CMove bishopPromotion(54, 63, CMove::BISHOP_PROMOTION);

    REQUIRE(queenPromotion.getNotation() == "g7h8Q");
    REQUIRE(knightPromotion.getNotation() == "g7h8N");
    REQUIRE(rookPromotion.getNotation() == "g7h8R");
    REQUIRE(bishopPromotion.getNotation() == "g7h8B");
  }

  SECTION("getNotation works for kingside castles") {
    CMove move(5, 7, CMove::KSIDE_CASTLE);

    REQUIRE(move.getNotation() == "O-O");
  }

  SECTION("getNotation works for queenside castles") {
    CMove move(5, 7, CMove::QSIDE_CASTLE);

    REQUIRE(move.getNotation() == "O-O-O");
  }
}
