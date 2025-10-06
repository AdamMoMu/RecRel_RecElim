CXX = g++
CXXFLAGS = -I/usr/rose/include/rose -std=c++17
LDFLAGS = -L/usr/rose/lib -lrose -ldl -lpthread -lstdc++

SRC = src/transformation_helper.cpp src/transformation_divconq.cpp \
      src/tail.cpp src/helper.cpp src/analysis.cpp

HEADERS = src/transformation_helper.h src/transformation_divconq.h \
          src/tail.h src/helper.h

OBJ = $(SRC:.cpp=.o)
TARGET = analysis_tool

TEST_DIR = benchmarks/original
RESULT_DIR = benchmarks/RecRel_RecElim

BENCHMARKS = bin_search fact fib fib_mod gcd heap_sum mergesort ms_ternary sum_list

# Default target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)

# Run analysis_tool only on specified benchmarks
test: $(TARGET)
	@echo "Running analysis_tool on selected benchmarks..."
	@for bench in $(BENCHMARKS); do \
		echo "Processing $$bench..."; \
		./$(TARGET) -I $(TEST_DIR)/$$bench $(TEST_DIR)/$$bench/prog.cpp $(RESULT_DIR)/$$bench/prog.cpp; \
	done
	@echo "All selected benchmarks processed."

.PHONY: all clean test