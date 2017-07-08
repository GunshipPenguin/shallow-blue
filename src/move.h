#ifndef CMOVE_H
#define CMOVE_H

#include "defs.h"
#include "board.h"
#include <string>
#include <vector>

class Move {
public:
  Move(); // Null Move
  Move(unsigned int, unsigned int, PieceType, unsigned int=0); // Non Null Move

  enum Flag {
    NULL_MOVE = 1<<0,
    CAPTURE = 1<<1,
    DOUBLE_PAWN_PUSH = 1<<2,
    KSIDE_CASTLE = 1<<3,
    QSIDE_CASTLE = 1<<4,
    EN_PASSANT = 1<<5,
    PROMOTION = 1<<6
  };

  unsigned int getFlags();
  void setFlag(Flag);

  PieceType getPieceType();

  PieceType getCapturedPieceType();
  void setCapturedPieceType(PieceType);

  PieceType getPromotionPieceType();
  void setPromotionPieceType(PieceType);

  unsigned int getFrom();
  unsigned int getTo();

  std::string getNotation();

  static unsigned int notationToIndex(std::string);
  static std::string indexToNotation(int);

  bool operator==(Move) const;
private:
  // Format (28 bits total):
  // MSB  |----6----|---5---|---4---|-3--|-2--|-1--|  LSB
  //      28       21      15      9    6    3
  // 6 - Flags (7 bits)
  // 5 - To square (6 bits)
  // 4 - From square (6 bits)
  // 3 - Capture piece type (if applicable) (3 bits)
  // 2 - Promotion piece type (if applicable) (3 bits)
  // 1 - Piece type (3 bits)
  unsigned int _move;

  const static char RANKS[];
  const static char FILES[];
};

#endif
