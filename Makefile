# Define the compiler to use
CC = gcc

# Define any compile-time flags
CFLAGS = -Wall -g

# Define the directories for source files, header files, and the output directory
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Find all source files in the SRC_DIR
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Find all header files in the INCLUDE_DIR
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Define the object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Define the final executable name
TARGET = $(BIN_DIR)/main

# Default rule
all: $(TARGET)

# Link the object files into the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile the source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@
	@echo "build finished"
# Clean up the build directory
clean:
	rm -rf $(BUILD_DIR)/*

# Clean up the bin directory
clean_bin:
	rm -rf $(BIN_DIR)/*

# Clean up everything
clean_all: clean clean_bin

rebuild: clean_all all
