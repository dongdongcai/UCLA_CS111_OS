#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
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

struct termios saved_attributes;
int sockfd;
pthread_t *thread_id1;
pthread_t *thread_id2;
//int globalreturncode = 0;
int logfd = -1;
int logflag = 0;
int encryptflag = 0;
MCRYPT td;
//char *key;
char *IV;
void error(char *msg)
{
    perror(msg);
    exit(0);
}
void *thread_input(void *arg) {
    char buffer[2048];
    memset(buffer, 0, 2048);
    while(1) {
        int readsize = read(0, buffer, 2048);
        int i = 0;
        if(logflag == 1 && readsize > 0) dprintf(logfd, "\nSENT %d bytes: ", readsize);
        for(; i < readsize; i++) {
            //if(i == 0 && logflag == 1) dprintf(logfd, "\n");
            char c = buffer[i];
            if(c == 0x04) {
                pthread_cancel(*thread_id2);
                close(sockfd);
                return 0;
                exit(0);
            } /*else if(c == 0x03) {
                if(flag == 1) {
                    kill(child_pid, SIGINT);
                    //pthread_cancel(*thread_id2);
                }
            }*/ else if(c ==0x0D || c == 0x0A) {
                c = 0x0D;
                write(1, &c, 1);
                c = 0x0A;
                write(1, &c, 1);
                if(encryptflag == 1) mcrypt_generic (td, &c, 1);
                if(logflag == 1) dprintf(logfd, "%c", c);
                write(sockfd, &c, 1);
            } else {
                write(1, &c, 1);
                if(encryptflag == 1) mcrypt_generic (td, &c, 1);
                if(logflag == 1) dprintf(logfd, "%c", c);
                write(sockfd, &c, 1);
            }
        }
    }
    return 0;
}
void *thread_output(void *arg) {
    char buffer[2048];
    memset(buffer, 0, 2048);
    while(1) {
        int readsize = read(sockfd, buffer, 2048);
        int i = 0;
        if(logflag == 1 && readsize > 0) {
            //printf("hahahahaha\n");
            dprintf(logfd, "\nRECEIVE %d bytes: ", readsize);
        }
        if(readsize <= 0) {
            //printf("hahaha\n");
            close(sockfd);
            //pthread_cancel(*thread_id1);
            //globalreturncode = 1;
            //return 0;
            exit(1);           
        }
        for(; i < readsize; i++) {
            char c = buffer[i];
            //if(i == 0 && logflag == 1) dprintf(logfd, "\n");
            if(logflag == 1) dprintf(logfd, "%c", c);
            if(encryptflag == 1) mdecrypt_generic (td, &c, 1);
            if(c == 0x04) {
                close(sockfd);
                //pthread_cancel(*thread_id1);
                //globalreturncode = 1;
                exit(1);
            } else if(c ==0x0D || c == 0x0A) {
                //if(logflag == 1 && encryptflag == 0) dprintf(logfd, "%c", c);
                c = 0x0D;
                write(1, &c, 1);
                c = 0x0A;
                write(1, &c, 1);
            } else {
                write(1, &c, 1);
                //if(logflag == 1 && encryptflag == 0) dprintf(logfd, "%c", c);
            }
            //if(i == readsize - 1 && logflag == 1) dprintf(logfd, "\n");
        }
    }
    return 0;
}
void reset_terminal (void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}
int main(int argc, char *argv[])
{   
    struct termios tattr;
    tcgetattr(STDIN_FILENO, &saved_attributes);
    atexit(reset_terminal);
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO);
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
    int portno = 1888, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    // char buffer[256];
    // if (argc < 3) {
    //    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    //    exit(0);
    // }
    //portno = atoi(argv[2]);
    int opt;
    int option_index;  
    static struct option long_options[] = {
        {"port", required_argument, NULL, 'p'},
        {"log", required_argument, NULL, 'l'},
        {"encrypt", no_argument, NULL, 'e'},
        {0, 0, 0, 0}
    };
    while((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        switch(opt) {
            case 'p' : {
                portno = atoi(optarg);
                break;
            }
            case 'l' : {
                char* logfile = optarg;
                if((logfd = creat(logfile, 0644)) < 0) {
                    fprintf(stderr, "Can not create log file\n");
                    exit(1);
                }
                logflag = 1;
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
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    // printf("Please enter the message: ");
    // bzero(buffer,256);
    // fgets(buffer,255,stdin);
    // n = write(sockfd,buffer,strlen(buffer));
    // if (n < 0) 
    //      error("ERROR writing to socket");
    // bzero(buffer,256);
    // n = read(sockfd,buffer,255);
    // if (n < 0) 
    //      error("ERROR reading from socket");
    // printf("%s\n",buffer);
    thread_id1 = calloc(1, sizeof(pthread_t));
    thread_id2 = calloc(1, sizeof(pthread_t));
    if(pthread_create(thread_id1, NULL, thread_input, NULL) != 0) {
        fprintf(stderr, "pthread_create() failed!\n");
        exit(1);            
    }
    if(pthread_create(thread_id2, NULL, thread_output, NULL) != 0) {
        fprintf(stderr, "pthread_create() failed!\n");
        exit(1);            
    }
    void *res1;
    void *res2;
    if(pthread_join(*thread_id1, &res1) != 0) {
        fprintf(stderr, "pthread_join() failed!\n");
    }
    if(pthread_join(*thread_id2, &res2) != 0) {
        fprintf(stderr, "pthread_join() failed!\n");
    }
    return 0;
}