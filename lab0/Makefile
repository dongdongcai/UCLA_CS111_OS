CC = gcc
DISTDIR = lab0-304587528
submission_files = main.c Makefile backtrace.png breakpoint.png README

lab0:main.c
	@$(CC) main.c -o $@

check:
	@echo "Running smoke test......";
	@$(CC) main.c -o test
	echo "hahahahahahha" > input.txt
	@echo "Copying......"
	./test --output=output.txt --input=input.txt
	@([ $$? -eq 0 ] && echo "Copy file success!") || (echo "Copy file fail"; exit 1)
	@echo "Checking if input and output files are the same......"
	cat output.txt
	diff input.txt output.txt
	@([ $$? -eq 0 ] && echo "Input and the output files are match!") || (echo "Input and the output files are not match"; exit 1)
	@echo "Checking invalid input file......"
	@./test --input=invalid.txt || ([ $$? -eq 1 ] && echo "Return code for invalid iutput is 1!") || (echo "Return code for invalid iutput is not 1"; exit 1)
	@echo "Checking invalid output file......"
	@echo "   " > invalidoutput.txt
	chmod 0444 invalidoutput.txt
	@./test --output=invalidoutput.txt || ([ $$? -eq 2 ] && echo "Return code for invalid output is 2!") || (echo "Return code for invalid output is not 2"; exit 1)
	@echo "Checking the return code for --segfault and catch......"
	@./test --segfault --catch || ([ $$? -eq 3 ] && echo "Return code is 3!") || (echo "Return code for invalid output is not 3"; exit 1)
	@echo "Smoke test complete."

dist: lab0-304587528.tar.gz

lab0-304587528.tar.gz: $(submission_files)
		@tar cvzf $@ $(submission_files)
clean:
		@rm -f *.o *.tar.gz lab0 test *.txt