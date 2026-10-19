
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -Isrc/include
LDFLAGS := 


SRC_DIR := src
BUILD_DIR:= build
TARGET := $(BUILD_DIR)/program


SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))


all: $(TARGET)


$(TARGET):$(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -rf $(BUILD_DIR)

re: clean all

.PHONY: all clean re