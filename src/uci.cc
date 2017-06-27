#include "uci.h"
#include "board.h"
#include "defs.h"
#include "cmove.h"
#include "movegen.h"
#include "search.h"
#include <iostream>
#include <thread>
#include <sstream>

void Uci::_uciNewGame(){
  _board.setToStartPos();
}

void Uci::_setPosition(std::istringstream& is) {
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

void Uci::_pickBestMove(int maxDepth) {
  Search search(_board);

  for (int currDepth=1;currDepth<=maxDepth;currDepth++) {
    search.perform(currDepth);
  }

  std::cout << "bestmove " << search.getBestMove().getNotation() << std::endl;
}

void Uci::_go(std::istringstream& is) {
  std::string token;
  int depth = DEFAULT_DEPTH;
  while (is >> token) {
    if (token == "depth") is >> depth;
  }

  std::thread searchThread(&Uci::_pickBestMove, this, depth);
  searchThread.detach();
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
      std::cout << "id author rhys" << std::endl;
      std::cout << "uciok" << std::endl;
    } else if (token == "ucinewgame") {
      _uciNewGame();
    } else if (token == "isready") {
      std::cout << "readyok" << std::endl;
    } else if (token == "go") {
      _go(is);
    } else if (token == "quit") {
      return;
    } else if (token == "position") {
      _setPosition(is);
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
