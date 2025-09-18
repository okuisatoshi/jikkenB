CC := gcc
# CC := clang
# CC := ./zig/zig cc
CFLAGS := -Wall -std=c11 -O0 -g3 $(CFLAGS)
SHELL := /bin/bash

picoc: scan.o hashmap.o tidwall/tidwall_hashmap.o picoc.o
	$(CC) $(CFLAGS) $^ -o $@

scan.o picoc.o: scan.h

hashmap.o picoc.o: hashmap.h

tidwall_hashmap.o: tidwall/tidwall_hashmap.h

test: picoc
	diff <(echo "966") <(./picoc < example/binary.pc | lli) && echo "OK" || echo "NG"

picoc.wasm:
	make clean
	make CFLAGS='-std=c11 -Os --target=wasm32-wasi' picoc
	mv picoc dist/picoc.wasm

debug:
	make clean
	CFLAGS=-DDEBUG make picoc

wasmer:
	curl https://get.wasmer.io -sSfL | sh

zig:
	mkdir -p zig
	curl -qL https://ziglang.org/download/0.15.1/zig-x86_64-linux-0.15.1.tar.xz | tar Jx -C zig --strip-components 1

clean:
	rm -rf *.o tidwall/*.o *~ picoc a.out

.PHONY: test debug wasmer zig clean


