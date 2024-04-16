CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: $(BIN_DIR)/main

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/main: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/main
