CPPFLAGS=-Wall -Wextra -std=c++11
TEST_INCLUDES=-I../../include -I../dependencies/ansa/include -I../stdlib-api/include
TEST_SOURCES=../../src/util/*.cpp ../stdlib-api/build/*.o dummy-api/*.cpp build/ansa/*.o
ARCH_FLAG ?= -D__ANARCH_TARGET_X64__

all: build/assert build/stream build/critical

build/assert: ../dependencies build/ansa ../stdlib-api/build
	$(CXX) $(ARCH_FLAG) $(CPPFLAGS) $(TEST_INCLUDES) $(TEST_SOURCES) test-assert.cpp -o build/assert

build/stream: ../dependencies build/ansa ../stdlib-api/build
	$(CXX) $(ARCH_FLAG) $(CPPFLAGS) $(TEST_INCLUDES) $(TEST_SOURCES) test-stream.cpp -o build/stream

build/critical: ../dependencies build/ansa ../stdlib-api/build
	$(CXX) $(ARCH_FLAG) $(CPPFLAGS) $(TEST_INCLUDES) $(TEST_SOURCES) test-critical.cpp -o build/critical

build/ansa: ../dependencies build
	mkdir build/ansa
	../build-objects $(CXX) $(CPPFLAGS) -I../dependencies/ansa/include -c ../dependencies/ansa/src/*.cpp -o build/ansa

../stdlib-api/build:
	$(MAKE) -C ../stdlib-api

../dependencies:
	mkdir ../dependencies
	git clone https://github.com/unixpickle/ansa.git ../dependencies/ansa;

build:
	mkdir build

clean:
	rm -rf build

clean-all:
	rm -rf build
	rm -rf dependencies
