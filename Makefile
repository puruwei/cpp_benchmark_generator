# Váriavel opcional contendo o diretório alvo da compilação do módulo
OUT_DIR ?= ./a.out.js

clear:
	@rm a.out.js
	@rm a.out.wasm

build:
	@emcc benchmarks.cpp -o $(OUT_DIR) \
-s MODULARIZE=1 \
-s EXPORT_ES6=1 \
-s EXPORT_NAME="BenchmarkModule" \
--bind \
-std=c++17 \
-sENVIRONMENT=web \
-sALLOW_MEMORY_GROWTH=1 \
-sASSERTIONS=1 \
-sSAFE_HEAP=1