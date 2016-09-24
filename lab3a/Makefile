CC = gcc
DISTDIR = lab3a-304587528
submission_files = lab3a.c Makefile README

lab3a:lab3a.c
	@$(CC) lab3a.c -Wall -o $@

dist: lab3a-304587528.tar.gz

lab3a-304587528.tar.gz: $(submission_files)
		@tar cvzf $@ $(submission_files)
clean:
		@rm -f *.o *.tar.gz lab3a