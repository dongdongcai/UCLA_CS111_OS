# INFORMATION
My name : Dongdong Cai
My ID# : 304587528

# INCLUDED FILES
-- lab2a.c: a single C source modules 
-- Makefile: to build the program and the tarball
	make: build executable file
	make dist: build this tarball
	make clean: delete all makefile-created files to return the directory to its just installed state.
-- two graphs:
	lab2afirst.png: the average time per operation vs the number of iterations
	lab2asecond.png: the average time per operation vs the number of threads for all four versions of the add function
-- this README file

# QUESTIONS
Q1A: Why does it take this many threads or iterations to result in failure?
A: If the number of threads is low, the probability of add() function of one threads being interrpted by other threads is low as well. For example, if there is only one thread, the add() function would not be interrput and the program would not report any error. When the number of threads increase the probablity would increase as well, so the program would be more likely to report an error.

Q1B: Why does a significantly smaller number of iterations so seldom fail?
A: The answer for this question is quite similar as 1A. When the number of threads is low, the probability of add() function of one threads being interrpted by other threads is low as well. For example, if there is only one thread, the add() function would not be interrput and the program would not report any error. When the number of threads increase the probablity would increase as well, so the program would be more likely to report an error.

Q2A: Why does the average cost per operation drop with increasing iterations?
A: Since we start the timer before the creation of threads and stop the timer after all threads end, the overhead is included in the total run time we get and the overhead is proportional to the number of threads. When the number of iterations is low, the run time is dominated by overhead, so the average cost is relative high. However, if we increase the number of iterations, the time spend in add() function would become dominant in total run time since the number of threads remained the same thus the overhead remained the same as well. As a result, the average cost would decrease with increasing number of iterations.

Q2B: How do we know what the “correct” cost is?
A: As I mentioned in the previous 2A question, if we increase the number of iterations, the time spend in add() function would become dominant in total run time. If we increase the number of iterations to infinity, the overhead would become negligible. So, in order to get "correct" cost, we should increase the number of iterations to make it big enough for use to neglect the overhead. 

Q2C: Why are the --yield runs so much slower?  Where is the extra time going?
A:  Because the thread yielded will give up the CPU to another thread, be put into the end of run queue. It must wait for another thread to finish before running again. The extra time is spent for extra context switching and scheduling.

Q2D: Can we get valid timings if we are using --yield?  How, or why not?
A: No, we can not get valid time since as I metioned in 2A and 2C, we can not get correct time because context switching an other overhead. Worse still, the --yield option would increase time spent in context switching, which would make our result even more inaccurate.

Q3A: Why do all of the options perform similarly for low numbers of threads?
A: This question is quite similar as question 1A and question 1B, the low number of threads make the probability of competition low as well. So, the time wasted in waiting for critical section becoming avalialbe is less too. As a result, all of the options perform similarly for low number of threads.

Q3B: Why do the three protected operations slow down as the number of threads rises?
A: Firstly, the increase number of threads would lead to more context switch, which would increase the average cost of operations. Secondly, the critical section would lead to some time being wasted while threads waiting for locks to be released.

Q3C: Why are spin-locks so expensive for large numbers of threads?
A: The threads waiting for lock to be released would still be in the run queue. So, when it is its trun to run, it would do nothing but keep asking "Is the lock released yet?" which is totally a waste of time. So, spin-locks are very expensive for large number of threads since when the number of threads is large, the time wasted in spining is large as well.
