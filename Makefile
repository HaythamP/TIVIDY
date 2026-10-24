CXX            := g++
CXXFLAGS       := -std=c++11 -Wall -Wextra -pedantic -g -Isrc -Isrc/includes
TEST_LDFLAGS   := -lgtest -lgtest_main -pthread

SRC_DIR        := src
TEST_DIR       := testing
BUILD_DIR      := build
BIN_DIR        := bin

VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

TARGET         := $(BIN_DIR)/TIVIDY

# All implementation source files in src/ (exceptions, non-template utilities)
SRCS           := $(wildcard $(SRC_DIR)/*.cpp)
LIB_SRCS       := $(filter-out $(SRC_DIR)/main.cpp, $(SRCS))
LIB_OBJS       := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SRCS))

# Test suites
TEST_SRCS      := $(wildcard $(TEST_DIR)/test_*.cpp)
TEST_BINS      := $(patsubst $(TEST_DIR)/test_%.cpp, $(BIN_DIR)/test_%, $(TEST_SRCS))

# Default target runs the tests while main.cpp is not yet implemented
all: test-build

# Rule for building object files from src/*.cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build each test binary independently, linking compiled non-template src objects
$(BIN_DIR)/test_%: $(TEST_DIR)/test_%.cpp $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJS) -o $@ $(TEST_LDFLAGS)

test-build: $(if $(strip $(TEST_BINS)), $(TEST_BINS),)

# Run every test binary produced
test: test-build
	@if [ -z "$(TEST_BINS)" ]; then \
		echo "No tests found in $(TEST_DIR)/. Skipping test run."; \
		exit 0; \
	else \
		for t in $(TEST_BINS); do \
			echo "=== Running $$t ==="; \
			./$$t || exit 1; \
		done; \
	fi

# Run Valgrind against every test binary produced
valgrind: test-build
	@if [ -z "$(TEST_BINS)" ]; then \
		echo "No tests found in $(TEST_DIR)/. Skipping Valgrind verification."; \
		exit 0; \
	else \
		for t in $(TEST_BINS); do \
			echo "=== Valgrind: $$t ==="; \
			valgrind $(VALGRIND_FLAGS) ./$$t || exit 1; \
		done; \
	fi

# Main target for when main.cpp is eventually added
$(TARGET): $(LIB_OBJS) $(BUILD_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

re: clean all

.PHONY: all test-build test valgrind clean re