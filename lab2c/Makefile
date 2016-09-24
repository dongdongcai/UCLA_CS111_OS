CC = gcc
DISTDIR = lab2c-304587528
submission_files = lab2c.c SortedList.h SortedList.c Makefile lab2cfirst.png README

lab2c:lab2c.c SortedList.h SortedList.c
	@$(CC) lab2c.c SortedList.c -pthread -lrt -Wall -o $@

dist: lab2c-304587528.tar.gz

lab2c-304587528.tar.gz: $(submission_files)
		@tar cvzf $@ $(submission_files)
clean:
		@rm -f *.o *.tar.gz lab2c