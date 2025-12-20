CXX = clang++
CXXFLAGS = -Wall -Wextra -g -std=c++17
SRCS = src/Lab2.cpp src/MyString.cpp src/MyVector.cpp src/Rect.cc src/Circle.cc src/Shape.cc
BUILD_DIR = build
OBJS = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = $(BUILD_DIR)/lab2

# make the build directory if it doesn't exist
.PHONY: all $(BUILD_DIR) run debug clean

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# compiling to obj
$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	lldb ./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)