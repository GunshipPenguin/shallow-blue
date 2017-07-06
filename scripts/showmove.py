'''
Showmove.py

Given the value of a move's internal _move variable, print the move and all
it's associated info.
'''
import sys

FILES = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
FLAGS = {
    1<<0: 'NULL_MOVE',
    1<<1: 'CAPTURE',
    1<<2: 'DOUBLE_PAWN_PUSH',
    1<<3: 'KSIDE_CASTLE',
    1<<4: 'QSIDE_CASTLE',
    1<<5: 'EN_PASSANT',
    1<<6: 'PROMOTION'
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

    # PieceType
    piece_type = (move & 0x7)
    print "piece type: " + PIECE_TYPES[piece_type]

    # Promotion piece type
    promotion_piece_type = (move >> 3) & 0x7
    print "promotion piece: " + PIECE_TYPES[promotion_piece_type]

    # Captured piece type
    captured_piece_type = (move >> 6) & 0x7
    print "captured piece: " + PIECE_TYPES[captured_piece_type]

    # From square
    from_square_index = (move >> 9) & 0x3f
    from_square = FILES[from_square_index % 8] + str((from_square_index // 8) + 1)
    print "from: " + from_square + " (" + str(from_square_index) + ")"

    # To square
    to_square_index = (move >> 15) & 0x3f
    to_square = FILES[to_square_index % 8] + str((to_square_index // 8) + 1)
    print "to: " + to_square + " (" + str(to_square_index) + ")"

    # Flags
    print "flags:",
    flag_section = (move >> 21) & 0x1ff
    for flag in FLAGS:
        if flag_section & flag:
            print FLAGS[flag],
    print ''

if len(sys.argv) == 1:
    for line in sys.stdin.readlines():
        print_move(line.strip())
else:
    print_move(sys.argv[1])
