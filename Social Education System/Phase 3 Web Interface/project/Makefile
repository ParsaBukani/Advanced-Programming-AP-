# Compiler
CXX = g++
CXXFLAGS = -std=c++14

# Target executable
TARGET = utms

# Source files
SRCS = main.cpp system.cpp admin.cpp readFile.cpp student.cpp professor.cpp user.cpp utilities.cpp

# Header files
HDRS = readFile.hpp system.hpp admin.hpp student.hpp professor.hpp user.hpp consts.hpp utilities.hpp

# Build directory
BUILDDIR = build

# Object files (inside the build directory)
OBJS = $(addprefix $(BUILDDIR)/, $(SRCS:.cpp=.o))

# Default target
all: $(BUILDDIR) $(TARGET)

# Create the build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile source files into object files
$(BUILDDIR)/%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Declare phony targets
.PHONY: all clean

# Clean up object files and target executable
clean:
	rm -f $(OBJS) $(TARGET)