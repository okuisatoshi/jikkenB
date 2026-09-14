# 情報工学実験B -- LLVMを用いた簡単なコンパイラの作成
```

.
├── cfg.sh
├── .clangd
├── .clang-format
├── .gitignore
├── hashmap.c
├── hashmap.h
├── LICENSE
├── Makefile
├── picoc.c
├── README.md
├── scan.c
├── scan.h
├── Smolfile
├── testscan.c
├── .devcontainer
│   ├── devcontainer.json
│   └── Dockerfile
├── dist
│   ├── coi-serviceworker.js
│   ├── index.html
│   ├── picoc.html
│   ├── picoc.wasm
│   └── server.py
├── example
│   ├── bench.pc
│   ├── binary.pc
│   ├── fib.pc
│   ├── gcd.pc
│   ├── loop.pc
│   ├── prime.pc
│   ├── c
│   │   ├── bench.c
│   │   ├── binary.c
│   │   ├── donut.c
│   │   ├── fib.c
│   │   ├── gcd.c
│   │   └── prime.c
│   ├── donut
│   │   └── donut.pc
│   ├── kadai3
│   │   ├── binary_kadai3.pc
│   │   ├── fizz1.pc
│   │   ├── fizz2.pc
│   │   └── prime_kadai3.pc
│   └── kadai4
│       ├── min_max.pc
│       ├── sieve.pc
│       └── sort.pc
└── tidwall
    ├── example.c
    ├── LICENSE
    ├── README.md
    ├── tidwall_hashmap.c
    └── tidwall_hashmap.h

9 directories, 46 files

cat example/prime.pc | ./picoc | ./misc/cfg.sh png >prime.png```
- `tidwall`ディレクトリ以下は[tidwall氏の標準Cによるハッシュ表の実装](https://github.com/tidwall/hashmap.c)でMITライセンスです
- gzuidhof氏の[`coi-serviceworker.js`](https://github.com/gzuidhof/coi-serviceworker)はMITライセンスです

