#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/wait.h>

struct termios saved_attributes;
int flag = 0;
int globalreturncode = 0;
pthread_t *thread_id1;
pthread_t *thread_id2;
int to_child_pipe[2], from_child_pipe[2];
struct thread_info
{
	int child_pid;
	int* to_child_pipe;
	int* from_child_pipe;
};
void handler(int sig) {
	//printf("gagagagagaga\n");
    globalreturncode = 1;
    pthread_cancel(*thread_id1);
    pthread_cancel(*thread_id2);
	close(to_child_pipe[1]);
	close(from_child_pipe[0]);
}
void handlerc(int sig) {
	return;
}
void reset_terminal (void) {
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}
void *thread_input(void *arg) {
	struct thread_info *tinfo = arg;
	int child_pid = tinfo->child_pid;
	int *to_child_pipe = tinfo->to_child_pipe;
	int *from_child_pipe = tinfo->from_child_pipe;
	char buffer[2048];
	while(1) {
		int readsize = read(0, buffer, 2048);
		int i = 0;
		for(; i < readsize; i++) {
			char c = buffer[i];
			if(c == 0x04) {
				if(flag == 1) kill(child_pid, SIGHUP);
				pthread_cancel(*thread_id2);
				close(to_child_pipe[1]);
				close(from_child_pipe[0]);
				return 0;
			} else if(c == 0x03) {
				if(flag == 1) {
					kill(child_pid, SIGINT);
					//pthread_cancel(*thread_id2);
				}
			} else if(c ==0x0D || c == 0x0A) {
				c = 0x0D;
				write(1, &c, 1);
				c = 0x0A;
				write(1, &c, 1);
				if(flag == 1) write(to_child_pipe[1], &c, 1);
			} else {
				write(1, &c, 1);
				if(flag == 1) write(to_child_pipe[1], &c, 1);
			}
		}
	}
	return 0;
}
void *thread_output(void *arg) {
	struct thread_info *tinfo = arg;
	int child_pid = tinfo->child_pid;
	int *to_child_pipe = tinfo->to_child_pipe;
	int *from_child_pipe = tinfo->from_child_pipe;
	char buffer[2048];
	while(1) {
		int readsize = read(from_child_pipe[0], buffer, 2048);
		int i = 0;
		if(readsize == 0 && flag == 1) {
			//printf("hahaha\n");
			pthread_cancel(*thread_id1);
			globalreturncode = 1;
			return 0;			
		}
		for(; i < readsize; i++) {
			char c = buffer[i];
			if(c == 0x04) {
				pthread_cancel(*thread_id1);
				globalreturncode = 1;
				return 0; 
			} else if(c ==0x0D || c == 0x0A) {
				c = 0x0D;
				write(1, &c, 1);
				c = 0x0A;
				write(1, &c, 1);
			} else {
				write(1, &c, 1);
			}
		}
	}
	return 0;
}
int main(int argc, char** argv) {
	int status;
	//int to_child_pipe[2], from_child_pipe[2];
	if(pipe(to_child_pipe) == -1) {
		fprintf(stderr, "pipe() failed!\n");
		exit(1);
	}
	if(pipe(from_child_pipe) == -1) {
		fprintf(stderr, "pipe() failed!\n");
	}
	struct termios tattr;
	tcgetattr(STDIN_FILENO, &saved_attributes);
	atexit(reset_terminal);
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON | ECHO);
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
	char c;
	pid_t pid = 1;
	static struct option long_options[] = {
		{"shell", no_argument, NULL, 's'},
		{0, 0, 0, 0}
	};
	int option_index;
	int opt = getopt_long(argc, argv, "", long_options, &option_index);
	if(opt == 's') {
		pid = -1;
		pid = fork();
		flag = 1;
	}
	if(pid > 0) {
		//printf("My process ID : %d\n", getpid());
		close(to_child_pipe[0]);
		close(from_child_pipe[1]);
		signal(SIGPIPE, handler);
		//signal(SIGINT, handlerc);
		struct thread_info *tinfo;
		tinfo = calloc(1, sizeof(struct thread_info));
		tinfo->child_pid = pid;
		tinfo->to_child_pipe = to_child_pipe;
		tinfo->from_child_pipe = from_child_pipe;
		thread_id1 = calloc(1, sizeof(pthread_t));
		thread_id2 = calloc(1, sizeof(pthread_t));
		if(pthread_create(thread_id1, NULL, thread_input, tinfo) != 0) {
			fprintf(stderr, "pthread_create() failed!\n");
			exit(1);			
		}
		if(pthread_create(thread_id2, NULL, thread_output, tinfo) != 0) {
			fprintf(stderr, "pthread_create() failed!\n");
			exit(1);			
		}
		//if(flag == 0) {
		void *res1;
		void *res2;
		if(pthread_join(*thread_id1, &res1) != 0) {
			fprintf(stderr, "pthread_join() failed!\n");
		}
		//printf("1 end\n");
		if(pthread_join(*thread_id2, &res2) != 0) {
			fprintf(stderr, "pthread_join() failed!\n");
		}
		//printf("2 end\n"); 
		//}
		int childreturncode = waitpid(pid, &status, WUNTRACED);
		if(flag == 1) {
			if(WIFSIGNALED(status) == 0)
				fprintf(stdout, "The exit status of child process is %d\n", WEXITSTATUS(status));
			else
				fprintf(stdout, "Child process terminated by signal, signal#%d\n", status);
		}		
	} else if(pid == 0){
		close(to_child_pipe[1]);
		close(from_child_pipe[0]);
		dup2(to_child_pipe[0], STDIN_FILENO);
		dup2(from_child_pipe[1], STDOUT_FILENO);
		close(to_child_pipe[0]);
		close(from_child_pipe[1]);
		char *execvp_argv[2];
		char execvp_filename[] = "/bin/bash";
		execvp_argv[0] = execvp_filename;
		execvp_argv[1] = NULL;
		if(execvp(execvp_filename, execvp_argv) == -1) {
			fprintf(stderr, "execvp() failed!\n");
			exit(1);
		}
	} else {
		fprintf(stderr, "fork() failed!\n");
		exit(1);
	}
	//printf("%d\n", globalreturncode);
	return globalreturncode;
}