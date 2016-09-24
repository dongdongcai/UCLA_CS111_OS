#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include <string.h>

int opt_yield = 0,
    flag = 0,
    lock = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
struct thread_data {
    long long *pointer;
    int numberofiter;
};

void newadd(long long *pointer, long long value) {
    long long sum;
    long long now;
    do{    
        now = *pointer;
        sum = now + value;
        if(opt_yield)
            pthread_yield();
    }while((__sync_val_compare_and_swap(pointer, now, sum)) != now);
}

void add(long long *pointer, long long value) {
        if(flag == 1) pthread_mutex_lock(&mymutex);
        else if(flag == 2) while(__sync_lock_test_and_set(&lock, 1) == 1);
        long long sum = *pointer + value;
        if(opt_yield)
            pthread_yield();
        *pointer = sum;
        if(flag == 1) pthread_mutex_unlock(&mymutex);
        else if(flag == 2) __sync_lock_release(&lock);
}

void *thread_action(void *arg) {
    struct thread_data *my_data = arg;
    int numberofiter = my_data->numberofiter,
        i;
    long long *p = my_data->pointer;
    for(i = 0; i < numberofiter; i++) {
        if(flag != 3) add(p, 1);
        else newadd(p, 1);
    }
    for(i = 0; i < numberofiter; i++) {
        if(flag != 3) add(p, -1);
        else newadd(p, -1);
    }   
    return 0;
}

int main(int argc, char** argv) {
    long long pointer = 0;
    int numberofthreads = 1, 
        numberofiter = 1,
        opt,
        option_index,
        i,
        returnvalue = 0;
    pthread_t *thread_id;
    struct thread_data *my_data;
    struct timespec start, end;
    static struct option long_options[] = {
        {"threads", required_argument, NULL, 't'},
        {"iterations", required_argument, NULL, 'i'},
        {"yield", no_argument, NULL, 'y'},
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
                opt_yield = 1;
                break;
            }
            case 's' : {
                if(strcmp(optarg, "m") == 0)    
                    flag = 1;
                else if(strcmp(optarg, "s") == 0)
                    flag = 2;
                else if(strcmp(optarg, "c") == 0)
                    flag = 3;
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
    if((my_data = (struct thread_data*)malloc(sizeof(struct thread_data))) == NULL) {
        fprintf(stderr, "malloc() for my_data failed!\n");
        exit(1);
    }
    my_data->pointer = &pointer;
    my_data->numberofiter = numberofiter;
    if((thread_id = (pthread_t*)malloc(numberofthreads * sizeof(pthread_t))) == NULL) {
        fprintf(stderr, "malloc() for pthread_ t failed!\n");
        exit(1);
    }
    if((clock_gettime(CLOCK_MONOTONIC, &start)) != 0) {
        fprintf(stderr, "clock_gettime() failed!\n");
        exit(1);
    }
    for(i = 0; i < numberofthreads; i++) {
        if(pthread_create(&thread_id[i], NULL, thread_action, my_data) != 0) {
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
    //printf("%d threads x %d iterations x (add + subtract) = %d operations\n", numberofthreads, numberofiter, 2 * numberofiter * numberofthreads);
    if(pointer != 0) {
        fprintf(stderr, "ERROR: final count = %lld\n", pointer);
        returnvalue = 1;
    }
    printf("%d %lld\n", numberofthreads, (elapsetime) / (2 * numberofiter * numberofthreads));
    //pthread_mutex_destroy(&mymutex);
    return returnvalue;
}