CXXFLAGS=-std=c++11 -Wall -Wextra
INCLUDES=-Iinclude

all: build
	../build-objects $(CXX) $(INCLUDES) $(CXXFLAGS) -c src/*.cpp -o build

build:
	mkdir build

clean:
	rm -rf build
