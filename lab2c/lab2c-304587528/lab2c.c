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
//Todo: every sublist need a lock
int flag = 0,
    *lock,
    numberofiter = 1,
    numberofthreads = 1,
    numberofsublists = 1;
pthread_mutex_t *mymutex;
SortedList_t **head;

void *thread_action(void *arg) {
	SortedListElement_t *linknodes = arg;
	int i,
        j,
        length = 0;
	for(i = 0; i < numberofiter; i++) {
        //printf("node: %s\n", linknodes[i].key);
         int toinsert = 0;
         char* tempkey = (char*)linknodes[i].key;
         for(j = 0; j < 3; j++) {
             //printf("%lld\n", *(linknodes[i].key + j));
             toinsert += tempkey[j];
         }
        //printf("%d\n", toinsert);
        //int toinsert = atoi(linknodes[i].key) % numberofsublists;
        toinsert = toinsert % numberofsublists;
		switch(flag) {
			case 1 : {
			    pthread_mutex_lock(&mymutex[toinsert]);
			    SortedList_insert(head[toinsert], &linknodes[i]);
			    pthread_mutex_unlock(&mymutex[toinsert]);
				break;
			}
			case 2 : {
                while(__sync_lock_test_and_set(&lock[toinsert], 1) == 1);
			    SortedList_insert(head[toinsert], &linknodes[i]);
                __sync_lock_release(&lock[toinsert]);
				break;				
			}
            default : {
                SortedList_insert(head[toinsert], &linknodes[i]);
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
            for(i = 0; i < numberofsublists; i++) {
                pthread_mutex_lock(&mymutex[i]);
                length += SortedList_length(head[i]);
            }
            for(i = 0; i < numberofsublists; i++) {
                pthread_mutex_unlock(&mymutex[i]);
            }   
            break;
        }
        case 2 : {
            for(i = 0; i < numberofsublists; i++) {
                while(__sync_lock_test_and_set(&lock[i], 1) == 1);            
                length += SortedList_length(head[i]);
            }
            for(i = 0; i < numberofsublists; i++) {
                __sync_lock_release(&lock[i]);    
            } 
            break;              
        }
        default : {
            for(i = 0; i < numberofsublists; i++) {
                length += SortedList_length(head[i]);
            }
            break;
        }
    }
	for(i = 0; i < numberofiter; i++) {
         int todelete = 0;
         char* tempkey = (char*)linknodes[i].key;
         for(j = 0; j < 3; j++) {
             todelete += tempkey[j];
         }
         todelete = todelete % numberofsublists;        
        //int todelete = atoi(linknodes[i].key) % numberofsublists;
		switch(flag) {
			case 1 : {
			    pthread_mutex_lock(&mymutex[todelete]);
			    SortedListElement_t* temp = SortedList_lookup(head[todelete], linknodes[i].key);
			    SortedList_delete(temp);
			    pthread_mutex_unlock(&mymutex[todelete]);
				break;
			}
			case 2 : {
                while(__sync_lock_test_and_set(&lock[todelete], 1) == 1);
			    SortedListElement_t* temp = SortedList_lookup(head[todelete], linknodes[i].key);
			    SortedList_delete(temp);
                __sync_lock_release(&lock[todelete]); 
				break;				
			}
            default : {
                SortedListElement_t* temp = SortedList_lookup(head[todelete], linknodes[i].key);
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
        {"lists", required_argument, NULL, 'l'},
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
            case 'l' : {
                numberofsublists = atoi(optarg);
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
                //randkey[k] = '0' + balba % 10;
                randkey[k] = balba % 40;
                //printf("randkey: %c\n", randkey[k]);
            }
            randkey[3] = '\0';
            strcpy((char*)nodes[i][j].key, randkey);
            //printf("nodekey: %s\n", nodes[i][j].key);
    	}
    }
    if((head = (SortedList_t**)malloc(numberofsublists * sizeof(SortedList_t*))) == NULL) {
     	fprintf(stderr, "malloc() for head failed!\n");
    	exit(1);   	
    }
    if((lock = (int*)malloc(numberofsublists * sizeof(int))) == NULL) {
        fprintf(stderr, "malloc() for lockfailed!\n");
        exit(1);    
    }
    if((mymutex = (pthread_mutex_t *)malloc(numberofsublists * sizeof(pthread_mutex_t))) == NULL) {
        fprintf(stderr, "malloc() for pthread_mutex_t failed!\n");
        exit(1);    
    }        
    for(i = 0; i < numberofsublists; i++) {
        if((head[i] = (SortedList_t*)malloc(sizeof(SortedList_t))) == NULL) {
            fprintf(stderr, "malloc() for subheads failed!\n");
            exit(1);            
        }
        head[i]->next = NULL;
        head[i]->prev = NULL;
        lock[i] = 0;
        if(pthread_mutex_init(mymutex + i, NULL) != 0) {
            fprintf(stderr, "pthread_mutex_init() failed!\n");
            exit(1);             
        }
        //mymutex[i] = PTHREAD_MUTEX_INITIALIZER;

    }
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
    //printf("%f, %f\n", 1.0 * numberofthreads / numberofsublists, (elapsetime * numberofsublists) / (2.0 * numberofiter * numberofthreads * numberofiter * numberofthreads));
    int length = 0;
    for(i = 0; i < numberofsublists; i++) {
        length += SortedList_length(head[i]);
    }
    if(length != 0) {
    	fprintf(stderr, "ERROR: final length = %d\n", length);
    	returnvalue = 1;
    }
    printf("elapsed time: %lldns\nper operation: %lldns\n", elapsetime, (elapsetime) / (2 * numberofiter * numberofthreads));  
    return returnvalue;
}