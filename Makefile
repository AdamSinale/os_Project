# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++17 -Wall -g -lpthread

# Target executable
TARGET = mst_solver

# Source files
SRCS = main.cpp graph.cpp vertex.cpp reactor.cpp Threadpool.cpp pipeline.cpp MSTAlgo.cpp

# Object files (generated by the compiler)
OBJS = $(SRCS:.cpp=.o)

# Include directories
INCLUDES = -I.

all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to build the object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and the target
clean:
	rm -f $(OBJS) $(TARGET)
