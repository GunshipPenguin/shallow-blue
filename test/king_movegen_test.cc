#include "board.h"
#include "movegen.h"
#include "catch.hpp"

TEST_CASE("King move generation is correct") {
  Board board;
  MoveGen movegen;

  SECTION("Kings can move to all possible squares on an empty board") {
    board.setToFen("8/8/8/8/3K4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);

    board.setToFen("8/8/8/8/3k4/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Kings have the correct number of moves on a1") {
    board.setToFen("8/8/8/8/8/8/8/K7 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 3);

    board.setToFen("8/8/8/8/8/8/8/k7 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 3);
  }

  SECTION("Kings have the correct number of moves on h8") {
    board.setToFen("7K/8/8/8/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 3);

    board.setToFen("7k/8/8/8/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 3);
  }

  SECTION("Kings do not move to squares occupied by their own pieces") {
    board.setToFen("8/8/8/3K4/3P4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);

    board.setToFen("8/8/3p4/3k4/8/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);
  }

  SECTION("Kings attack the other player's pieces") {
    board.setToFen("8/8/8/4r3/3K4/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);

    board.setToFen("8/8/8/4R3/3k4/8/8/8 b - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 8);
  }

  SECTION("Kings do not capture the other king") {
    board.setToFen("8/8/8/3Kk3/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);

    board.setToFen("8/8/8/3kK3/8/8/8/8 w - -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 7);
  }

  SECTION("Castles are generated when castling is possible as specified in the fen string") {
    // White kingside only
    board.setToFen("8/8/8/8/8/8/8/4K2R w K -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 15);

    // White queenside only
    board.setToFen("8/8/8/8/8/8/8/R3K3 w Q -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 16);

    // White king and queenside
    board.setToFen("8/8/8/8/8/8/8/R3K2R w KQ -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 26);

    // Black kingside only
    board.setToFen("4k2r/8/8/8/8/8/8/8 b k -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 15);

    // Black queenside only
    board.setToFen("r3k3/8/8/8/8/8/8/8 b q -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 16);

    // Black king and queenside
    board.setToFen("r3k2r/8/8/8/8/8/8/8 b kq -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 26);
  }

  SECTION("Castles are not generated when the king is in check") {
    board.setToFen("8/8/8/8/8/3n4/8/R3K2R w KQ -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 24);

    board.setToFen("r3k2r/8/3N4/8/8/8/8/8 b kq -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 24);
  }

  SECTION("White ks castles are not generated when squares that the king passes through are attacked") {
    board.setToFen("8/8/8/8/5r2/8/8/R3K2R w KQ -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 25);

    board.setToFen("8/8/8/2b5/8/8/8/R3K2R w KQ -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 25);
  }

  SECTION("White qs castles are not generated when squares that the king passes through are attacked") {
    board.setToFen("8/8/8/8/2r5/8/8/R3K2R w KQ -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 25);

    board.setToFen("8/8/8/8/3r4/8/8/R3K2R w KQ -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 25);
  }

  SECTION("Black ks castles are not generated when squares that the king passes through are attacked") {
    board.setToFen("r3k2r/8/8/5R2/8/8/8/8 b kq -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 25);

    board.setToFen("r3k2r/8/8/3B4/8/8/8/8 b kq -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 25);
  }

  SECTION("Black qs castles are not generated when squares that the king passes through are attacked") {
    board.setToFen("r3k2r/8/8/3R4/8/8/8/8 b kq -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 25);

    board.setToFen("r3k2r/8/8/5B2/8/8/8/8 b kq -");
    movegen.setBoard(board);
    REQUIRE(movegen.getMoves().size() == 25);
  }

  SECTION("White ks castles are not generated after the ks rook has been captured") {
    board.setToFen("8/8/8/3b4/8/8/8/4K2R b K -");

    REQUIRE(board.whiteCanCastleKs() == true);
    Move move(d5, h1, BISHOP, Move::CAPTURE);

    board.doMove(move);

    REQUIRE(board.whiteCanCastleKs() == false);
  }

  SECTION("White qs castles are not generated after the qs rook has been captured") {
    board.setToFen("8/8/8/8/4b3/8/8/R3K3 b Q -");

    REQUIRE(board.whiteCanCastleQs() == true);
    Move move(e4, a1, BISHOP, Move::CAPTURE);

    board.doMove(move);

    REQUIRE(board.whiteCanCastleQs() == false);
  }

  SECTION("Black ks castles are not generated after the ks rook has been captured") {
    board.setToFen("4k2r/8/8/4B3/8/8/8/8 w k -");

    REQUIRE(board.blackCanCastleKs() == true);
    Move move(e5, h8, BISHOP, Move::CAPTURE);

    board.doMove(move);

    REQUIRE(board.blackCanCastleKs() == false);
  }

  SECTION("Black qs castles are not generated after the qs rook has been captured") {
    board.setToFen("r3k3/8/8/8/4B3/8/8/8 w q -");

    REQUIRE(board.blackCanCastleQs() == true);
    Move move(e4, a8, BISHOP, Move::CAPTURE);

    board.doMove(move);

    REQUIRE(board.blackCanCastleQs() == false);
  }

  SECTION("White ks castles are not generated after the white ks rook has moved") {
    board.setToFen("8/8/8/8/8/8/8/4K2R w K -");

    REQUIRE(board.whiteCanCastleKs() == true);
    Move move(h1, g1, ROOK);

    board.doMove(move);

    REQUIRE(board.whiteCanCastleKs() == false);
  }

  SECTION("White qs castles are not generated after the white qs rook has moved") {
    board.setToFen("8/8/8/8/8/8/8/R3K3 w Q -");

    REQUIRE(board.whiteCanCastleQs() == true);
    Move move(a1, b1, ROOK);

    board.doMove(move);

    REQUIRE(board.whiteCanCastleQs() == false);
  }

  SECTION("Black ks castles are not generated after the black ks rook has moved") {
    board.setToFen("4k2r/8/8/8/8/8/8/8 b k -");

    REQUIRE(board.blackCanCastleKs() == true);
    Move move(h8, g8, ROOK);

    board.doMove(move);

    REQUIRE(board.blackCanCastleKs() == false);
  }

  SECTION("Black qs castles are not generated after the black qs rook has moved") {
    board.setToFen("r3k3/8/8/8/8/8/8/8 b q -");

    REQUIRE(board.blackCanCastleQs() == true);
    Move move(a8, b8, ROOK);

    board.doMove(move);

    REQUIRE(board.blackCanCastleQs() == false);
  }

  SECTION("White castles are not generated after the white king has moved") {
    board.setToFen("8/8/8/8/8/8/8/R3K2R w KQ -");

    REQUIRE(board.whiteCanCastleKs() == true);
    REQUIRE(board.whiteCanCastleQs() == true);

    Move move(e1, e2, KING);
    board.doMove(move);

    REQUIRE(board.whiteCanCastleKs() == false);
    REQUIRE(board.whiteCanCastleQs() == false);
  }

  SECTION("Black castles are not generated after the black king has moved") {
    board.setToFen("r3k2r/8/8/8/8/8/8/8 b kq -");

    REQUIRE(board.blackCanCastleKs() == true);
    REQUIRE(board.blackCanCastleQs() == true);

    Move move(e8, e7, KING);
    board.doMove(move);

    REQUIRE(board.blackCanCastleKs() == false);
    REQUIRE(board.blackCanCastleQs() == false);
  }

  SECTION("White qs castles are not generated when there are pieces in between the king and rook") {
    board.setToFen("8/8/8/8/8/8/8/RB2K3 w KQ -");
    REQUIRE(board.whiteCanCastleQs() == false);

    board.setToFen("8/8/8/8/8/8/8/R1B1K3 w KQ -");
    REQUIRE(board.whiteCanCastleQs() == false);

    board.setToFen("8/8/8/8/8/8/8/R2BK3 w KQ -");
    REQUIRE(board.whiteCanCastleQs() == false);
  }

  SECTION("White ks castles are not generated when there are pieces in between the king and rook") {
    board.setToFen("8/8/8/8/8/8/8/4K1BR w KQ -");
    REQUIRE(board.whiteCanCastleKs() == false);

    board.setToFen("8/8/8/8/8/8/8/4KB1R w - -");
    REQUIRE(board.whiteCanCastleKs() == false);
  }

  SECTION("Black qs castles are not generated when there are pieces in between the king and rook") {
    board.setToFen("rb2k3/8/8/8/8/8/8/8 b kq -");
    REQUIRE(board.blackCanCastleQs() == false);

    board.setToFen("r2bk3/8/8/8/8/8/8/8 b kq -");
    REQUIRE(board.blackCanCastleQs() == false);

    board.setToFen("r2bk3/8/8/8/8/8/8/8 b kq -");
    REQUIRE(board.blackCanCastleQs() == false);
  }

  SECTION("Black ks castles are not generated when there are pieces in between the king and rook") {
    board.setToFen("4k1br/8/8/8/8/8/8/8 w kq -");
    REQUIRE(board.blackCanCastleKs() == false);

    board.setToFen("4kb1r/8/8/8/8/8/8/8 w kq -");
    REQUIRE(board.blackCanCastleKs() == false);
  }
}
