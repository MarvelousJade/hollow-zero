CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
LDFLAGS_DYNAMIC = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

SOURCES = main.cpp AssetManager.cpp
HEADERS = Animation.h AssetManager.h Atlas.h Camera.h Timer.h Vector2.h
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

# Debug run with gdb
debug: build/$(EXECUTABLE)
	@echo "Starting $(EXECUTABLE) with gdb..."
	@cd . && gdb ./build/$(EXECUTABLE)

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

.PHONY: all clean static portable dist dist-bundle zip
