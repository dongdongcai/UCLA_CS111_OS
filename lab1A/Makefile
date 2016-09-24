CC = gcc
DISTDIR = lab1a-304587528
submission_files = main.c Makefile README

lab1a:main.c
	@$(CC) main.c -pthread -o $@

dist: lab1a-304587528.tar.gz

lab1a-304587528.tar.gz: $(submission_files)
		@tar cvzf $@ $(submission_files)
clean:
		@rm -f *.o *.tar.gz lab1a