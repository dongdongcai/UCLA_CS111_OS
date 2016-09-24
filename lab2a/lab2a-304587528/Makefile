CC = gcc
DISTDIR = lab2a-304587528
submission_files = lab2a.c Makefile lab2afirst.png lab2asecond.png README

lab2a:lab2a.c
	@$(CC) lab2a.c -pthread -lrt -Wall -o $@

dist: lab2a-304587528.tar.gz

lab2a-304587528.tar.gz: $(submission_files)
		@tar cvzf $@ $(submission_files)
clean:
		@rm -f *.o *.tar.gz lab2a