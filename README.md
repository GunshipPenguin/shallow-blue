<p align="center">
  <img src="logos/logo.png" height="220" width="220">
</p>

<h1 align="center">Shallow Blue</h1>
<p align="center">(Not <a href="https://en.wikipedia.org/wiki/Deep_Blue_(chess_computer)">Deep Blue</a>)</p>

<p align="center">
  <a href="https://travis-ci.org/GunshipPenguin/shallow-blue"><img src="https://img.shields.io/travis/GunshipPenguin/shallow-blue/master.svg"></a>
</p>

A UCI Chess engine written in C++11.

Features:
  - [Bitboard](https://en.wikipedia.org/wiki/Bitboard) board representation and move generation
  - [Zobrist hashing](https://en.wikipedia.org/wiki/Zobrist_hashing) / [Transposition Table](https://en.wikipedia.org/wiki/Transposition_table)
  - [Piece square tables](https://chessprogramming.wikispaces.com/Piece-Square+Tables) for evaluation
  - [Iterative deepening](https://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search) [Minimax Search](https://en.wikipedia.org/wiki/Minimax) with  [Alpha-Beta Pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) and [Move Ordering](https://chessprogramming.wikispaces.com/Move+Ordering)
  - [Quiescence search](https://en.wikipedia.org/wiki/Quiescence_search) with [MVV/LVA move ordering](https://chessprogramming.wikispaces.com/MVV-LVA)

## Building

`make`

You can build with debugging symbols and no optimizations using:

`make debug`

If you have Mingw-w64 installed, you can cross compile for Windows on Linux with:

`./build_windows.sh`

## Tests

[Catch](https://github.com/philsquared/Catch) unit tests are located in the `test` directory.

To build and run the unit tests, use:

`make test`

To build and run the unit tests, skipping perft tests (these take a while to run), use:

`make test-noperft`

## Documentation

Shallow Blue's code is extensively documented with Doxygen. To generate HTML
documentation use:

`doxygen`

## Implemented non UCI Commands

These commands can be useful for debugging.

- `perft <depth>`
  - Prints the perft value for each move on the current board to the specified depth
- `printboard`
    - Pretty prints the current state of the game board
- `printmoves`
    - Prints all legal moves for the currently active player

## Future Improvements

- General
  - Avoid draws by repetition / 50 move rule
  - Opening book support
  - Magic bitboards
  - Unmake move function
  - Opening book support
- Evaluation
  - King safety
  - Pawn structure
- Search
  - Switch search algorithm to Principal Variation Search
  - Use a capture-only move generator for quiescence search
- UCI
  - Support more of the UCI protocol
    - Support timers (wtime / btime)
    - `stop` command
    - `go infinite`

# License

[MIT](https://github.com/GunshipPenguin/shallow-blue/blob/master/LICENSE) © Rhys Rustad-Elliott
