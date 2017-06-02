all:
	g++ -g -std=c++11 main.cc board.cc cmove.cc -o chess

clean:
	rm chess
