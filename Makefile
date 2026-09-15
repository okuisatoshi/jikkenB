CC := clang
TARGET := $(shell llvm-config --host-target)
# TARGET := x86_64-linux-gnu
# TARGET := aarch64-linux-gnu
# TARGET := wasm32-wasi
CFLAGS := -Wall -std=c11 -g3 $(CFLAGS) -O0 -target $(TARGET) -static
SHELL  := /bin/bash

# MacOS26のApple Container (container)は全く問題なく使用できる
# Windows11のWSL Containers (wslc.exe, 別名container.exe)はPowerShellからの使用なら問題ない
# (WSL2からの利用は現状，遅すぎて使えない)
# Windows11用のgnu makeのインストール: winget install ezwinports.make
CONTAINER := container

# Dev Containersの利用
# vscodeや以下のdevcontainers/cliを用いる
# https://github.com/devcontainers/cli
# curl -fsSL https://raw.githubusercontent.com/devcontainers/cli/main/scripts/install.sh | sh
DEVCONTAINER := docker
# DEVCONTAINER := podman

picoc: scan.o hashmap.o tidwall/tidwall_hashmap.o picoc.o
	$(CC) $(CFLAGS) $^ -o $@

testscan: scan.o hashmap.o tidwall/tidwall_hashmap.o testscan.o
	$(CC) $(CFLAGS) $^ -o $@

scan.o picoc.o: scan.h

hashmap.o picoc.o: hashmap.h

tidwall_hashmap.o: tidwall/tidwall_hashmap.h

test: picoc
	diff <(echo "966") <(./picoc < example/binary.pc | lli) && echo "OK" || echo "NG"

wasmer:
	curl https://get.wasmer.io -sSfL | sh

asciinema:
	curl -o asciinema -L https://github.com/asciinema/asciinema/releases/download/v3.2.1/asciinema-x86_64-unknown-linux-musl
	chmod a+x asciinema

build-container:
	$(CONTAINER) build -f .devcontainer/Dockerfile -t picoc .

run-container:
	$(CONTAINER) run --rm -it --name picoc -v $(CURDIR):/workspace picoc /bin/bash

devcontainer-up:
	devcontainer up --docker-path $(DEVCONTAINER)

devcontainer-shell:
	devcontainer exec --docker-path $(DEVCONTAINER) /bin/bash

smolvm-create:
	smolvm machine create --name picoc -s Smolfile

smolvm-shell:
	smolvm machine exec -ti --user $(shell id -u):$(shell id -g) --name picoc /bin/bash
clean:
	rm -rf *.o tidwall/*.o *~ picoc testscan a.out

.PHONY: test wasmer asciinema zig build-container run-container devcontainer-up devcontainer-shell smolvm-create smolvm-shell clean

