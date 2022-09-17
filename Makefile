CXX_FLAGS = -std=c++17 -Wpedantic -g -lpthread
BUILD_DIR = build

$(BUILD_DIR)/tests: test_main.cpp $(BUILD_DIR)/life.o
	g++ $(CXX_FLAGS) $^ -o $@

$(BUILD_DIR)/life.o: life.cpp
	g++ $(CXX_FLAGS) $^ -c -o $@

.PHONY: clean
clean: 
	rm -rf $(BUILD_DIR)/*
