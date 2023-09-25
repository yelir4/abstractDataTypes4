#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdbool.h>
#include "list.h"
#include "set.h"

// coen 12 lab 4, set.c
// 11/10/21
// implement the set header file

typedef struct list LIST;
typedef struct set SET;

/*	struct set
 *	utilizes array of linked lists
 *	compare and hash functions passed in
 */
struct set {
	int count;
	int length;
	struct list **lists;
	int (*compare)();
	unsigned (*hash)();
};

/*	@param (maxElts/20) is the length of "lists" array, in which each index is a linked list
 *	@param compare and hash functions
 *	@return pointer to set sp
 *	@notation O(m) where m is maxElts/20 (also sp->length)
 */
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
	SET *sp = malloc(sizeof(SET));
	assert(sp != NULL);

	sp->count = 0; // total number of items in the set
	sp->length = (maxElts/20); // value "a"

	sp->lists = malloc(sizeof(LIST*)*sp->length);
	assert(sp->lists != NULL);

	int i;
	for(i=0; i<sp->length; i++) {
		sp->lists[i] = createList(compare);
		assert(sp->lists[i] != NULL);
	}

	sp->compare = compare;
	sp->hash = hash;
	return sp;
}

/*	deallocates memory associated with set sp
 *	@notation O(n*m) where n is the count of any given list and m is sp->length
 */
void destroySet(SET *sp) {
	assert (sp != NULL);

	int i;
	for(i=0; i<sp->length; i++)
		destroyList(sp->lists[i]);

	free(sp->lists);
	free(sp);
}

/*	@return count of all elements
 *	@notation O(1) all cases
 */
int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->count;
}

/*	add element to sp
 *	utilize hash function to determine which list to insert to
 *	@notation O(n) where n is lp->counts of given list
 */
void addElement(SET *sp, void *elt) {
	assert(sp != NULL);
	assert(elt != NULL);

	unsigned hash = (*sp->hash)(elt) % sp->length;

	if(findItem(sp->lists[hash], elt) == NULL) { // if elt not present in list
		sp->count++;
		addFirst(sp->lists[hash], elt); // this increments this list's count
	}
}

/*	remove element from set sp
 *	@notation O(n) worst case where n is lp->count for given list
 */
void removeElement(SET *sp, void *elt) {
	assert(sp != NULL);
	assert(elt != NULL);

	unsigned hash = (*sp->hash)(elt) % sp->length;

	if(findItem(sp->lists[hash], elt) != NULL) { // if elt is found in sp
		sp->count--;
		removeItem(sp->lists[hash], elt);
	}
}

/*	find elt in sp
 *	@return elt if found, null otherwise (via findItem function)
 *	@notation O(n) where n is lp->count for given list
 */
void *findElement(SET *sp, void *elt) {
	assert(sp != NULL);
	assert(elt != NULL);

	unsigned hash = (*sp->hash)(elt) % sp->length;
	return findItem(sp->lists[hash], elt);
}

/*	@return array with all elements in sp
 *	@notation O(n) where n is sp->count
 */
void *getElements(SET *sp) {
	assert(sp != NULL);

	void **copy = malloc(sizeof(void*)*sp->count);
	assert(copy != NULL);

	int i, j;
	i = j = 0;

	while(i<sp->count && j<sp->length) {
		memcpy(copy + i, getItems(sp->lists[j]), sizeof(void*)*numItems(sp->lists[j]));
		i += numItems(sp->lists[j]);
		j++;
	}
	return copy;
}
