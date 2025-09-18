# 情報工学実験B -- LLVMを用いた簡単なコンパイラの作成
```
.
├── dist
│   ├── coi-serviceworker.js
│   ├── index.html
│   ├── picoc.html
│   ├── picoc.wasm
│   └── server.py
├── example
│   ├── array
│   │   ├── min_max.pc
│   │   ├── sieve.pc
│   │   └── sort.pc
│   ├── c
│   │   ├── bench.c
│   │   ├── binary.c
│   │   ├── div.c
│   │   ├── donut.c
│   │   ├── fib.c
│   │   ├── gcd.c
│   │   └── prime.c
│   ├── donut
│   │   └── donut.pc
│   ├── kadai3
│   │   ├── binary_kadai3.pc
│   │   └── prime_kadai3.pc
│   ├── bench.pc
│   ├── binary.pc
│   ├── fib.pc
│   ├── gcd.pc
│   ├── loop.pc
│   └── prime.pc
├── misc
│   ├── cfg.sh
│   ├── codediff.sh
│   ├── get_imgcat.sh
│   ├── get_rr.sh
│   ├── grammar.txt
│   └── pc_to_c.sh
├── tidwall
│   ├── example.c
│   ├── LICENSE
│   ├── README.md
│   ├── tidwall_hashmap.c
│   └── tidwall_hashmap.h
├── hashmap.c
├── hashmap.h
├── LICENSE
├── Makefile
├── picoc.c
├── README.md
├── scan.c
└── scan.h

9 directories, 43 files
```

- `tidwall`ディレクトリ以下は[tidwall氏の標準Cによるハッシュ表の実装](https://github.com/tidwall/hashmap.c)でMITライセンスです
- gzuidhof氏の[`coi-serviceworker.js`](https://github.com/gzuidhof/coi-serviceworker)はMITライセンスです

