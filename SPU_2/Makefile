.PHONY: build_all build_stack clean_stack build_asm clean_asm build_spu clean_spu

build_all: build_stack

build_stack:
	cd stack_ded && make

clean_stack:
	cd stack_ded && make clean


build_asm:
	cd assemblers && make

clean_asm:
	cd assemblers && make clean


build_spu:
	cd spu_ded && make

clean_spu:
	cd spu_ded && make clean