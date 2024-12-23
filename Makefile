# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Output executable file
OUTPUT = sim

# Source files
SRCS = cpu_scheduling.cpp

# Object files (automatically generated from source files)
OBJS = $(SRCS:.cpp=.o)

# The default rule: build the output program
all: $(OUTPUT)

# Rule to build the executable from object files
$(OUTPUT): $(OBJS)
	$(CXX) $(OBJS) -o $(OUTPUT)

# Rule to build object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object and executable files
clean:
	rm -f $(OBJS) $(OUTPUT)

