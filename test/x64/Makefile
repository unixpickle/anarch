ARCH_FLAG=-D__ANARCH_TARGET_X64__

CXXFLAGS=-Wall -Wextra -std=c++11 $(ARCH_FLAG)
CCFLAGS=-Wall -Wextra $(ARCH_FLAG)

CXX_STANDALONE=-nostdlib -nostdinc -I../../src/stdlib/h -I../../src/stdlib/hpp
CC_STANDALONE=-nostdlib -nostdinc -I../../src/stdlib/h

TEST_INCLUDES=-I../../include -I../dependencies/ansa/include
TEST_SOURCES=build/util/*.o build/dummy/*.o build/ansa/*.o build/assert.o

all: build/page-table

build/page-table: build/dummy build/ansa build/util build/assert.o
	$(CXX) $(CPPFLAGS) $(TEST_INCLUDES) $(TEST_SOURCES) test-page-table.cpp -o build/page-table

build/util: build
	mkdir build/util
	../build-objects $(CXX) $(CXXFLAGS) $(CXX_STANDALONE) $(TEST_INCLUDES) -c ../../src/util/*.cpp -o build/util

build/dummy: build
	mkdir build/dummy
	../build-objects $(CXX) $(CXXFLAGS) $(CXX_STANDALONE) $(TEST_INCLUDES) -c dummy-api/*.cpp -o build/dummy

build/ansa: ../dependencies build
	mkdir build/ansa
	../build-objects $(CXX) $(CXXFLAGS) $(CXX_STANDALONE) $(TEST_INCLUDES) -c ../dependencies/ansa/src/*.cpp -o build/ansa
	../build-objects $(CC) $(CCFLAGS) $(CC_STANDALONE) $(TEST_INCLUDES) -c ../dependencies/ansa/dependencies/anlock/src/*.c -o build/ansa

build/assert.o: build
	g++ -c ../assert.cpp -o build/assert.o

../dependencies:
	mkdir ../dependencies
	git clone https://github.com/unixpickle/ansa.git ../dependencies/ansa

build:
	mkdir build

clean:
	rm -rf build