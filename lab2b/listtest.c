#include "SortedList.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char* argv[]) {
	//opt_yield = 0;
	SortedList_t *head;
	SortedListElement_t *nodes;
	int numberofnodes = 1,
		i,
		option_index,
		opt;
    static struct option long_options[] = {
        {"nodes", required_argument, NULL, 'n'},
        {0, 0, 0, 0}
    };
    while((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
    	switch(opt) {
    		case 'n' : {
    			numberofnodes = atoi(optarg);
    			break;
    		}
    		default : {
    			perror("Argument error");
    			exit(1);
    		}
    	}
    }
    if((nodes = (SortedListElement_t*)malloc(numberofnodes * sizeof(SortedListElement_t))) == NULL) {
    	fprintf(stderr, "malloc() for nodes failed!\n");
    	exit(1);    		
    }
	for(i = 0; i < numberofnodes; i++) {
		if((nodes[i].key = (char *)malloc(6 * sizeof(char))) == NULL) {
    		fprintf(stderr, "malloc() for key failed!\n");
			exit(1);			
		}
		int random = rand() % 1000;
		sprintf(nodes[i].key, "%d", random);
		printf("%d ", random);
	}
	printf("\n");
    if((head = (SortedList_t*)malloc(sizeof(SortedList_t*))) == NULL) {
     	fprintf(stderr, "malloc() for head failed!\n");
    	exit(1);   	
    }
    head->next = NULL;
    head->prev = NULL;
	for(i = 0; i < numberofnodes; i++) {
		nodes[i].prev = NULL;
		nodes[i].next = NULL;
		SortedList_insert(head, &nodes[i]);
	}
	SortedList_t *current = head;
	for(i = 0; i < numberofnodes; i++) {
		printf("%s ", (current->next->key));
		current = current->next;
	}
	printf("\n");	
	printf("%d\n", SortedList_length(head));
	while(SortedList_length(head) != 0) {
		char s[10];
		scanf("%s", s);
		SortedListElement_t *temp = SortedList_lookup(head, s);
		SortedList_delete(temp);
		int l = SortedList_length(head);
		current = head;
		for(i = 0; i < l; i++) {
			printf("%s ", (current->next->key));
			current = current->next;
		}
		printf("\n");
	}
	return 0;
}