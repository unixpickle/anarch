CPPFLAGS=-Wall -Wextra -std=c++11

all: build/assert

build/assert: dependencies build/ansa
	$(CXX) $(CPPFLAGS) -I../../include -Idependencies/ansa/include test-assert.cpp ../../src/util/*.cpp dummy-api/*.cpp build/ansa/*.o -o build/assert

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
