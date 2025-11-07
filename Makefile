# Makefile for XENOSTRIKE

CXX = /usr/bin/clang++
CXXFLAGS = -std=c++17 -Wall -g -Wno-deprecated -fdiagnostics-color=always

PROJECT_ROOT = /Users/fatemehosseini/Documents/Documents - Fateme’s MacBook Pro/ECG/Project

INCLUDE_PATHS = -Iinclude -Idependencies/include -I/opt/homebrew/include
LIBRARY_PATHS = -Ldependencies/library -L/opt/homebrew/lib

# Source files
SOURCES = \
    main.cpp \
    shader.cpp \
    src/alien.cpp \
    src/collision.cpp \
    src/game_manager.cpp \
    src/particle.cpp \
    src/plasma.cpp \
    src/renderer.cpp \
    src/shield.cpp \
    src/spacecraft.cpp \
    src/tentacle.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Libraries and Frameworks for macOS linking
LDFLAGS = $(LIBRARY_PATHS) \
    -lglfw.3.4 -lGLEW \
    -framework OpenGL \
    -framework Cocoa \
    -framework IOKit \
    -framework CoreVideo \
    -framework CoreFoundation

# Output binary
TARGET = app

# --- Build Rules ---

# Default goal: build the target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJECTS)
	@echo "Linking executable: $(notdir $@)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile all source files into object files
# The '$<' is the prerequisite (the .cpp file), and '$@' is the target (the .o file)
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATHS) -c $< -o $@

# --- Utility Rules ---

.PHONY: clean run all

clean:
	@echo "Cleaning up object files and executable..."
	# Remove object files from root and src folders
	rm -f $(TARGET) main.o shader.o src/*.o 

run: $(TARGET)
	@echo "Running $(notdir $(TARGET))..."
	./$(TARGET)
