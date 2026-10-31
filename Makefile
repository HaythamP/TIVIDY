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