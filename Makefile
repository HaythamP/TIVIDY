CXX       := g++
CXXFLAGS  := -std=c++11 -Wall -Wextra -pedantic -g -Isrc/includes
LDFLAGS   := 

SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build

TARGET      := TIVIDY
TEST_TARGET := $(BUILD_DIR)/test_runner

# All source files
SRCS      := $(wildcard $(SRC_DIR)/*.cpp)
OBJS      := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Exclude main.cpp from test builds to avoid multiple 'main' definitions
LIB_SRCS  := $(filter-out $(SRC_DIR)/main.cpp, $(SRCS))
LIB_OBJS  := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SRCS))

# Test source files (adjust TEST_DIR if your tests live elsewhere)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/test_%.o, $(TEST_SRCS))

# Valgrind flags: --error-exitcode=1 ensures CI fails if memory errors/leaks occur
VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

all: $(TARGET)

# Link main executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link test executable
$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(LIB_OBJS) $(TEST_OBJS) -o $@ $(LDFLAGS)

# CI Step: 'make test'
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# CI Step: 'make val-all'
val-all: $(TARGET) $(TEST_TARGET)
	valgrind $(VALGRIND_FLAGS) ./$(TEST_TARGET)
	valgrind $(VALGRIND_FLAGS) ./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

re: clean all

.PHONY: all clean re test val-all