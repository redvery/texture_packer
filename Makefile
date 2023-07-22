bin/texpack: src/texpack.cpp | bin
	@echo Compiling texpack.cpp...
	@g++ -o $@ $< -Wc++11-extensions -std=c++20

bin:
	@echo Creating bin directory...
	@mkdir -p bin

clean:
	@echo Cleaning up...
	@rm -rf bin