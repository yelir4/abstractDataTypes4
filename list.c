#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdbool.h>
#include "list.h"

// coen 12 lab 4, list.c
// 11/10/21
// implement the linked list datastructure

typedef struct list LIST;
typedef struct node NODE;

// previous node pointer so its a doubly linked list
struct node {
	void* data;
	struct node *next;
	struct node *prev;
};

struct list {
	int count;
	struct node *head;
	int (*compare)(); // compare function passed in
};

/*	@param compare function
 *	@return list pointer
 *	@notation O(1) all cases
 */
LIST *createList(int (*compare)()) {
	LIST *lp = malloc(sizeof(LIST));
	assert(lp != NULL);

	lp->count = 0;

	NODE *np = malloc(sizeof(NODE)); // dummy/sentinel node
	assert(np != NULL);
	np->data = NULL;
	np->prev = np;
	np->next = np; // circular implementation, initially points to self
	lp->head = np;

	lp->compare = compare;
	return lp;
}

/*	deallocates memory associated with lp
 *	@notation O(n) where n = lp->count
 */
void destroyList(LIST *lp) {
	assert(lp != NULL);
	NODE *nCur, *nCur2; // node pointer to traverse through lp
	nCur = lp->head->next;

	while(nCur != lp->head) { // stop at dummy node
		nCur2 = nCur;
		nCur = nCur->next;
		free(nCur2);
	}
	free(nCur);
	free(lp);
}

/*	@return number of elements in linked list lp
 *	@notation O(1) all cases
 */
int numItems(LIST *lp) {
	assert(lp != NULL);
	return lp->count;
}

/*	add item to first slot in lp
 *	@notation O(1) all cases
 */
void addFirst(LIST *lp, void *item) {
	assert(lp != NULL);
	assert(item != NULL);

	NODE *nNew = malloc(sizeof(struct node));
	assert(nNew != NULL);
	nNew->data = item;

	nNew->prev = lp->head; // nNew's previous pointer is sentinel
	nNew->next = lp->head->next; // nNew's next pointer is node after sentinel
	lp->head->next->prev = nNew;
	lp->head->next = nNew; // sentinel's next pointer is nNew
	lp->count++;
}

/*	add item to last slot in lp
 *	@notation O(1) all cases
 */
void addLast(LIST *lp, void *item) {
	assert(lp != NULL);
	assert(item != NULL);

	NODE *nNew = malloc(sizeof(struct node));
	assert(nNew != NULL);
	nNew->data = item;

	nNew->prev = lp->head->prev;
	nNew->next = lp->head;
	lp->head->prev->next = nNew;
	lp->head->prev = nNew;
	lp->count++;
}

/*	@return data from first node in lp, then removes it
 *	@precondition lp is not empty
 *	@notation O(1) all cases
 */
void *removeFirst(LIST *lp) {
	assert(lp != NULL);
	assert(lp->count > 0); // list cant be empty

	NODE *nDel;
	nDel = lp->head->next;

	lp->head->next->next->prev = lp->head;
	lp->head->next = lp->head->next->next;

	lp->count--;
	void *item = nDel->data;
	free(nDel);
	return item;
}

/*	@return data from last node in lp, then removes it
 *	@precondition lp is not empty
 *	@notation O(1) all cases
 */
void *removeLast(LIST *lp) {
	assert(lp != NULL);
	assert(lp->count > 0); // list cant be empty

	NODE *nDel;
	nDel = lp->head->prev;

	lp->head->prev->prev->next = lp->head;
	lp->head->prev = lp->head->prev->prev;

	lp->count--;
	void *item = nDel->data;
	free(nDel);
	return item;
}

/*	@return data from first node in lp
 *	@precondition lp is not empty
 *	@notation O(1) all cases
 */
void *getFirst(LIST *lp) {
	assert(lp != NULL);
	assert(lp->count > 0);

	return lp->head->next->data;
}

/*	@return data from last node in lp
 *	@precondition lp is not empty
 *	@notation O(1) all cases
 */
void *getLast(LIST *lp) {
	assert(lp != NULL);
	assert(lp->count > 0);

	return lp->head->prev->data;
}

/*	remove node with data "item" passed in from lp if found
 *	@precondition lp->compare != NULL
 *	@notation O(n) where n = lp->count
 */
void removeItem(LIST *lp, void *item) {
	assert(lp != NULL);
	assert(item != NULL);
	assert(lp->compare != NULL);

	NODE *nDel = lp->head->next; // cursor node to be deleted

	int i;
	for(i=0; i<lp->count; i++) {
		if((*lp->compare)(nDel->data, item) == 0) {
			nDel->prev->next = nDel->next;
			nDel->next->prev = nDel->prev;
			free(nDel);
			lp->count--;
			i=lp->count; // end loop
		} else {
			nDel = nDel->next;
		}
	}
}

/*	@return data if found in lp, null otherwise
 *	@precondition lp->compare != NULL
 *	@notation O(n) where n = lp->count
 */
void *findItem(LIST *lp, void *item) {
	assert(lp != NULL);
	assert(item != NULL);
	assert(lp->compare != NULL);

	NODE *nCur = lp->head->next;

	int i;
	for(i=0; i<lp->count; i++) {
		if((*lp->compare)(nCur->data, item) == 0)
			return nCur->data;
		nCur = nCur->next;
	}
	return NULL;
}

/*	@return array of items in lp
 *	@notation O(n) where n = lp->count
 */
void *getItems(LIST *lp) {
	assert(lp != NULL);

	void **copy = malloc(sizeof(void*)*lp->count);
	assert(copy != NULL);

	NODE *nCur = lp->head->next;

	int i;
	for(i=0; i<lp->count; i++) {
		void *copy2 = nCur->data;
		assert(copy2 != NULL);
		copy[i] = copy2;
		nCur = nCur->next;
	}
	return copy;
}
