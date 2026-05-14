# ============================================
# C++ II Semester Project - Shielding Simulation
# ============================================

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Executable name
TARGET = shielding_sim

# Test build
TEST_TARGET = runTests
TEST_OBJS = $(filter-out main.o,$(OBJS))

# Source files
SRCS = \
	main.cpp \
	Particle.cpp \
	Shielding.cpp \
	ROI.cpp \
	Output.cpp \
	Simulation.cpp \
	GeometryOutput.cpp \
	Source.cpp \
	Octree.cpp

# Object files (replace .cpp by .o)
OBJS = $(SRCS:.cpp=.o)

# --------------------------------------------
# Default target
# --------------------------------------------
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --------------------------------------------
# Utility targets
# --------------------------------------------

# Remove compiled files
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_TARGET)

# Rebuild from scratch
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run

# Test code
test: clean $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): test.cpp $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^