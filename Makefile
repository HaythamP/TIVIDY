CXX            := g++
CXXFLAGS       := -std=c++11 -Wall -Wextra -pedantic -g -Isrc -Itividy_headers -Isrc/includes
TEST_LDFLAGS   := -lgtest -lgtest_main -pthread   #link google test

SRC_DIR        := src
TEST_DIR       := testing
BUILD_DIR      := build
BIN_DIR        := bin

VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

TARGET         := $(BIN_DIR)/TIVIDY

# Find all ready source files (excluding main.cpp for test linking)
SRCS           := $(wildcard $(SRC_DIR)/*.cpp)
LIB_SRCS       := $(filter-out $(SRC_DIR)/main.cpp, $(SRCS))
LIB_OBJS       := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SRCS))

# Find all standalone test suites matching test_*.cpp
TEST_SRCS      := $(wildcard $(TEST_DIR)/test_*.cpp)
TEST_BINS      := $(patsubst $(TEST_DIR)/test_%.cpp, $(BIN_DIR)/test_%, $(TEST_SRCS))

all: $(TARGET)

# Compile src objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile the main project binary (only if main.cpp exists)
$(TARGET): $(LIB_OBJS) $(BUILD_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@

# Build each test binary independently, linking only compiled src objects
$(BIN_DIR)/test_%: $(TEST_DIR)/test_%.cpp $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJS) -o $@ $(TEST_LDFLAGS)

test-build: $(TEST_BINS)

# Run every test binary produced
test: test-build
	@if [ -z "$(TEST_BINS)" ]; then \
		echo "No tests found in $(TEST_DIR)/"; \
	else \
		for t in $(TEST_BINS); do \
			echo "=== Running $$t ==="; \
			./$$t || exit 1; \
		done; \
	fi

# Run Valgrind against every test binary produced
valgrind: test-build
	@if [ -z "$(TEST_BINS)" ]; then \
		echo "No tests found to run Valgrind on."; \
	else \
		for t in $(TEST_BINS); do \
			echo "=== Valgrind: $$t ==="; \
			valgrind $(VALGRIND_FLAGS) ./$$t || exit 1; \
		done; \
	fi

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

re: clean all

.PHONY: all test-build test valgrind clean re

# make, just builds
# make test-build compile tests/test_*.cpp
# make test, test binaries sequentially
# make valgrind (memory check)
# make clean ( deletes build/ and bin/ )
# make re (make clean && make all)