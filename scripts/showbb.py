'''
Pretty prints a bitboard to the console given its hex or decimal value.
'''
import sys, textwrap

if len(sys.argv) == 1:
    print 'Syntax: showbb.py <bitboard>'
    sys.exit(1)

# Handle hex/decimal bitboards
if sys.argv[1].startswith('0x'):
    bb = bin(int(sys.argv[1], 16))
else:
    bb = bin(int(sys.argv[1], 10))

# Slice off the '0b'
bb = bb[2:]

if len(bb) < 64:
    bb = '0' * (64-len(bb)) + bb

print '\n'.join([line[::-1] for line in textwrap.wrap(bb, 8)])
