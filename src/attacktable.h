#ifndef ATTACKTABLE_H
#define ATTACKTABLE_H

#include "defs.h"

class AttackTable {
public:    
    static U64 getAttacks(PieceType, int, Color=WHITE);

    static void init();
private:
    static void _zeroTables();

    static U64 ATTACKS[2][6][64];
    static void _calcPawn();
    static void _calcKnight();
    static void _calcBishop();    
    static void _calcRook();
    static void _calcQueen();
    static void _calcKing();
};

#endif