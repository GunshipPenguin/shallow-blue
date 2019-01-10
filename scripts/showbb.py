"""
Pretty prints a bitboard to the console given a Python expression that
evaluates to it.

examples:

python showbb.py 45602
python showbb.py 0x10000
python showbb.py "((0x1000 >> 3) | 0x20)"
"""
import sys
import textwrap

FULL_BITBOARD = 0xFFFFFFFFFFFFFFFF  # 64 set bits


def print_bb(bb_evalstring):
    bb = eval(bb_evalstring)
    print hex(bb)

    if bb.bit_length() > 64:
        print "WARNING: Bit length is greater than 64, taking lowest 64 bits"
    bb_str = bin(bb & FULL_BITBOARD)

    # Slice off the "0b"
    bb_str = bb_str[2:]

    bb_str = bb_str.replace("0", ".")
    bb_str = bb_str.replace("1", "1")

    if len(bb_str) < 64:
        bb_str = "." * (64 - len(bb_str)) + bb_str

    print "\n".join([" ".join(line[::-1]) for line in textwrap.wrap(bb_str, 8)]) + "\n"


if len(sys.argv) == 1:
    for line in sys.stdin.readlines():
        print_bb(line.strip())
else:
    print_bb(sys.argv[1])
