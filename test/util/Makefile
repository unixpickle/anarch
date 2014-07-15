CPPFLAGS=-Wall -Wextra -std=c++11
TEST_INCLUDES=-I../../include -Idependencies/ansa/include
TEST_SOURCES=../../src/util/*.cpp dummy-api/*.cpp build/ansa/*.o

all: build/assert build/stream

build/assert: dependencies build/ansa
	$(CXX) $(CPPFLAGS) $(TEST_INCLUDES) $(TEST_SOURCES) test-assert.cpp -o build/assert

build/stream: dependencies build/ansa
	$(CXX) $(CPPFLAGS) $(TEST_INCLUDES) $(TEST_SOURCES) test-stream.cpp -o build/stream

build/ansa: dependencies build
	mkdir build/ansa
	cd build/ansa; $(CXX) $(CPPFLAGS) -I../../dependencies/ansa/include -c ../../dependencies/ansa/src/*.cpp; cd -
	cd build/ansa; $(CC) -I../../dependencies/ansa/include -c ../../dependencies/ansa/dependencies/anlock/src/*.c; cd -

dependencies:
	mkdir dependencies
	git clone https://github.com/unixpickle/ansa.git dependencies/ansa

build:
	mkdir build

clean:
	rm -rf build

clean-all:
	rm -rf build
	rm -rf dependencies
