SCRIPT = lab3b.py
DISTDIR = lab3b-304587528
submission_files = lab3b.py Makefile README

run:lab3b.py
	python $(SCRIPT)

dist: lab3b-304587528.tar.gz

lab3b-304587528.tar.gz: $(submission_files)
		@tar cvzf $@ $(submission_files)
clean:
		@rm -f *.o *.tar.gz *.txt