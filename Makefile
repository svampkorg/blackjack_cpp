# Compiler to use
CXX = g++

# Compiler flags
# -Iheaders specifies the directory for header files
# -Wall enables all warnings
# -g adds debugging information
CXXFLAGS = -std=c++17 -Iheaders/ -Wall -g

# The name of your final executable
TARGET = blackjack

# List of all your .cc source files
SOURCES = art.cc arthandling.cc blackjack.cc blackjacktypes.cc helpers.cc statehandling.cc main.cc

# Replaces the .cc extension with .o to create a list of object files
OBJECTS = $(SOURCES:.cc=.o)

# The default rule, which is run when you just type 'make'
# It depends on the target executable
all: $(TARGET)

# Rule to link all the object files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile a .cc file into a .o object file
# The $< represents the prerequisite (the .cc file) and $@ represents the target (the .o file)
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up the project directory
# Removes all object files and the final executable
clean:
	rm -f $(OBJECTS) $(TARGET)
