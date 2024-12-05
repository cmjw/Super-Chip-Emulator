# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I/usr/include/SDL2
LDFLAGS = -lSDL2

# Source directories and files
SRC_DIR = src
DISASSEMBLER_DIR = disassemble

SOURCES = $(SRC_DIR)/chip8.cpp $(SRC_DIR)/op.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/chip8video.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = emulator

DISASSEMBLER_SOURCES = $(DISASSEMBLER_DIR)/main.cpp $(DISASSEMBLER_DIR)/disassembler.cpp
DISASSEMBLER_OBJECTS = $(DISASSEMBLER_SOURCES:.cpp=.o)
DISASSEMBLER_EXECUTABLE = disassembler

# Default target
all: $(EXECUTABLE) $(DISASSEMBLER_EXECUTABLE)

# Emulator
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Disassembler
$(DISASSEMBLER_EXECUTABLE): $(DISASSEMBLER_OBJECTS)
	$(CXX) $(DISASSEMBLER_OBJECTS) -o $@

$(DISASSEMBLER_DIR)/%.o: $(DISASSEMBLER_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(DISASSEMBLER_OBJECTS) $(EXECUTABLE) $(DISASSEMBLER_EXECUTABLE)

# Phony targets
.PHONY: all clean