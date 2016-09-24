CC = gcc
DISTDIR = lab1b-304587528
submission_files = server.c client.c my.key Makefile README

lab1b:server client

server:server.c	
	@$(CC) server.c -pthread -l mcrypt -o $@
client:client.c
	@$(CC) client.c -pthread -l mcrypt -o $@

dist: lab1b-304587528.tar.gz

lab1b-304587528.tar.gz: $(submission_files)
		@tar cvzf $@ $(submission_files)
clean:
		@rm -f *.o *.tar.gz server client *.txt