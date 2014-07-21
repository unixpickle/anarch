ARCH_FLAG=-D__ANARCH_TARGET_X64__

CXXFLAGS=-Wall -Wextra -std=c++11 -nostdinc $(ARCH_FLAG)
CCFLAGS=-Wall -Wextra -nostdinc $(ARCH_FLAG)

TEST_INCLUDES=-I../../include -I../dependencies/ansa/include -I../../src/stdlib/h -I../../src/stdlib/hpp -I../stdlib-api/include
TEST_SOURCES=../stdlib-api/build/*.o build/util/*.o build/dummy-api/*.o build/ansa/*.o

all: build/page-table

build/page-table: build/dummy-api build/inverse-memory build/ansa build/util ../stdlib-api/build
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) $(TEST_SOURCES) build/inverse-memory/*.o ../../src/arch/api/allocator.cpp ../../src/arch/x64/vmm/page-table.cpp test-page-table.cpp -o build/page-table

build/util: build
	mkdir build/util
	../build-objects $(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -c ../../src/util/*.cpp -o build/util

build/inverse-memory: build
	mkdir build/inverse-memory
	../build-objects $(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -c inverse-memory/*.cpp -o build/inverse-memory

build/dummy-api: build
	mkdir build/dummy-api
	../build-objects $(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -c dummy-api/*.cpp -o build/dummy-api

build/ansa: ../dependencies build
	mkdir build/ansa
	../build-objects $(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -c ../dependencies/ansa/src/*.cpp -o build/ansa
	../build-objects $(CC) $(CCFLAGS) $(TEST_INCLUDES) -c ../dependencies/ansa/dependencies/anlock/src/*.c -o build/ansa

../stdlib-api/build:
	$(MAKE) -C ../stdlib-api

../dependencies:
	mkdir ../dependencies
	git clone https://github.com/unixpickle/ansa.git ../dependencies/ansa

build:
	mkdir build

clean:
	rm -rf build