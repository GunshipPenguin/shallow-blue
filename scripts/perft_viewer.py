'''
Given two files containing correct and incorrect perft divide data in the format

[move]: [number]\n

display the inaccuracy in the incorrect file.
'''
import sys

if len(sys.argv) < 3:
    print "Usage: python perft_viewer.py <correct perft file> <incorrect perft file>"
    sys.exit(1)

correctFile = open(sys.argv[1], 'r')
incorrectFile = open(sys.argv[2], 'r')

correctMap = {result.split(": ")[0]:int(result.split(": ")[1]) for result in correctFile.readlines()}
incorrectMap = {result.split(": ")[0]:int(result.split(": ")[1]) for result in incorrectFile.readlines()}

for move in correctMap:
    try:
        if correctMap[move] != incorrectMap[move]:
            print "INCORRECT: ", move
    except KeyError:
        print "MOVE NOT FOUND: ", move
