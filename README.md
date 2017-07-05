# Shallow Blue
(Not [Deep Blue]( https://en.wikipedia.org/wiki/Deep_Blue_(chess_computer)))

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

## Tests

[Catch](https://github.com/philsquared/Catch) unit tests are located in the `test` directory.

To build and run the unit tests, use:

`make test`

## Implemented non UCI Commands

These commands can be useful for debugging.

- `printboard`
    - Pretty prints the current state of the game board
- `printmoves`
    - Prints all pseudo-legal moves for the currently active player

# License

[MIT](https://github.com/GunshipPenguin/shallow-blue/blob/master/LICENSE) © Rhys Rustad-Elliott
