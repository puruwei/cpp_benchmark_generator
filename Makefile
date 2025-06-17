# Váriavel opcional contendo o diretório alvo da compilação do módulo
OUT_DIR ?= ./

clear:
	@echo "$$(basename $$(pwd))"
	@if [ -d "$$(basename $$(pwd))" ]; then \
		echo "$(basename $(pwd))"; \
		rm -rf "$$(basename $$(pwd))"; \
	else \
		echo "nada pra limpar aqui"; \
	fi

build:
	@mkdir "$(OUT_DIR)/$$(basename $$(pwd))/"
	@emcc benchmarks.cpp -o "$(OUT_DIR)/$$(basename $$(pwd))/a.out.js" \
-s MODULARIZE=1 \
-s EXPORT_ES6=1 \
-s EXPORT_NAME="BenchmarkModule" \
--bind \
-std=c++17 \
-sENVIRONMENT=web \
-sALLOW_MEMORY_GROWTH=1 \
-sASSERTIONS=1 \
-sSAFE_HEAP=1