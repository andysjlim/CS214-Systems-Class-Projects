#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Allows for debugging bad calls to malloc or free
#define malloc(x) myMalloc(x, __FILE__, __LINE__)
#define free(x) myFree(x, __FILE__, __LINE__)

#define memSize 5000
#define true 1
#define false 0

static char memSpace[memSize];
const char *endMem;

typedef struct memNode {

	short hasPrev: 1;
	short hasSucc: 1;
	short isFree: 1;
	unsigned int ID;										/* ID used to determine if created by myMalloc */
	unsigned int line;
	char *file;
	unsigned int prevSize;									/* Size of previous pointer */
	unsigned int size;										/* Size of space for memNode */

} memNode;

void * myMalloc(unsigned int size, char *file, unsigned int line);

void * smallMalloc(unsigned int size, char* file, unsigned int line, memNode *chunkSmall);

void * largeMalloc(unsigned int size, char* file, unsigned int line, memNode *chunkLarge, memNode *chunkSmall);

short myFree(void *passedptr, const char *file, unsigned int line);

