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

MAIN_BIN        := $(BIN_DIR)/tividy
VALGRIND_FLAGS  := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

.SECONDARY:

# Gather all .cpp files in src except main.cpp
# Compile all src/*.cpp files into build/*.o (excluding main.cpp for tests)
LIB_SRCS := $(filter-out $(SRC_DIR)/server_main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
LIB_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SRCS))

# Discovers whatever tests exist in testing/ (TestExceptions, TestSanity, etc.)
TEST_SRCS       := $(wildcard $(TEST_DIR)/Test*.cpp)
TEST_BINS       := $(patsubst $(TEST_DIR)/Test%.cpp, $(BIN_DIR)/Test%, $(TEST_SRCS))

# Subsets filtered directly from TEST_BINS without altering discovery
INTEGRATION_BINS := $(filter $(BIN_DIR)/TestIntegration%, $(TEST_BINS))
E2E_BINS         := $(filter $(BIN_DIR)/TestE2E% $(BIN_DIR)/TestScenario%, $(TEST_BINS))
UNIT_BINS        := $(filter-out $(INTEGRATION_BINS) $(E2E_BINS), $(TEST_BINS))

# Default target: builds main binary and test suite
all: $(MAIN_BIN) test-build

$(BUILD_DIR) $(BIN_DIR):
	@mkdir -p $@

# Compile rule for individual source objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INC_DIR)/*.h) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile and link the main application (LIB_OBJS is now properly expanded)
$(MAIN_BIN): $(SRC_DIR)/main.cpp $(LIB_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJS) -o $@

# Run the main program
run: $(MAIN_BIN)
	./$(MAIN_BIN)

# Compile each test executable by linking with library objects
$(BIN_DIR)/Test%: $(TEST_DIR)/Test%.cpp $(LIB_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJS) -o $@ $(TEST_LDFLAGS)

# Original target kept completely unchanged
test-build: $(if $(strip $(TEST_BINS)), $(TEST_BINS),)

# Categorized run targets (all rely directly on your unchanged test-build)
test-unit: test-build
	@echo "=== Running Unit Tests ==="
	@if [ -z "$(strip $(UNIT_BINS))" ]; then \
		echo "No unit tests found."; \
	else \
		set -e; \
		for t in $(UNIT_BINS); do \
			echo "=== Running $$t ==="; \
			./$$t; \
		done; \
		echo "=== UNIT TESTS PASSED ==="; \
	fi

test-integration: test-build
	@echo "=== Running Integration Tests ==="
	@if [ -z "$(strip $(INTEGRATION_BINS))" ]; then \
		echo "No integration tests found (skipping)."; \
	else \
		set -e; \
		for t in $(INTEGRATION_BINS); do \
			echo "=== Running $$t ==="; \
			./$$t; \
		done; \
		echo "=== INTEGRATION TESTS PASSED ==="; \
	fi

test-e2e: test-build
	@echo "=== Running End-to-End Tests ==="
	@if [ -z "$(strip $(E2E_BINS))" ]; then \
		echo "No E2E tests found (skipping)."; \
	else \
		set -e; \
		for t in $(E2E_BINS); do \
			echo "=== Running $$t ==="; \
			./$$t; \
		done; \
		echo "=== E2E TESTS PASSED ==="; \
	fi

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
