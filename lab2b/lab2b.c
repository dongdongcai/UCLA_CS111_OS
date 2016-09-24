#define _GNU_SOURCE
#include "SortedList.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include <sys/stat.h> 
#include <fcntl.h>

int flag = 0,
    lock = 0,
    numberofiter = 1,
    numberofthreads = 1;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
SortedList_t *head;

void *thread_action(void *arg) {
	SortedListElement_t *linknodes = arg;
	int i;
	for(i = 0; i < numberofiter; i++) {
        //printf("node: %s\n", linknodes[i].key);
		switch(flag) {
			case 1 : {
			    pthread_mutex_lock(&mymutex);
			    SortedList_insert(head, &linknodes[i]);
			    pthread_mutex_unlock(&mymutex);
				break;
			}
			case 2 : {
                while(__sync_lock_test_and_set(&lock, 1) == 1);
			    SortedList_insert(head, &linknodes[i]);
                __sync_lock_release(&lock);
				break;				
			}
            default : {
                SortedList_insert(head, &linknodes[i]);
                break;
            }
		}
	}
    // SortedList_t *c = head;
    // for(i = 0; i < numberofiter; i++) {
    //     printf("%s->", (c->next->key));
    //     c = c->next;
    // }
    //printf("\n");
    switch(flag) {
        case 1 : {
            pthread_mutex_lock(&mymutex);
            SortedList_length(head);
            pthread_mutex_unlock(&mymutex);
            break;
        }
        case 2 : {
            while(__sync_lock_test_and_set(&lock, 1) == 1);
            SortedList_length(head);
            __sync_lock_release(&lock); 
            break;              
        }
        default : {
            SortedList_length(head);
            break;
        }
    }
	for(i = 0; i < numberofiter; i++) {
		switch(flag) {
			case 1 : {
			    pthread_mutex_lock(&mymutex);
			    SortedListElement_t* temp = SortedList_lookup(head, linknodes[i].key);
			    SortedList_delete(temp);
			    pthread_mutex_unlock(&mymutex);
				break;
			}
			case 2 : {
                while(__sync_lock_test_and_set(&lock, 1) == 1);
			    SortedListElement_t* temp = SortedList_lookup(head, linknodes[i].key);
			    SortedList_delete(temp);
                __sync_lock_release(&lock); 
				break;				
			}
            default : {
                SortedListElement_t* temp = SortedList_lookup(head, linknodes[i].key);
                SortedList_delete(temp);
                break;
            }
		}
	}	
	return 0;
}

int main(int argc, char** argv) {
    int opt,
    	option_index,
    	i,
    	j,
    	returnvalue = 0;
    SortedListElement_t **nodes;
    pthread_t *thread_id;
    opt_yield = 0;
    struct timespec start, end;
    static struct option long_options[] = {
        {"threads", required_argument, NULL, 't'},
        {"iterations", required_argument, NULL, 'i'},
        {"yield", required_argument, NULL, 'y'},
        {"sync", required_argument, NULL, 's'},
        {0, 0, 0, 0}
    };
    while((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
    	switch(opt) {
    		case 't' : {
    			numberofthreads = atoi(optarg);
    			break;
    		}
    		case 'i' : {
    			numberofiter = atoi(optarg);
    			break;
    		}
            case 'y' : {
                int k;
                for(k = 0; k < strlen(optarg); k++) {
                	switch(optarg[k]) {
                		case 'i' : {
                			opt_yield |= INSERT_YIELD;
                			break;
                		}
                		case 'd' : {
                			opt_yield |= DELETE_YIELD;
                			break;
                		}
                		case 's' : {
                			opt_yield |= SEARCH_YIELD;
                			break;
                		}
                	}
                }
                break;
            }
            case 's' : {
                if(strcmp(optarg, "m") == 0)    
                    flag = 1;
                else if(strcmp(optarg, "s") == 0)
                    flag = 2;
                else {
                    fprintf(stderr, "Please use correct argument for sync!\n");
                    exit(1);
                }
                break;
            }
    		default : {
    			perror("Argument error");
    			exit(1);
    		}
    	}
    }
    if(flag == 0 && numberofthreads > 1) {
        printf("Run unprotected method for multi threads may cause segfault!\n");
        //exit(1);        
    }
    if((nodes = (SortedListElement_t**)malloc(numberofthreads * sizeof(SortedListElement_t*))) == NULL) {
    	fprintf(stderr, "malloc() for nodes[] failed!\n");
    	exit(1);
    }
    for(i = 0; i < numberofthreads; i++) {
    	if((nodes[i] = (SortedListElement_t*)malloc(numberofiter * sizeof(SortedListElement_t))) == NULL) {
    		fprintf(stderr, "malloc() for nodes failed!\n");
    		exit(1);    		
    	}
    	for(j = 0; j < numberofiter; j++) {
            int k;
            char randkey[4];
    		if((nodes[i][j].key = (char *)malloc(4 * sizeof(char))) == NULL) {
        		fprintf(stderr, "malloc() for key failed!\n");
    			exit(1);			
    		}
            nodes[i][j].next = NULL;
            nodes[i][j].prev = NULL;
            for(k = 0; k < 3; k++) {
    		  //sprintf((char*)nodes[i][j].key, "%d", rand() % 1000);
                int balba =  rand();
                //printf("%d\n", balba % 10);
                randkey[k] = '0' + balba % 10;
                //printf("randkey: %c\n", randkey[k]);
            }
            randkey[3] = '\0';
            strcpy((char*)nodes[i][j].key, randkey);
            //printf("nodekey: %s\n", nodes[i][j].key);
    	}
    }
    if((head = (SortedList_t*)malloc(sizeof(SortedList_t))) == NULL) {
     	fprintf(stderr, "malloc() for head failed!\n");
    	exit(1);   	
    }
    head->next = NULL;
    head->prev = NULL;
    if((thread_id = (pthread_t*)malloc(numberofthreads * sizeof(pthread_t))) == NULL) {
    	fprintf(stderr, "malloc() for pthread_ t failed!\n");
    	exit(1);
    }
    if((clock_gettime(CLOCK_MONOTONIC, &start)) != 0) {
    	fprintf(stderr, "clock_gettime() failed!\n");
    	exit(1);
    }
    for(i = 0; i < numberofthreads; i++) {
    	if(pthread_create(&thread_id[i], NULL, thread_action, nodes[i]) != 0) {
    		fprintf(stderr, "pthread_create() failed!\n");
    		exit(1);
    	}
    }
    for(i = 0; i < numberofthreads; i++) {
    	if(pthread_join(thread_id[i], NULL) != 0) {
    		fprintf(stderr, "pthread_join() failed!\n");
    		exit(1);
    	}
    }
    if((clock_gettime(CLOCK_MONOTONIC, &end)) != 0) {
    	fprintf(stderr, "clock_gettime() failed!\n");
    	exit(1);
    }
    long long elapsetime = (end.tv_sec - start.tv_sec) * 1000000000;
    elapsetime += end.tv_nsec;
    elapsetime -= start.tv_nsec;    
    printf("%d threads x %d iterations x (insert + lookup/delete) = %d operations\n", numberofthreads, numberofiter, 2 * numberofiter * numberofthreads);
    int length = SortedList_length(head);
    if(length != 0) {
    	fprintf(stderr, "ERROR: final length = %d\n", length);
    	returnvalue = 1;
    }
    printf("elapsed time: %lldns\nper operation: %lldns\n", elapsetime, (elapsetime) / (2 * numberofiter * numberofthreads));
    // if((logfd = open("result.txt",  O_CREAT|O_WRONLY|O_APPEND, 0644)) < 0) {
    //     fprintf(stderr, "Can not create log file\n");
    //     exit(1);
    // }
    // double realtime = (1.0 * elapsetime) / (2 * numberofiter * numberofthreads * numberofiter * numberofthreads);
    // dprintf(logfd, "%d %f\n", numberofthreads, realtime);
    //close(logfd);    
    return returnvalue;
}