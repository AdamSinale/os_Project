# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++14

# Target executable
TARGET = main

# Source files
SRC = main.cpp graph.cpp vertex.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Rule to compile individual .cpp files into .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)
