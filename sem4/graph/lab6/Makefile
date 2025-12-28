
TEST_DIR = tests
BUILD_DIR = $(TEST_DIR)/build

.PHONY: all test clean

all: test

test:
	@cmake -S $(TEST_DIR) -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR)
	@cd $(BUILD_DIR) && ctest --output-on-failure

clean:
	rm -rf $(BUILD_DIR)