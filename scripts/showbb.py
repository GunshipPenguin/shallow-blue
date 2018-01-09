"""
Pretty prints a bitboard to the console given its hex or decimal value from
argv or stdin.
"""
import sys
import textwrap

FULL_BITBOARD = 0xFFFFFFFFFFFFFFFF  # 64 set bits


def print_bb(bb_str):
    # Handle hex/decimal bitboards
    if bb_str.startswith("0x"):
        base = 16
    else:
        base = 10

    if int(bb_str, base).bit_length() > 64:
        print "WARNING: Bit length is greater than 64, taking lowest 64 bits"
    bb = bin(int(bb_str, base) & FULL_BITBOARD)

    # Slice off the "0b"
    bb = bb[2:]

    bb = bb.replace("0", ".")
    bb = bb.replace("1", "1")

    if len(bb) < 64:
        bb = "." * (64 - len(bb)) + bb

    print bb_str
    print "\n".join([" ".join(line[::-1]) for line in textwrap.wrap(bb, 8)]) + "\n"


if len(sys.argv) == 1:
    for line in sys.stdin.readlines():
        print_bb(line.strip())
else:
    print_bb(sys.argv[1])
