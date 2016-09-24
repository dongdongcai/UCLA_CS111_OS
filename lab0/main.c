#include <unistd.h>
#include <ctype.h>
#include <stdio.h> 
#include <getopt.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
void handler(int sig) {
    fprintf(stderr, "error: segment fault occured!\n");
    _exit(3);
}
int main(int argc, char** argv) {

    char *inputfile = NULL, *outputfile = NULL;
    int opt;
    int option_index;
    char buffer[1024];
    int size_read, size_write;
    int flag = 0;
    int *errortest = NULL;
    static struct option long_options[] = {
        {"input", required_argument, NULL, 'i'},
        {"output", required_argument, NULL, 'o'},
        {"segfault", no_argument, NULL, 'f'},
        {"catch", no_argument, NULL, 'c'},
        {0, 0, 0, 0}
    };
    while((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        switch(opt) {
            case 'i' : {
                //printf("%s, %d\n", optarg, option_index);
                inputfile = optarg;
                int ifd = open(inputfile, O_RDONLY);
                if(ifd >= 0) {
                    close(0);
                    dup(ifd);
                    close(ifd);
                } else {
                    fprintf(stderr, "stderr: can not open input file!\n");
                    perror("perror");
                    _exit(1);
                }                           
                break;
            }
            case 'o' : {
                //printf("%s, %d\n", optarg, option_index);
                outputfile = optarg;
                int ofd = creat(outputfile, 0644);
                if(ofd >= 0) {
                    close(1);
                    dup(ofd);
                    close(ofd);
                } else {
                    fprintf(stderr, "stderr: can not create output file!\n");
                    perror("perror");
                    _exit(2);        
                }
                break;
            }
            case 'f' : {
                flag = 1;
                break;
            }
            case 'c' : {
                signal(SIGSEGV, handler);
            }
            default: {
                printf("perror");
            }
        }
    }
    if(flag == 1)  {
        *errortest = 1;
    }
    while((size_read = read(0, &buffer, 1024)) > 0) {
        size_write = write(1, &buffer, size_read);
    }    
    close(0);
    close(1);
    _exit(0);
 }
