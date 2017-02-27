#include "mymalloc.h"

/* 
	Frees allocated space made with myMalloc function. Also checks for 
 	potential errors.	
*/
short myFree (void *passedptr, const char *file, unsigned int line)
{
	memNode *ptr, *prev, *succ;

	if (passedptr == 0) {
		printf("Error: Attempt to free a NULL pointer. File %s and line %u\n", file, line);
		return false;
	}
	if ((char *) passedptr < memSpace || (char *) passedptr > &memSpace[memSize]) {
		printf("Error: Attempt to free pointer that was not allocated. File %s and line %u\n", file, line);
		return false;
	}
    ptr = (memNode *) passedptr - 1;
    if (ptr->ID != 0xAAAAAAAA) {
		printf("Error: Attempt to free pointer that was not returned by myMalloc. File %s line %u\n", file, line);
		return false;
	}
    
	/* 
		Now that the passed pointer has been validated, we can create a memNode struct for it
		and attempt to free the space.
	*/
	if (ptr->isFree) {
		printf("Error: Pointer has already been freed. File %s line %u\n", file, line);
		return false;
	}

	/* If current pointer has a previous pointer and the prev is free, merge their space */
	if (ptr->hasPrev) {		 
		prev = (memNode *) ((char *) ptr - ptr->prevSize - sizeof(memNode));
		if (prev->isFree) {
			prev->hasSucc = ptr->hasSucc;
			prev->size += sizeof(memNode) + ptr->size;

			if (ptr->hasSucc) {
				succ = (memNode *) ((char *) ptr + sizeof(memNode) + ptr->size);
				succ->prevSize = prev->size;			
			}
			ptr->isFree = true;
		}
	}

	/* Mark pointer as being free and set prev equal to it (to allow consistent modification/merging) */
	if (!ptr->isFree) {
		ptr->isFree = true;
		prev = ptr;
	}

	/* If prev pointer has a successor and it's free, merger the two */
	if (prev->hasSucc) {    
		memNode *prevSucc = (memNode *) ((char *) prev + sizeof(memNode) + prev->size);
		if (!prevSucc->isFree)
			return true;

		prev->size += sizeof(memNode) + prevSucc->size;

		/* If there is a successor of the merged successor, set its previous size to prev's size */
		if (prevSucc->hasSucc) {
			memNode *nextSucc = (memNode *) ((char *) prevSucc + sizeof(memNode) + prevSucc->size);
			nextSucc->prevSize = prev->size;
		}
	}
	return true;
}


/* 
	Allocates space in chunk, as requested by user. If unable to
	find and return address of available space, returns false (0)
*/
void * miniMalloc (unsigned int size, char *file, unsigned int line, memNode *chunkSmall)
{
	memNode *ptr = chunkSmall;
	/* While ptr is within bounds of memory */
	while ((ptr+size+sizeof(memNode)) < (memNode *) endMem) {
		if (ptr->size <= size) {											/* If ptr size cannot contain requested space, increment */
			ptr = (memNode *) ((char *) ptr + sizeof(memNode) + ptr->size);
        }
		else if (!ptr->isFree) { 											/* If pointer isn't free, increment */
			ptr = (memNode *) ((char *) ptr + sizeof(memNode) + ptr->size);
        }
		else if (ptr->size <= size + 8) {									/* If ptr size can fit requested space as well as extra bytes, use block */
			ptr->isFree = false;
			return ++ptr;
		}
		else {																/* Create new block of memory to be allocated for user, and create new pointer with remaining memory (that's free) */
			memNode *newPtr = (memNode *) ((char *) ptr + sizeof(memNode) + size);

			newPtr->hasPrev = true;
			newPtr->hasSucc = ptr->hasSucc;

			
			if (ptr->hasSucc) { 											/* If ptr has a successor, set the prevSize to the size of the newPtr size */
				memNode *oldSuccPtr = (memNode *) ((char *) ptr + sizeof(memNode) + size);
				oldSuccPtr->prevSize = oldSuccPtr->prevSize - sizeof(memNode) - size;
			}
			newPtr->prevSize = size;										/* Setting newPtr's prevSize as ptr's size */
			newPtr->size = ptr->size - sizeof(memNode) - size;				/* NewPtr->size is remainder of memory from ptr (after new allocation) */
			newPtr->isFree = true;
			newPtr->ID = 0xAAAAAAAA;
			ptr->size = size;
			ptr->file = file;
			ptr->line = line;
			ptr->isFree = false;
			ptr->hasSucc = true;

			return ++ptr;													/* Returning pointer of newly allocated space */
		}
		/* While ptr is within bounds of small chunk and not exceeding memory's limits */
	}
	return false;
}

/* 
	Allocates space, as requested by user. On initialization, the memory is organized into
	one large chunk. Smaller chunks will be made out of this large chunk upon request for allocated memory by user.
	A pointer to the allocated memory will be returned upon completion. 
	Otherwise, a false (0) will be returned upon failure of allocation.
*/
void * myMalloc(unsigned size, char *file, unsigned int line)
{
	static int initialized = false;
	static struct memNode *chunkSmall;

	void *rtrnSpace;
	
	if (!initialized) {									/* If memory has not been initialized for allocation, hence static variable */
		initialized = true;
		endMem = &memSpace[memSize];

		chunkSmall = (memNode *) memSpace;
		chunkSmall->size = memSize - sizeof(memNode);	/* Allocating remainder of space to small chunk */
		chunkSmall->ID = 0xAAAAAAAA;
		chunkSmall->prevSize = 0;
		chunkSmall->hasPrev = false;
		chunkSmall->hasSucc = false;
		chunkSmall->isFree = true;
	}

	rtrnSpace = miniMalloc(size, file, line, chunkSmall);

	return (void *) rtrnSpace;							/* Return 0 if space cannot be allocated */
}
