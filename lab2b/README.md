# INFORMATION
My name : Dongdong Cai
My ID# : 304587528

# INCLUDED FILES
* SortedList.h: a header file containing interfaces for linked list operations
* SortedList.c: the source for a C source module that implements the interface in SortedList,h
* lab2b.c: the source for the C program required by the spec 
* Makefile: to build the program and the tarball
	make: build executable file
	make dist: build this tarball
	make clean: delete all makefile-created files to return the directory to its just installed state.
* two graphs:
	lab2bfirst.png: the time per operation vs the number of iterations (for --threads=1)
	lab2bsecond.png: the (corrected) per operation times (for each of the three synchronization options: unprotected, mutex, spin) vs the number of threads
* this README file

# QUESTIONS
Q1A: Explain the variation in time per operation vs the number of iterations?
A: When the number of iterations is low, the run time is dominated by overhead of the program such as the creation of threads, context switch and etc.. While the number of iterations increases, the run time become dominated by list operations, so the average cost per operation we get would be smaller and closer to the real value. However, the insert and lookup operations of the list involve searching through the entire list. The complexity would be O(n), so as the number of iterations further increases, the operation of inserting and lookup would become very expensive which lead to the increasing of average cost per operation. 

Q1B: How would you propose to correct for this effect?
A: Since the complexity of insert and lookup is O(n), when the number of iterations is large, we can simply divide the average cost per operation by the number of nodes we inserted. The result we get may be smaller than the real value but would be closer to the real value than our previous result.

Q2A: Compare the variation in time per protected operation vs the number of threads in Project 2B and in Project 2A.  Explain the difference.
A: The critical section of list operation is bigger than the that of add function, so the threads hold the lock for longer time in list than add function. As a result, the probablity of contention with same number of threads in lab2b is larger than lab2a which means more blocked threads. So, there would be more overhead and less parallelism.

# Note
1. I choose 1K iterations to plot the second graph because it is fast and can partialy eliminate the effect of overheads.
2. With --yield option, it takers fewer threads and iterations for the program to consisitently demonstrate the problem(final length != 0).
3. Run multi-threads program with unprotcted method would sometimes cause segfault. 
