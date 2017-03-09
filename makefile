.PHONY: release
release:
	cd src/asm; make release
	cd src/asm-check; make release
	cd src/asm-syntax; make release

.PHONY: debug
debug:
	cd src/asm; make debug
	cd src/asm-check; make debug
	cd src/asm-syntax; make debug

.PHONY: clean
clean:
	cd src/asm; make clean
	cd src/asm-check; make clean
	cd src/asm-syntax; make clean
