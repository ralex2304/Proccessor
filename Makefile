
DOCS_DIR = docs


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

build: build_asm build_disasm build_spu

build_asm:
	@cd ./assembler && make

build_disasm:
	@cd ./disassembler && make

build_spu:
	@cd ./spu && make


.PHONY: doxygen dox

DOCS_TARGET = $(DOCS_DIR)/docs_generated

doxygen dox: $(DOCS_TARGET)

$(DOCS_TARGET): $(FILES:/%=%) | $(DOCS_DIR)
	@echo "Doxygen generated %date% %time%" > $(DOCS_TARGET)
	@doxygen.exe docs/Doxyfile

$(DOCS_DIR):
	@mkdir ./$@

clean:
	@rm -rf ./$(DOCS_TARGET)
