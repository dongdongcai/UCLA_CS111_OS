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
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <mcrypt.h>


//int globalreturncode = 0;
pthread_t *thread_id1;
pthread_t *thread_id2;
int to_child_pipe[2], from_child_pipe[2];
pid_t child_pid;
int encryptflag = 0;
MCRYPT td;
char *key;
char *IV; 
void handler(int sig) {
    // globalreturncode = 2;
    // //pthread_cancel(*thread_id1);
    // //pthread_cancel(*thread_id2);
    // close(to_child_pipe[1]);
    // close(from_child_pipe[0]);
    close(0);
    close(1);
    close(2);
    kill(child_pid,SIGKILL);
    exit(2);
}
void error(char *msg)
{
    perror(msg);
    exit(1);
}
void *thread_client(void *arg) {
    char buffer[2048];
    memset(buffer, 0, 2048);
    while(1) {
        int readsize = read(0, buffer, 2048);
        if(readsize <= 0) {
            //printf("6666666\n", );
            close(0);
            close(1);
            close(2);
            kill(child_pid,SIGKILL);
            //globalreturncode = 1;
            exit(1);
        }
        int i;
        for(i = 0; i < readsize; i++) {
            //int writesize = write(to_child_pipe[1], buffer, readsize);
            //if(writesize < 0) fprintf(stderr, "write() to shell failed!\n");
            char c = buffer[i];
            if(encryptflag == 1) mdecrypt_generic (td, &c, 1);
            if(c == 0x04) {
                close(0);
                close(1);
                close(2);
                kill(child_pid,SIGKILL);
                //close(to_child_pipe[1]);
                //close(from_child_pipe[0]);
                //globalreturncode = 1;
                exit(1);                    
            } else {
                write(to_child_pipe[1], &c, 1);
            }
        }
    }
    return 0;
}
void *thread_shell(void *arg) {
    char buffer[2048];
    memset(buffer, 0, 2048);
    while(1) {
        int readsize = read(from_child_pipe[0], buffer, 2048);
        if(readsize < 0) fprintf(stderr, "read() from shell failed!\n");
        else if(readsize == 0) {
            close(0);
            close(1);
            close(2);
            kill(child_pid,SIGKILL);
            //globalreturncode = 2;
            exit(2);

        }
        int writesize = 0;
        if(readsize > 0) {
            if(encryptflag == 1) mcrypt_generic (td, buffer, readsize);
            writesize = write(1, buffer, readsize);
        }
        if(writesize < 0) fprintf(stderr, "write() to socket failed!\n");
        // int i;
        // for(i = 0; i < readsize; i++) {
        //     char c = buffer[i];
        //     if(encryptflag == 1) mcrypt_generic (td, &c, 1);
        //     write(1, &c, 1);    
        // }
    }
    return 0;
}
int main(int argc, char *argv[]) {
    int status;
    int sockfd, newsockfd, portno = 1888;    
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    //int n;    
    int opt;
    int option_index;  
    static struct option long_options[] = {
        {"port", required_argument, NULL, 'p'},
        {"encrypt", no_argument, NULL, 'e'},
        {0, 0, 0, 0}
    };
    signal(SIGPIPE, handler);
    while((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        switch(opt) {
            case 'p' : {
                portno = atoi(optarg);
                break;
            }
            case 'e' : {
                encryptflag = 1;
                td = mcrypt_module_open("twofish", NULL, "cfb", NULL);
                if(td == MCRYPT_FAILED) {
                    fprintf(stderr, "mcrypt_module_open() failed!\n");
                    exit(1);
                }
                if((IV = malloc(mcrypt_enc_get_iv_size(td))) < 0) {
                    fprintf(stderr, "malloc() failed!\n");
                    exit(1);
                }
                int i;
                for(i = 0; i < mcrypt_enc_get_iv_size(td); i++) {
                    IV[i] = rand();
                } 
                int tempfd = open("my.key", O_RDONLY);
                if(tempfd < 0) {
                    fprintf(stderr, "can not open my.key!\n");
                    exit(1);
                }
                char key[16];
                read(tempfd, key, 16);
                close(tempfd);
                i = mcrypt_generic_init(td, key, 16, IV);
                if(i < 0) {
                    fprintf(stderr, "mcrypt_generic_init() failed!\n");
                    exit(1);
                }
                break;
            }
            default : {
                perror("Argument error");
            }
        }
    }    
/*    if (argc < 2) {
     fprintf(stderr,"ERROR, no port provided\n");
     exit(1);
    }*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    error("ERROR opening socket");
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    //portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
          sizeof(serv_addr)) < 0) 
          error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
      error("ERROR on accept");
    if(pipe(to_child_pipe) == -1) {
        fprintf(stderr, "pipe() failed!\n");
        exit(1);
    }
    if(pipe(from_child_pipe) == -1) {
        fprintf(stderr, "pipe() failed!\n");
    }
    child_pid = fork();
    //bzero(buffer,256);
    if(child_pid > 0) {
        close(to_child_pipe[0]);
        close(from_child_pipe[1]);
        dup2(newsockfd, STDIN_FILENO);
        dup2(newsockfd, STDOUT_FILENO);
        dup2(newsockfd, STDERR_FILENO);
        close(newsockfd);
        thread_id1 = calloc(1, sizeof(pthread_t));
        thread_id2 = calloc(1, sizeof(pthread_t));
        if(pthread_create(thread_id1, NULL, thread_shell, NULL) != 0) {
            fprintf(stderr, "pthread_create() failed!\n");
            exit(1);            
        }
        if(pthread_create(thread_id2, NULL, thread_client, NULL) != 0) {
            fprintf(stderr, "pthread_create() failed!\n");
            exit(1);            
        }
        int childreturncode = waitpid(child_pid, &status, WUNTRACED);
        // if(WIFSIGNALED(status) == 0)
        //     fprintf(stdout, "The exit status of child process is %d\n", WEXITSTATUS(status));
        // else
        //     fprintf(stdout, "Child process terminated by signal, signal#%d\n", status);
        // n = read(0,buffer,255);
        // if (n < 0) error("ERROR reading from socket");
        // printf("Here is the message: %s\n",buffer);
        // n = write(1,"I got your message",18);
        // if (n < 0) error("ERROR writing to socket");
    } else if(child_pid == 0) {
        close(to_child_pipe[1]);
        close(from_child_pipe[0]);
        dup2(to_child_pipe[0], STDIN_FILENO);
        dup2(from_child_pipe[1], STDOUT_FILENO);
        dup2(from_child_pipe[1], STDERR_FILENO);
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
    return 0; 
}