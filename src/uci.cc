#include "uci.h"
#include "board.h"
#include "defs.h"
#include "move.h"
#include "movegen.h"
#include "search.h"
#include "version.h"
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

void Uci::_pickBestMove(Search::Limits limits) {
  Search search(_board, limits);
  search.iterDeep();
  std::cout << "bestmove " << search.getBestMove().getNotation() << std::endl;
}

void Uci::_go(std::istringstream& is) {
  std::string token;
  Search::Limits limits;

  while (is >> token) {
    if (token == "depth") is >> limits.depth;
    else if (token == "nodes") is >> limits.nodes;
    else if (token == "wtime") is >> limits.time[WHITE];
    else if (token == "btime") is >> limits.time[BLACK];
    else if (token == "winc") is >> limits.increment[WHITE];
    else if (token == "binc") is >> limits.increment[BLACK];
    else if (token == "movestogo") is >> limits.movesToGo;
  }

  std::thread searchThread(&Uci::_pickBestMove, this, limits);
  searchThread.detach();
}

unsigned long long Uci::_perft(const Board& board, int depth) {
  if (depth == 0) {
    return 1;
  }

  MoveGen movegen(board);
  Board movedBoard;

  unsigned long long nodes = 0;
  for (auto move : movegen.getLegalMoves()) {
    movedBoard = board;
    movedBoard.doMove(move);

    nodes += _perft(movedBoard, depth-1);
  }

  return nodes;
}

void Uci::_perftDivide(int depth) {
  unsigned long long total = 0;

  MoveGen movegen(_board);

  std::cout << std::endl;
  auto start = std::chrono::steady_clock::now();
  for (auto move : movegen.getLegalMoves()) {
    Board movedBoard = _board;
    movedBoard.doMove(move);

    unsigned long long perftRes = _perft(movedBoard, depth-1);
    total += perftRes;

    std::cout << move.getNotation() << ": " << perftRes << std::endl;
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end-start;

  std::cout << std::endl << "========" << std::endl;
  std::cout << "Total nodes: " << total << std::endl;
  std::cout << "Time elapsed (ms): " << elapsed.count() * 1000 << std::endl;
  std::cout << "Nodes per second: " << static_cast<int>(total / elapsed.count()) << std::endl;
}

void Uci::start() {
  std::cout << "Shallow Blue " << VER_MAJ << "." << VER_MIN << "." << VER_PATCH;
  std::cout << " by Rhys Rustad-Elliott";
  std::cout << " (built " << __DATE__ << " " << __TIME__ << ")" << std::endl;

  #ifdef __DEBUG__
    std::cout << "***DEBUG BUILD (This will be slow)***" << std::endl;
  #endif

  _board.setToStartPos();

  std::string line;
  std::string token;

  std::ofstream of("uci.out");

  while(std::getline(std::cin, line)) {
    of << line << std::endl;
    std::istringstream is(line);
    is >> token;

    if (token == "uci") {
      std::cout << "id name Shallow Blue" << VER_MAJ << "." << VER_MIN << "." << VER_PATCH << std::endl;
      std::cout << "id author Rhys Rustad-Elliott" << std::endl;
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
      for (auto move : MoveGen(_board).getLegalMoves()) {
        std::cout << move.getNotation() << std::endl;
      }
    } else if (token == "perft") {
      int depth;
      is >> depth;
      _perftDivide(depth);
    }

    else {
      std::cout << "what?" << std::endl;
    }
  }

  of.close();
}
