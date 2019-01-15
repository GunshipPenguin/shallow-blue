#include <memory>
#include "uci.h"
#include "version.h"
#include <iostream>
#include <thread>

namespace {
Book book;
std::shared_ptr<Search> search;
Board board;
std::vector<ZKey> positionHistory;

void loadBook() {
  std::ifstream bookFile(optionsMap["BookPath"].getValue());
  bool bookOk = bookFile.good();
  bookFile.close();

  if (bookOk) {
    book = Book(optionsMap["BookPath"].getValue());
  } else {
    std::cerr << optionsMap["BookPath"].getValue() << " is inaccessible or doesn't exist" << std::endl;
  }
}

void initOptions() {
  optionsMap["OwnBook"] = Option(false);
  optionsMap["BookPath"] = Option("book.bin", &loadBook);
}

void uciNewGame() {
  board.setToStartPos();
  positionHistory.clear();
}

void setPosition(std::istringstream &is) {
  std::string token;
  is >> token;

  if (token == "startpos") {
    board.setToStartPos();
  } else {
    std::string fen;

    while (is >> token && token != "moves") {
      fen += token + " ";
    }

    board.setToFen(fen);
  }

  while (is >> token) {
    if (token == "moves") {
      continue;
    }

    MoveGen movegen(board);
    for (auto move : movegen.getMoves()) {
      if (move.getNotation() == token) {
        board.doMove(move);
        positionHistory.push_back(board.getZKey());
        break;
      }
    }
  }
}

void pickBestMove() {
  if (optionsMap["OwnBook"].getValue() == "true" && book.inBook(board)) {
    std::cout << "bestmove " << book.getMove(board).getNotation() << std::endl;
  } else {
    search->iterDeep();
  }
}

void go(std::istringstream &is) {
  std::string token;
  Search::Limits limits;

  while (is >> token) {
    if (token == "depth") is >> limits.depth;
    else if (token == "infinite") limits.infinite = true;
    else if (token == "movetime") is >> limits.moveTime;
    else if (token == "nodes") is >> limits.nodes;
    else if (token == "wtime") is >> limits.time[WHITE];
    else if (token == "btime") is >> limits.time[BLACK];
    else if (token == "winc") is >> limits.increment[WHITE];
    else if (token == "binc") is >> limits.increment[BLACK];
    else if (token == "movestogo") is >> limits.movesToGo;
  }

  search = std::make_shared<Search>(board, limits, positionHistory);

  std::thread searchThread(&pickBestMove);
  searchThread.detach();
}

unsigned long long perft(const Board &board, int depth) {
  if (depth <= 0) {
    return 1;
  } else if (depth == 1) {
    return MoveGen(board).getLegalMoves().size();
  }

  MoveGen movegen(board);

  unsigned long long nodes = 0;
  for (auto move : movegen.getLegalMoves()) {
    Board movedBoard = board;
    movedBoard.doMove(move);

    nodes += perft(movedBoard, depth - 1);
  }

  return nodes;
}

void perftDivide(int depth) {
  unsigned long long total = 0;

  MoveGen movegen(board);

  std::cout << std::endl;
  auto start = std::chrono::steady_clock::now();
  for (auto move : movegen.getLegalMoves()) {
    Board movedBoard = board;
    movedBoard.doMove(move);

    unsigned long long perftRes = perft(movedBoard, depth - 1);
    total += perftRes;

    std::cout << move.getNotation() << ": " << perftRes << std::endl;
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  std::cout << std::endl << "==========================" << std::endl;
  std::cout << "Total time (ms) : " << static_cast<int>(elapsed.count() * 1000) << std::endl;
  std::cout << "Nodes searched  : " << total << std::endl;
  std::cout << "Nodes / second  : " << static_cast<int>(total / elapsed.count()) << std::endl;
}

void printEngineInfo() {
  std::cout << "id name Shallow Blue " << VER_MAJ << "." << VER_MIN << "." << VER_PATCH << std::endl;
  std::cout << "id author Rhys Rustad-Elliott" << std::endl;
  std::cout << std::endl;

  for (auto optionPair : optionsMap) {
    std::cout << "option ";
    std::cout << "name " << optionPair.first << " ";
    std::cout << "type " << optionPair.second.getType() << " ";
    std::cout << "default " << optionPair.second.getDefaultValue() << " ";

    if (optionPair.second.getType() == "spin") {
      std::cout << "min " << optionPair.second.getMin() << " ";
      std::cout << "max " << optionPair.second.getMax();
    }
    std::cout << std::endl;
  }
  std::cout << "uciok" << std::endl;
}

void setOption(std::istringstream &is) {
  std::string token;
  std::string optionName;

  is >> token >> optionName; // Advance past "name"

  if (optionsMap.find(optionName) != optionsMap.end()) {
    is >> token >> token; // Advance past "value"
    optionsMap[optionName].setValue(token);
  } else {
    std::cout << "Invalid option" << std::endl;
  }
}

void loop() {
  std::cout << "Shallow Blue " << VER_MAJ << "." << VER_MIN << "." << VER_PATCH;
  std::cout << " by Rhys Rustad-Elliott";
  std::cout << " (built " << __DATE__ << " " << __TIME__ << ")" << std::endl;

#ifdef __DEBUG__
  std::cout << "***DEBUG BUILD (This will be slow)***" << std::endl;
#endif

  board.setToStartPos();

  std::string line;
  std::string token;

  while (std::getline(std::cin, line)) {
    std::istringstream is(line);
    is >> token;

    if (token == "uci") {
      printEngineInfo();
    } else if (token == "ucinewgame") {
      uciNewGame();
    } else if (token == "isready") {
      std::cout << "readyok" << std::endl;
    } else if (token == "stop") {
      if (search) search->stop();
    } else if (token == "go") {
      go(is);
    } else if (token == "quit") {
      if (search) search->stop();
      return;
    } else if (token == "position") {
      setPosition(is);
    } else if (token == "setoption") {
      setOption(is);
    }

      // Non UCI commands
    else if (token == "printboard") {
      std::cout << std::endl << board.getStringRep() << std::endl;
    } else if (token == "printmoves") {
      for (auto move : MoveGen(board).getLegalMoves()) {
        std::cout << move.getNotation() << " ";
      }
      std::cout << std::endl;
    } else if (token == "perft") {
      int depth = 1;
      is >> depth;
      perftDivide(depth);
    } else {
      std::cout << "what?" << std::endl;
    }
  }
}
}

void Uci::init() {
  initOptions();
}

void Uci::start() {
  loop();
}
