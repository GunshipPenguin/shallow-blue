#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "board.h"
#include "movegen.h"

#include <time.h>
#include <stdlib.h>

#include <random>

int main() {
  // TODO Pawns up 2 Fix
  // TODO Capture Fix

  // TODO Fix this bugged FEN (h1e1 is not a legal move)
  // board.setToFen("r1b1k1nr/ppp2ppp/4pq2/8/3p2P1/3nPP1P/PPPPNK1b/R1BQ1BNR w kq - 6 10");

  Board board;
  MoveGen movegen;

  board.setToFen("8/8/8/8/3P4/4Q3/8/8 w - -");
  movegen.setBoard(board);

  for(auto move : movegen.getMoves()) {
    std::cout << move.getNotation() << std::endl;
  }

  //TODO Fix this bugged FEN (h1b1 is not a legal move)
//   Board board;
//   board.setToFen("rn1qkbnr/ppp2ppp/4b3/3pp3/8/2NP3P/PPP1PPP1/R1BQKBNR w KQkq - 2 4");

  // Board board;
  // board.setToFen("rnbqkbnr/7N/8/pppp2p1/P2Ppp2/2Q5/1PPBPPPP/RN2KB1R w KQkq - 0 10");
  // std::cout << board.getStringRep() << std::endl;
  for(auto move : movegen.getMoves()) {
    std::cout << move.getNotation() << std::endl;
  }

  // Board board;
  //
  // board.setToStartPos();
  // for (auto move : board.getMoves()) {
  //   std::cout << move.getNotation() << std::endl;
  // }
  //


  // std::string line;
  // Board board;
  // board.setToStartPos();
  //
  // std::ofstream outputFile("/home/rhys/chess/out.txt");
  //
  // outputFile << "----------" << std::endl;
  // std::cout.setf (std::ios::unitbuf);
  //
  // std::string token;
  //
  // while(std::getline(std::cin, line)) {
  //   outputFile << line << std::endl;
  //   std::istringstream is(line);
  //   is >> token;
  //
  //   if (token == "uci") {
  //     std::cout << "id name chess" << std::endl;
  //     std::cout << "id author rhys" << std::endl << std::endl;
  //     std::cout << "uciok" << std::endl;
  //   } else if (token == "isready") {
  //     std::cout << "readyok" << std::endl;
  //   } else if (token == "go") {
  //     MoveList moves = board.getMoves();
  //
  //     srand (time(NULL));
  //     int randInt = rand() % moves.size() + 1;
  //     CMove move = moves[randInt];
  //
  //     std::cout << "bestmove " << move.getNotation() << std::endl;
  //
  //     board.doMove(move);
  //   } else if (token == "quit") {
  //     return 0;
  //   } else if (token == "position") {
  //     std::string fen;
  //
  //     is >> token;
  //
  //     if (token == "startpos") {
  //       board.setToStartPos();
  //     } else {
  //       while (is >> token && token != "moves")
  // 			   fen += token + " ";
  //
  //       board.setToFen(fen);
  //     }
  //
  //     // Parse move list (if any)
  //   	while (is >> token) {
  //   		if (token != "moves") {
  //   			CMove move(token);
  //         board.doMove(move);
  //   		}
  //   	}
  //
  //   } else {
  //     std::cout << "What?" << std::endl;
  //   }
  // }

  return 0;
}
