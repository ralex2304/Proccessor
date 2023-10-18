
DOCS_DIR = docs


.PHONY: assemble execute disassemble run

run: assemble execute

assemble:
	@echo // Make: Assembling
	@cd assembler && ./main -i ../Programs/$(prog)/main.code -o ../Programs/$(prog)/main.exec -l ../Programs/$(prog)/main.list

execute:
	@echo // Make: Running
	@cd spu && ./main -i ../Programs/$(prog)/main.exec

disassemble:
	@echo // Make: Disassembling
	@cd disassembler && ./main -i ../Programs/$(prog)/main.exec -o ../Programs/$(prog)/main.disasm -d

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
