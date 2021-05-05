SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/P4
FIL := $(OBJ_DIR)/P4
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CC := g++ -std=c++11
CPPFLAGS := -Iinclude -MMD -MP
CFLAGS := -Wall -g

.PHONY: all test clean cleanrun

all: $(EXE)

$(EXE): $(filter-out $(FIL), $(OBJ)) | $(BIN_DIR)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) kb.* test-files/*.asm

-include $(OBJ:.o=.d)