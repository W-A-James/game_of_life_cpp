CXX_FLAGS = -std=c++17 -Wpedantic -g -lpthread
BUILD_DIR = build
TARGETS = $(BUILD_DIR)/neighbour_tests_opt $(BUILD_DIR)/naive_tests_opt $(BUILD_DIR)/neighbour_tests $(BUILD_DIR)/naive_tests

all: $(TARGETS)

$(BUILD_DIR)/neighbour_tests_opt: test_main.cpp $(BUILD_DIR)/neighbour_refs.o
	g++ $(CXX_FLAGS) -DNEIGHBOUR -O2 $^ -o $@

$(BUILD_DIR)/naive_tests_opt: test_main.cpp $(BUILD_DIR)/naive.o
	g++ $(CXX_FLAGS) -DNAIVE -O2 $^ -o $@

$(BUILD_DIR)/neighbour_tests: test_main.cpp $(BUILD_DIR)/neighbour_refs.o
	g++ $(CXX_FLAGS) -DNEIGHBOUR $^ -o $@

$(BUILD_DIR)/naive_tests: test_main.cpp $(BUILD_DIR)/naive.o
	g++ $(CXX_FLAGS) -DNAIVE $^ -o $@


$(BUILD_DIR)/naive.o: naive.cpp
	g++ $(CXX_FLAGS) $^ -c -o $@

$(BUILD_DIR)/neighbour_refs.o: neighbour_refs.cpp
	g++ $(CXX_FLAGS) $^ -c -o $@

.PHONY: clean
clean: 
	rm -rf $(BUILD_DIR)/*
