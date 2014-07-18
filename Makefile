TARGET_ARCH ?= x64
PROJECT_ROOT=$(shell pwd)

export TARGET_ARCH
export PROJECT_ROOT

CC ?= gcc
CXX ?= g++
ASM ?= nasm
override CXXFLAGS += -nostdlib -nostdinc -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -Wno-long-long -Wall -Wextra -std=c++11 -mno-sse -mno-mmx
override CFLAGS += -nostdlib -nostdinc -ffreestanding -mno-red-zone -Wno-long-long -Wall -Wextra -mno-sse -mno-mmx
override ASMFLAGS += -f elf64

export CC
export CXX
export ASM
export CXXFLAGS
export CFLAGS
export ASMFLAGS

build/anarch.a: build/objects
	make -C link/$(TARGET_ARCH)

build/objects: build dependencies
	mkdir build/objects
	./dependencies/makemaker/bin/makemaker build.coffee
	make -C build/objects

dependencies:
	mkdir dependencies
	git clone https://github.com/unixpickle/ansa.git dependencies/ansa
	git clone https://github.com/unixpickle/analloc2.git dependencies/analloc2
	git clone https://github.com/unixpickle/makemaker.git dependencies/makemaker

build:
	mkdir build

clean:
	rm -rf build

clean-all:
	rm -rf build
	rm -rf dependencies
