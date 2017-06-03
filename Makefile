CPP_FILES := $(wildcard src/*.cc)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cc=.o)))
LD_FLAGS :=
CC_FLAGS := -std=c++11
OBJ_DIR := obj
BIN_NAME := chess

all: $(OBJ_DIR) $(BIN_NAME)

$(BIN_NAME): $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cc
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -r $(OBJ_DIR)
	rm $(BIN_NAME)
