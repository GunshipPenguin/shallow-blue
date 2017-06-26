#include "uci.h"
#include "board.h"
#include "defs.h"
#include "cmove.h"
#include "movegen.h"
#include "search.h"
#include <iostream>
#include <sstream>

void Uci::uciNewGame(){
  _board.setToStartPos();
}

void Uci::setPosition(std::istringstream& is) {
  std::string token;
  is >> token;

  if (token == "startpos") {
    _board.setToStartPos();
  } else {
    std::string fen;

    while (is >> token && token != "moves") {
      fen += token + " ";
    }

    _board.setToFen(fen);
  }

  while (is >> token) {
    if (token == "moves") {
      continue;
    }

    MoveGen movegen(_board);
    for (auto move : movegen.getMoves()) {
      if (move.getNotation() == token) {
        _board.doMove(move);
        break;
      }
    }
  }
}

CMove Uci::pickBestMove() {
  Search search(_board, DEFAULT_DEPTH, DEFAULT_MAX_TIME);
  return search.getBestMove();
}

void Uci::start() {
  _board.setToStartPos();

  std::string line;
  std::string token;

  while(std::getline(std::cin, line)) {
    std::istringstream is(line);
    is >> token;

    if (token == "uci") {
      std::cout << "id name chess" << std::endl;
      std::cout << "id author rhys" << std::endl << std::endl;
      std::cout << "uciok" << std::endl;
    } else if (token == "ucinewgame") {
      uciNewGame();
    } else if (token == "isready") {
      std::cout << "readyok" << std::endl;
    } else if (token == "go") {
      CMove bestMove = pickBestMove();
      std::cout << "bestmove " << bestMove.getNotation() << std::endl;
    } else if (token == "quit") {
      return;
    } else if (token == "position") {
      setPosition(is);
    }

    // Non UCI commands
    else if (token == "printboard") {
      std::cout << _board.getStringRep() << std::endl;
    } else if (token == "printmoves") {
      MoveGen(_board).printMoves();
    }

    else {
      std::cout << "what?" << std::endl;
    }
  }
}
