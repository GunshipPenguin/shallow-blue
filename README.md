# Shallow Blue
(Not [Deep Blue]( https://en.wikipedia.org/wiki/Deep_Blue_(chess_computer)))

A UCI Chess engine written entirely in C++.

Features:
  - [Bitboard](https://en.wikipedia.org/wiki/Bitboard) board representation and move generation
  - [Zobrist hashing](https://en.wikipedia.org/wiki/Zobrist_hashing) / [Transposition Table](https://en.wikipedia.org/wiki/Transposition_table)
  - [Piece square tables](https://chessprogramming.wikispaces.com/Piece-Square+Tables) for evaluation
  - [Iterative deepening](https://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search) / [Principal Variation Search](https://en.wikipedia.org/wiki/Principal_variation_search)

## Building

`make`

## Tests

[Catch](https://github.com/philsquared/Catch) unit tests are located in the `test` directory.

To build and run the unit tests, use:

## Implemented non UCI Commands

These commands can be useful for debugging.

- `printboard`
    - Pretty prints the current state of the game board
- `printmoves`
    - Prints all pseudo-legal moves for the currently active player


`make test`

# License

All code released under the MIT license (See LICENSE file).
