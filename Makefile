CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
LDFLAGS_DYNAMIC = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

CXXFLAGS_DEBUG = -std=c++20 -Wall -Wextra -g -O0 -DDEBUG
OBJECTS_DEBUG = $(addprefix build/debug/, $(SOURCES:.cpp=.o))

SOURCES = main.cpp AssetManager.cpp Character.cpp CharacterManager.cpp CollisionManager.cpp Player.cpp PlayerStateNodes.cpp StateMachine.cpp StateNode.cpp
HEADERS = Animation.h AssetManager.h Atlas.h Camera.h Character.h CharacterManager.h CollisionBox.h CollisionLayer.h CollisionManager.h Player.h PlayerStateNodes.h StateMachine.h StateNode.h Timer.h Vector2.h Utils.h
OBJECTS = $(addprefix build/, $(SOURCES:.cpp=.o))
EXECUTABLE = hollowZero

COMPILE_COMMANDS = compile_commands.json

all: build/$(EXECUTABLE)

# Dynamic linking (for development)
build/$(EXECUTABLE): $(OBJECTS) | build
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS_DYNAMIC)

build/%.o: %.cpp $(HEADERS) | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

# Run the executable
run: build/$(EXECUTABLE)
	@echo "Running $(EXECUTABLE)..."
	@cd . && ./build/$(EXECUTABLE)

# Debug build

# Add these targets
build/debug/$(EXECUTABLE): $(OBJECTS_DEBUG) | build/debug
	$(CXX) $(OBJECTS_DEBUG) -o $@ $(LDFLAGS_DYNAMIC)

build/debug/%.o: %.cpp $(HEADERS) | build/debug
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@

build/debug:
	mkdir -p build/debug

debug: build/debug/$(EXECUTABLE)
	@echo "Starting debug version with gdb..."
	@cd . && gdb ./build/debug/$(EXECUTABLE)

# Add a debug run target
run-debug: build/debug/$(EXECUTABLE)
	@echo "Running debug version..."
	@cd . && ./build/debug/$(EXECUTABLE)

# Run with valgrind for memory checking
valgrind: build/$(EXECUTABLE)
	@echo "Running $(EXECUTABLE) with valgrind..."
	@cd . && valgrind --leak-check=full --show-leak-kinds=all ./build/$(EXECUTABLE)

# Generate compile_commands.json for clangd
$(COMPILE_COMMANDS): $(SOURCES) $(HEADERS)
	@echo "Generating compile_commands.json for clangd..."
	@echo '[' > $@
	@for src in $(SOURCES); do \
		echo '  {' >> $@; \
		echo '    "directory": "$(shell pwd)",' >> $@; \
		echo "    \"command\": \"$(CXX) $(CXXFLAGS) -c $$src\"," >> $@; \
		echo "    \"file\": \"$$src\"" >> $@; \
		if [ "$src" != "$(lastword $(SOURCES))" ]; then \
			echo '  },' >> $@; \
		else \
			echo '  }' >> $@; \
		fi; \
	done
	@echo ']' >> $@
	@echo "compile_commands.json generated successfully!"

# Setup clangd support
clangd: $(COMPILE_COMMANDS)

clean:
	rm -rf build/ dist/
	rm -f $(OBJECTS) build/$(EXECUTABLE) build/$(EXECUTABLE)_static build/$(EXECUTABLE)_portable

.PHONY: all clean run debug valgrind clangd
