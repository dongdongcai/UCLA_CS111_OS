CC = gcc
DISTDIR = lab2b-304587528
submission_files = lab2b.c SortedList.h SortedList.c Makefile lab2bfirst.png lab2bsecond.png README

lab2b:lab2b.c SortedList.h SortedList.c
	@$(CC) lab2b.c SortedList.c -pthread -lrt -Wall -o $@

dist: lab2b-304587528.tar.gz

lab2b-304587528.tar.gz: $(submission_files)
		@tar cvzf $@ $(submission_files)
clean:
		@rm -f *.o *.tar.gz lab2b