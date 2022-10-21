.PHONY: all clean
all: ../../data/font8x8.bin

clean:
	rm -fv build.bin ../../data/font8x8.bin

.DEFAULT_GOAL := all

../../data/font8x8.bin: build.bin
	./build.bin $@

build.bin: build.cpp
	g++ -I. -std=c++17 -o $@ $<

