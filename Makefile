# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -g 

# Target executables
TARGET_PIPELINE = mst_solver_pipeline
TARGET_THREADPOOL = mst_solver_threadpool

# Source files
COMMON_SRCS = graph.cpp vertex.cpp reactor.cpp Threadpool.cpp MSTAlgo.cpp Pipeline/ActiveObject.cpp Pipeline/MSTPipeline.cpp
MAIN_PIPELINE = mainPipeline.cpp
MAIN_THREADPOOL = mainThreadpool.cpp

# Object files
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
PIPELINE_OBJ = $(MAIN_PIPELINE:.cpp=.o)
THREADPOOL_OBJ = $(MAIN_THREADPOOL:.cpp=.o)

# Include directories
INCLUDES = -I.

all: $(TARGET_PIPELINE) $(TARGET_THREADPOOL)

# Rule to build the pipeline target
$(TARGET_PIPELINE): $(PIPELINE_OBJ) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $(PIPELINE_OBJ) $(COMMON_OBJS) -o $(TARGET_PIPELINE)

# Rule to build the threadpool target
$(TARGET_THREADPOOL): $(THREADPOOL_OBJ) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $(THREADPOOL_OBJ) $(COMMON_OBJS) -o $(TARGET_THREADPOOL)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and the target
clean:
	rm -f $(COMMON_OBJS) $(PIPELINE_OBJ) $(THREADPOOL_OBJ) $(TARGET_PIPELINE) $(TARGET_THREADPOOL)
# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -g 

# Target executables
TARGET_PIPELINE = mst_solver_pipeline
TARGET_THREADPOOL = mst_solver_threadpool

# Source files
COMMON_SRCS = graph.cpp vertex.cpp reactor.cpp Threadpool.cpp MSTAlgo.cpp Pipeline/ActiveObject.cpp Pipeline/MSTPipeline.cpp
MAIN_PIPELINE = mainPipeline.cpp
MAIN_THREADPOOL = mainThreadpool.cpp

# Object files
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
PIPELINE_OBJ = $(MAIN_PIPELINE:.cpp=.o)
THREADPOOL_OBJ = $(MAIN_THREADPOOL:.cpp=.o)

# Include directories
INCLUDES = -I.

all: $(TARGET_PIPELINE) $(TARGET_THREADPOOL)

# Rule to build the pipeline target
$(TARGET_PIPELINE): $(PIPELINE_OBJ) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $(PIPELINE_OBJ) $(COMMON_OBJS) -o $(TARGET_PIPELINE)

# Rule to build the threadpool target
$(TARGET_THREADPOOL): $(THREADPOOL_OBJ) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $(THREADPOOL_OBJ) $(COMMON_OBJS) -o $(TARGET_THREADPOOL)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and the target
clean:
	rm -f $(COMMON_OBJS) $(PIPELINE_OBJ) $(THREADPOOL_OBJ) $(TARGET_PIPELINE) $(TARGET_THREADPOOL)
