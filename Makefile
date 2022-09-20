CXX_FLAGS = -std=c++17 -Wpedantic -g -lpthread
OPT_FLAGS = -std=c++17 -Wpedantic -lpthread -O3
BUILD_DIR = build
TARGETS = $(BUILD_DIR)/neighbour_tests_opt $(BUILD_DIR)/naive_tests_opt $(BUILD_DIR)/neighbour_tests $(BUILD_DIR)/naive_tests $(BUILD_DIR)/neighbour_par_tests $(BUILD_DIR)/neighbour_par_tests_opt

all: $(TARGETS)

$(BUILD_DIR)/neighbour_par_tests_opt: test_main.cpp $(BUILD_DIR)/neighbour_refs_par.o
	g++ -DNEIGHBOUR_PAR $(OPT_FLAGS) $^ -o $@

$(BUILD_DIR)/neighbour_tests_opt: test_main.cpp $(BUILD_DIR)/neighbour_refs.o
	g++ -DNEIGHBOUR $(OPT_FLAGS) $^ -o $@

$(BUILD_DIR)/naive_tests_opt: test_main.cpp $(BUILD_DIR)/naive.o
	g++ -DNAIVE $(OPT_FLAGS) $^ -o $@

$(BUILD_DIR)/neighbour_par_tests: test_main.cpp $(BUILD_DIR)/neighbour_refs_par.o
	g++ $(CXX_FLAGS) -DNEIGHBOUR_PAR $^ -o $@

$(BUILD_DIR)/neighbour_tests: test_main.cpp $(BUILD_DIR)/neighbour_refs.o
	g++ $(CXX_FLAGS) -DNEIGHBOUR $^ -o $@

$(BUILD_DIR)/naive_tests: test_main.cpp $(BUILD_DIR)/naive.o
	g++ $(CXX_FLAGS) -DNAIVE $^ -o $@


$(BUILD_DIR)/naive.o: naive.cpp
	g++ $(CXX_FLAGS) $^ -c -o $@

$(BUILD_DIR)/neighbour_refs.o: neighbour_refs.cpp
	g++ $(CXX_FLAGS) $^ -c -o $@

$(BUILD_DIR)/neighbour_refs_par.o: neighbour_refs_par.cpp
	g++ $(CXX_FLAGS) $^ -c -o $@

.PHONY: clean
clean: 
	rm -rf $(BUILD_DIR)/*
