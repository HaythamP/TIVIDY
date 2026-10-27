SHELL          := /bin/bash
.SHELLFLAGS    := -eu -o pipefail -c

CXX            := g++
CXXFLAGS       := -std=c++11 -Wall -Wextra -pedantic -g -Isrc -Isrc/include
TEST_LDFLAGS   := -lgtest -lgtest_main -pthread

SRC_DIR        := src
TEST_DIR       := testing
BUILD_DIR      := build
BIN_DIR        := bin

VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

TARGET         := $(BIN_DIR)/TIVIDY

# ONLY compile non-templated classes into standalone .o files (exceptions)
LIB_SRCS       := $(wildcard $(SRC_DIR)/*Exception.cpp)
LIB_OBJS       := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SRCS))

# Discover all modular test suites in testing/
TEST_SRCS      := $(wildcard $(TEST_DIR)/test*.cpp)
TEST_BINS      := $(patsubst $(TEST_DIR)/test%.cpp, $(BIN_DIR)/test%, $(TEST_SRCS))

# Default target builds all tests while main.cpp is not ready
all: test-build

# Rule to compile non-templated exception objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build each test binary independently
$(BIN_DIR)/test_%: $(TEST_DIR)/test_%.cpp $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJS) -o $@ $(TEST_LDFLAGS)

test-build: $(if $(strip $(TEST_BINS)), $(TEST_BINS),)

# Run every compiled test binary sequentially
test: test-build
	@if [ -z "$(strip $(TEST_BINS))" ]; then \
		echo "No tests found in $(TEST_DIR)/. Skipping test execution."; \
	else \
		set -e; \
		for t in $(TEST_BINS); do \
			echo "=== Running $$t ==="; \
			./$$t; \
		done; \
	fi

# Run Valgrind against every test binary with strict zero-leak enforcement
valgrind: test-build
	@if [ -z "$(strip $(TEST_BINS))" ]; then \
		echo "No tests found in $(TEST_DIR)/. Skipping Valgrind verification."; \
	else \
		set -e; \
		for t in $(TEST_BINS); do \
			echo "=== Valgrind: $$t ==="; \
			valgrind $(VALGRIND_FLAGS) ./$$t; \
		done; \
	fi

# Optional application target for when main.cpp is created later
$(TARGET): $(LIB_OBJS) $(BUILD_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

re: clean all

.PHONY: all test-build test valgrind clean re