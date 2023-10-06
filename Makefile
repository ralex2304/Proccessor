.PHONY: assemble spu disassemble run

run: assemble spu

assemble:
	@echo // Make: Assembling
	@cd assembler && ./main

spu:
	@echo // Make: Running
	@cd spu && ./main

disassemble:
	@echo // Make: Disassembling
	@cd disassembler && ./main
