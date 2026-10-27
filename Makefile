SHELL           := /bin/bash
.SHELLFLAGS     := -eu -o pipefail -c

CXX             := g++
CXXFLAGS        := -std=c++11 -Wall -Werror -Isrc/include -Isrc
TEST_LDFLAGS    := -lgtest -lgtest_main -pthread

SRC_DIR         := src
INC_DIR         := src/include
TEST_DIR        := testing
BUILD_DIR       := build
BIN_DIR         := bin

VALGRIND_FLAGS  := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

.SECONDARY:

# Gather all .cpp files in src except main.cpp
LIB_SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
LIB_OBJS        := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SRCS))

# Discovers whatever tests exist in testing/ (TestExceptions, TestSanity, etc.)
TEST_SRCS       := $(wildcard $(TEST_DIR)/Test*.cpp)
TEST_BINS       := $(patsubst $(TEST_DIR)/Test%.cpp, $(BIN_DIR)/Test%, $(TEST_SRCS))

all: test-build

$(BUILD_DIR) $(BIN_DIR):
	@mkdir -p $@

# Compile rule for individual source objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INC_DIR)/*.h) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile each test executable by linking with library objects
$(BIN_DIR)/Test%: $(TEST_DIR)/Test%.cpp $(LIB_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJS) -o $@ $(TEST_LDFLAGS)

test-build: $(if $(strip $(TEST_BINS)), $(TEST_BINS),)

# Run test executables sequentially; stop immediately on the first failure
test: test-build
	@if [ -z "$(strip $(TEST_BINS))" ]; then \
		echo "No tests found in $(TEST_DIR)/Test*.cpp"; \
	else \
		set -e; \
		for t in $(TEST_BINS); do \
			echo "=== Running $$t ==="; \
			./$$t; \
		done; \
		echo "=== ALL TESTS PASSED ==="; \
	fi

# Run Valgrind leak check across all test executables
valgrind: test-build
	@if [ -z "$(strip $(TEST_BINS))" ]; then \
		echo "No tests found in $(TEST_DIR)/Test*.cpp"; \
	else \
		set -e; \
		for t in $(TEST_BINS); do \
			echo "=== Valgrind: $$t ==="; \
			valgrind $(VALGRIND_FLAGS) ./$$t; \
		done; \
		echo "=== NO MEMORY LEAKS DETECTED ==="; \
	fi

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

re: clean all

.PHONY: all test-build test valgrind clean re