'''
Showmove.py

Given the value of a move's internal _move variable, print the move and all
it's associated info.
'''
import sys

FILES = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
FLAGS = {
    1<<0: 'CAPTURE',
    1<<1: 'DOUBLE_PAWN_PUSH',
    1<<2: 'KSIDE_CASTLE',
    1<<3: 'QSIDE_CASTLE',
    1<<4: 'EN_PASSANT',
    1<<5: 'QUEEN_PROMOTION',
    1<<6: 'KNIGHT_PROMOTION',
    1<<7: 'ROOK_PROMOTION',
    1<<8: 'BISHOP_PROMOTION',
    1<<9: 'NULL_MOVE'
}
PIECE_TYPES = {
  0: 'PAWN',
  1: 'ROOK',
  2: 'KNIGHT',
  3: 'BISHOP',
  4: 'QUEEN',
  5: 'KING'
}

def print_move(moveStr):
    move = int(moveStr, 10)

    # From square
    from_square_index = (move >> 3) & 0x3f
    from_square = FILES[from_square_index % 8] + str((from_square_index // 8) + 1)
    print "FROM: " + from_square + " (" + str(from_square_index) + ")"

    # To square
    to_square_index = (move >> 9) & 0x3f
    to_square = FILES[to_square_index % 8] + str((to_square_index // 8) + 1)
    print "TO: " + to_square + " (" + str(to_square_index) + ")"

    # Flags
    print "FLAGS:",
    flag_section = (move >> 15) & 0x1ff
    for flag in FLAGS:
        if flag_section & flag:
            print FLAGS[flag],
    print ''

    # Piece type
    pieceType = move & 0x7
    print "PIECE TYPE: " + PIECE_TYPES[pieceType]

if len(sys.argv) == 1:
    for line in sys.stdin.readlines():
        print_move(line.strip())
else:
    print_move(sys.argv[1])
