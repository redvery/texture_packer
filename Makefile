bin/texture_packer: src/texture_packer.c | bin
	@echo Compiling texture_packer.c...
	@gcc -o $@ $< 

bin:
	@echo Creating bin directory...
	@mkdir -p bin

clean:
	@echo Cleaning up...
	@rm -rf bin