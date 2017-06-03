SRC_DIR := $(shell pwd)/src

CPP_FILES := $(wildcard src/*.cc)
TEST_CPP_FILES := $(filter-out src/main.cc, $(sort $(CPP_FILES) $(wildcard test/*.cc)))

OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cc=.o)))
TEST_OBJ_FILES := $(addprefix obj/,$(notdir $(TEST_CPP_FILES:.cc=.o)))

LD_FLAGS :=
CC_FLAGS := -Wall -std=c++11 -g

OBJ_DIR := obj

BIN_NAME := chess
TEST_BIN_NAME := chesstest

all: $(OBJ_DIR) $(BIN_NAME)

test: $(OBJ_DIR) $(TEST_BIN_NAME)
	./$(TEST_BIN_NAME)

$(BIN_NAME): $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

$(TEST_BIN_NAME): $(TEST_OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cc
	g++ $(CC_FLAGS) -c -o $@ $<

obj/%.o: test/%.cc
	g++ $(CC_FLAGS) -I $(SRC_DIR) -c -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TEST_BIN_NAME)
	rm -f $(BIN_NAME)
