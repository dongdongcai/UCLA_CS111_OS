#include "SortedList.h"

int opt_yield = 0;

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
	if(list == NULL || element == NULL) return;
	SortedListElement_t* current = list;
	//critical section
	while(current->next != NULL && strcmp(current->next->key, element->key) < 0) current = current->next;	
	SortedListElement_t* nextone = current->next;
	if(opt_yield & INSERT_YIELD) {
		pthread_yield();
	}
	current->next = element;
	element->next = nextone;
	element->prev = current;
	if(nextone != NULL) nextone->prev = element;
	//critical section
}

int SortedList_delete(SortedListElement_t *element) {
	if(element == NULL) return 1;
	//critical section
	SortedListElement_t* previous = element->prev;
	SortedListElement_t* nextone = element->next;
	if(opt_yield & DELETE_YIELD) {
		pthread_yield();
	}
	if(element->next != nextone || element->prev != previous) {
		return 1;
	}
	previous->next = nextone;
	if(nextone != NULL) nextone->prev = previous;
	//critical section
	return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
	if(list->next == NULL) return NULL;
	SortedListElement_t* current = list;
	//critical section
	while(current->next != NULL && strcmp(current->next->key, key) != 0) {
		current = current->next;
	}
	if(opt_yield & SEARCH_YIELD) {
		pthread_yield();
	}
	SortedListElement_t* res = current->next;
	//critical section
	return res;	
}

int SortedList_length(SortedList_t *list) {
	if(list->next == NULL) return 0;
	SortedListElement_t* current = list;
	int counter = 0;
	//critical section
	while(current->next != NULL) {
		SortedListElement_t* temp = current->next;
		SortedListElement_t* previous = temp->prev;
		SortedListElement_t* nextone = temp->next;
		if(opt_yield & SEARCH_YIELD) {
			pthread_yield();		
		}
		if(temp->prev != previous || temp->next != nextone) return -1;
		counter++;
		//previous = current;
		current = current->next;
	}
	//critical section
	return counter;
}