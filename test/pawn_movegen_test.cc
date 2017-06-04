#include "catch.hpp"
#include "board.h"

TEST_CASE("Pawn move generation is correct") {
    Board board;

    SECTION("White pawn moves are generated correctly from start position") {
      board.setToStartPos();

      REQUIRE(board.getWhitePawnMoves().size() == 16);
    }

    SECTION("Black pawn moves are generated correctly from start position") {
      board.setToStartPos();

      REQUIRE(board.getBlackPawnMoves().size() == 16);
    }

    SECTION("White pawn moves are generated correctly after a1a4") {
      board.setToFen("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR w KQkq -");

      REQUIRE(board.getWhitePawnMoves().size() == 15);
    }

    SECTION("White pawn moves are generated correctly after h1h4") {
      board.setToFen("rnbqkbnr/pppppppp/8/8/7P/8/PPPPPPP1/RNBQKBNR w KQkq -");

      REQUIRE(board.getWhitePawnMoves().size() == 15);
    }

    SECTION("Black pawn moves are generated correctly after a7a5") {
      board.setToFen("rnbqkbnr/1ppppppp/8/p7/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

      REQUIRE(board.getBlackPawnMoves().size() == 15);
    }

    SECTION("Black pawn moves are generated correctly after h7h5") {
      board.setToFen("rnbqkbnr/ppppppp1/8/7p/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

      REQUIRE(board.getBlackPawnMoves().size() == 15);
    }

    SECTION("White pawn moves are not generated if piece is blocked") {
      board.setToFen("8/8/8/3p4/3P4/8/8/8 w - -");

      REQUIRE(board.getWhitePawnMoves().size() == 0);
    }

    SECTION("Black pawn moves are not generated if piece is blocked") {
      board.setToFen("8/8/8/3p4/3P4/8/8/8 w - -");

      REQUIRE(board.getBlackPawnMoves().size() == 0);
    }

    SECTION("White pawn promotion moves are generated upon reaching rank 8") {
      board.setToFen("8/4P3/8/8/8/8/8/8 w - -");

      REQUIRE(board.getWhitePawnMoves().size() == 4);
    }

    SECTION("Black pawn promotion moves are generated upon reaching rank 8") {
      board.setToFen("8/8/8/8/8/8/4p3/8 w - -");

      REQUIRE(board.getBlackPawnMoves().size() == 4);
    }
}

TEST_CASE("Pawn attack generation is correct") {
  Board board;

  SECTION("White pawn attacks are generated correctly from start position") {
    board.setToStartPos();

    REQUIRE(board.getWhitePawnAttacks().size() == 0);
  }

  SECTION("Black pawn attacks are generated correctly from start position") {
    board.setToStartPos();

    REQUIRE(board.getBlackPawnAttacks().size() == 0);
  }

  SECTION("White pawn attacks are generated correctly for 2 possible attacks") {
    board.setToFen("8/8/8/3r1r2/4P3/8/8/8 w KQkq -");

    REQUIRE(board.getWhitePawnAttacks().size() == 2);
  }

  SECTION("White pawn attacks are generated correctly for 1 possible attack") {
    board.setToFen("8/8/8/5r2/4P3/8/8/8 w - -");

    REQUIRE(board.getWhitePawnAttacks().size() == 1);
  }

  SECTION("White pawn attacks are generated correctly for 1 possible attack on the H file") {
    board.setToFen("8/8/8/6r1/7P/8/8/8 w - -");

    REQUIRE(board.getWhitePawnAttacks().size() == 1);
  }

  SECTION("White pawn attacks are generated correctly for 1 possible attack on the A file") {
    board.setToFen("8/8/8/1r6/P7/8/8/8 w - -");

    REQUIRE(board.getWhitePawnAttacks().size() == 1);
  }

  SECTION("Black pawn attacks are generated correctly for 2 possible attacks") {
    board.setToFen("8/8/3p4/2R1R3/8/8/8/8 w - -");

    REQUIRE(board.getBlackPawnAttacks().size() == 2);
  }

  SECTION("Black pawn attacks are generated correctly for 1 possible attack") {
    board.setToFen("8/8/3p4/2R5/8/8/8/8 w - -");

    REQUIRE(board.getBlackPawnAttacks().size() == 1);
  }

  SECTION("Black pawn attacks are generated correctly for 1 possible attack on the H file") {
    board.setToFen("8/8/7p/6R1/8/8/8/8 w - -");

    REQUIRE(board.getBlackPawnAttacks().size() == 1);
  }

  SECTION("Black pawn attacks are generated correctly for 1 possible attack on the A file") {
    board.setToFen("8/8/p7/1R6/8/8/8/8 w - -");

    REQUIRE(board.getBlackPawnAttacks().size() == 1);
  }
}
