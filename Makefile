# Compiler
CXX = g++

# --- Common compiler flags for both builds ---
# -Iheaders [the directory for header files]
# -Wall [enables all warnings]
CXXFLAGS_COMMON = -std=c++17 -Iheaders/ -Wall

# --- Debug specific flags ---
# -g [adds debugging information]
CXXFLAGS_DEBUG = -g

# --- Release specific flags ---
# -O3 [a high level of optimization]
# -DNDEBUG [defines the NDEBUG macro which disables asserts debug prints]
CXXFLAGS_RELEASE = -O3 -DNDEBUG

# name of executables
TARGET = blackjack-debug
TARGET_RELEASE = blackjack

# List of all .cc source files
SOURCES = $(wildcard src/*.cc)

# List of object files for debug build
OBJECTS = $(SOURCES:.cc=.o)
# List of object files for release build
OBJECTS_RELEASE = $(addprefix obj/release/, $(SOURCES:.cc=.o))

.PHONY: all release clean clean-release clean-all

# Default rule. is ran when typing just 'make'
all: $(TARGET)

# --- Rules for the Debug Build ---
$(TARGET): $(OBJECTS)
	@echo "Linking debug build..."
	$(CXX) $(CXXFLAGS_COMMON) $(CXXFLAGS_DEBUG) -o $(TARGET) $(OBJECTS)

%.o: %.cc
	@echo "Compiling $< for debug..."
	$(CXX) $(CXXFLAGS_COMMON) $(CXXFLAGS_DEBUG) -c $< -o $@


# --- Rules for the Release Build ---
# To build the release version, you will run 'make release'
release: $(TARGET_RELEASE)

$(TARGET_RELEASE): $(OBJECTS_RELEASE)
	@echo "Linking release build..."
	$(CXX) $(CXXFLAGS_COMMON) $(CXXFLAGS_RELEASE) -o $(TARGET_RELEASE) $(OBJECTS_RELEASE)

# New rule specifically for release objects.
# It compiles sources and places the .o files into the 'obj/release' directory.
obj/release/%.o: %.cc
	@echo "Compiling $< for release..."
	@mkdir -p $(@D) # Create the directory if it doesn't exist
	$(CXX) $(CXXFLAGS_COMMON) $(CXXFLAGS_RELEASE) -c $< -o $@


# --- Clean Rules ---
# The default 'clean' removes debug build files.
clean:
	@echo "Cleaning debug files..."
	rm -f $(OBJECTS) $(TARGET)

# A new 'clean-release' for the release files.
clean-release:
	@echo "Cleaning release files..."
	rm -rf obj $(TARGET_RELEASE)

# A rule to clean everything
clean-all: clean clean-release
