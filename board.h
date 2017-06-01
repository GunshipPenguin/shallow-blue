#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

typedef unsigned long long U64;

class Board {
  public:
    void setToStartPos();

    std::string getStringRep();


  private:
    bool _whiteToMove;
    U64 WHITE_PAWNS;
    U64 BLACK_PAWNS;

    U64 WHITE_ROOKS;
    U64 BLACK_ROOKS;

    U64 WHITE_KNIGHTS;
    U64 BLACK_KNIGHTS;

    U64 WHITE_BISHOPS;
    U64 BLACK_BISHOPS;

    U64 WHITE_QUEENS;
    U64 BLACK_QUEENS;

    U64 WHITE_KING;
    U64 BLACK_KING;

    U64 WHITE_PIECES;
    U64 BLACK_PIECES;

    U64 OCCUPIED;

    static const U64 RANK_1 = 255ull;
    static const U64 RANK_2 = 65280ull;
    static const U64 RANK_3 = 16711680ull;
    static const U64 RANK_4 = 4278190080ull;
    static const U64 RANK_5 = 1095216660480ull;
    static const U64 RANK_6 = 280375465082880ull;
    static const U64 RANK_7 = 71776119061217280ull;
    static const U64 RANK_8 = 18374686479671623680ull;

    static const U64 FILE_1 = 9259542123273814144ull;
    static const U64 FILE_2 = 4629771061636907072ull;
    static const U64 FILE_3 = 2314885530818453536ull;
    static const U64 FILE_4 = 1157442765409226768ull;
    static const U64 FILE_5 = 578721382704613384ull;
    static const U64 FILE_6 = 289360691352306692ull;
    static const U64 FILE_7 = 144680345676153346ull;
    static const U64 FILE_8 = 72340172838076673ull;


    U64 getOccupied();
    U64 getBlackPieces();
    U64 getWhitePieces();

    U64 getPawnMoves();
};

#endif
