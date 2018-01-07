#ifndef BOOK_H
#define BOOK_H

#include "defs.h"
#include "move.h"
#include "board.h"
#include <vector>
#include <unordered_map>

class Book {
public:
  Book();
  Book(std::string);

  Move getMove(const Board&) const;
  bool inBook(const Board&) const;

  static U64 hashBoard(const Board&);
  static Move decodeMove(const Board&, unsigned short);

private:
  static const U64 RANDOM_PIECE[768];
  static const U64 RANDOM_CASTLE[4];
  static const U64 RANDOM_ENPASSANT[8];
  static const U64 RANDOM_TURN;

  std::unordered_map<U64, std::vector<std::pair<unsigned short, unsigned short>>> _content;

  void _initMoves(std::string);

};

#endif