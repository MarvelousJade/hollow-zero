CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
LDFLAGS_DYNAMIC = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

SOURCES = main.cpp AssetManager.cpp
HEADERS = Animation.h AssetManager.h Atlas.h Camera.h Timer.h Vector2.h
OBJECTS = $(addprefix build/, $(SOURCES:.cpp=.o))
EXECUTABLE = hollowZero

all: build/$(EXECUTABLE)

# Dynamic linking (for development)
build/$(EXECUTABLE): $(OBJECTS) | build
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS_DYNAMIC)

build/%.o: %.cpp $(HEADERS) | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build/ dist/
	rm -f $(OBJECTS) build/$(EXECUTABLE) build/$(EXECUTABLE)_static build/$(EXECUTABLE)_portable

.PHONY: all clean static portable dist dist-bundle zip
