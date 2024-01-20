
# Makefile for compiling and linking all CPP files in a folder

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++17 -Wall -g -O0

# Source files directory
SRC_DIR := .

# Output directory
BUILD_DIR := build

#TODO: add header files as deps, or better replace with cmake
# Find all CPP files in the source directory
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Create corresponding object file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Target executable
TARGET := Lox
# Target AST
TARGET_AST_CODEGEN := Expr.h

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_AST_CODEGEN): tool/GenerateAst.py
	python3 tool/GenerateAst.py .

# Rule to build object files from source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(TARGET_AST_CODEGEN)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TARGET_AST_CODEGEN)

.PHONY: all clean
