#include "zkey.h"
#include "defs.h"
#include "board.h"
#include "catch.hpp"

TEST_CASE("Zobrist hashing works properly") {
  ZKey::init();

  SECTION("Zorbrist hashing works after ZKey::init() is called") {
    Board board;

    SECTION("Zorbrist hash generates a nonzero hash for the starting position") {
      board.setToStartPos();

      ZKey key(board);

      REQUIRE(key.getValue() != 0);
    }

    SECTION("Same board positions generate the same key") {
      board.setToFen("rnbqkbnr/pppp1ppp/8/4p3/4P3/2N5/PPPP1PPP/R1BQKBNR b KQkq -");
      ZKey key1(board);
      ZKey key2(board);

      REQUIRE(key1.getValue() == key2.getValue());
    }

    SECTION("Keys constructed with different boards have different values") {
      board.setToFen("rn1qkbnr/pp2pppp/2p5/5b2/3PN3/8/PPP2PPP/R1BQKBNR w KQkq -");
      ZKey key1(board);

      board.setToFen("r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R w KQkq -");
      ZKey key2(board);

      REQUIRE(key1.getValue() != key2.getValue());
    }

    SECTION("Keys change when pawns are moved") {
      board.setToStartPos();
      ZKey key(board);

      U64 initValue = key.getValue();

      key.movePiece(WHITE, PAWN, a2, a4);

      REQUIRE(initValue != key.getValue());
    }

    SECTION("Keys change when the active player changes") {
      board.setToFen("rnbqkbnr/pppp1ppp/8/4p3/2B1P3/8/PPPP1PPP/RNBQK1NR b KQkq -");
      ZKey key1(board);

      board.setToFen("rnbqkbnr/pppp1ppp/8/4p3/2B1P3/8/PPPP1PPP/RNBQK1NR w KQkq -");
      ZKey key2(board);

      REQUIRE(key1.getValue() != key2.getValue());
    }

    SECTION("Keys change with different castling rights") {
      board.setToFen("rnbqkbnr/pppp1ppp/8/4p3/2B1P3/8/PPPP1PPP/RNBQK1NR b KQkq -");
      ZKey key(board);

      U64 initValue = key.getValue();

      key.updateCastlingRights(false, false, false, false);

      REQUIRE(key.getValue() != initValue);
    }

    SECTION("Keys change with different en passant files") {
      board.setToFen("rnbqkbnr/pppp2pp/5p2/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6");
      ZKey key(board);
      U64 initValue = key.getValue();

      key.clearEnPassant();
      key.setEnPassantFile(0);
      REQUIRE(initValue != key.getValue());
    }

    SECTION("Keys change when en passants are cleared") {
      board.setToFen("rnbqkbnr/pppp2pp/5p2/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6");
      ZKey key(board);

      U64 initValue = key.getValue();

      key.clearEnPassant();

      REQUIRE(initValue != key.getValue());
    }
  }
}
