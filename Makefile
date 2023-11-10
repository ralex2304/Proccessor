
DOCS_DIR = docs


.PHONY: assemble execute disassemble run build_asm build_disasm build_spu

build: build_asm build_disasm build_spu

run: assemble execute

assemble:
	@echo // Make: Assembling
	@cd assembler && ./main -i ../Programs/$(prog)/main.code -o ../Programs/$(prog)/main.exec $(if $(lst), -l ../Programs/$(prog)/main.list)

execute:
	@echo // Make: Running
	@cd spu && ./main -i ../Programs/$(prog)/main.exec

disassemble:
	@echo // Make: Disassembling
	@cd disassembler && ./main -i ../Programs/$(prog)/main.exec -o ../Programs/$(prog)/main.disasm -d

build_asm:
	@cd ./assembler && make

build_disasm:
	@cd ./disassembler && make

build_spu:
	@cd ./spu && make


.PHONY: encode

encode:
	@python3 Programs/encoder.py Programs/$(prog)/$(prog).mp4 Programs/$(prog)/main.code

.PHONY: doxygen dox

DOCS_TARGET = $(DOCS_DIR)/docs_generated

doxygen dox: | $(DOCS_DIR)
	@doxygen docs/Doxyfile

$(DOCS_DIR):
	@mkdir ./$@

clean:
	@rm -rf ./$(DOCS_TARGET)
