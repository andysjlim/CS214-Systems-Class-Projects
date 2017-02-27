/*
 * Patrick McGrath and Seung Jae Lim
 * Systems Programming Assignment 1, Memory Allocation
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

//Given by professor

int i;
#define malloc(x) myMalloc(x, __FILE__, __LINE__)
#define free(x) myFree(x, __FILE__, __LINE__)

void testCaseA(){
    printf("\n------------------------------\n");
    printf("1000 separate malloc()s of 1 byte, then free() the 1000 1 byte pointers one by one.\n");
    void* a[1000];
    int malloccount =0;
    struct timeval start,end;

    gettimeofday(&start, NULL);
    for(i = 0; i < 1000; i++){
        a[i] = malloc(1);
        if(a[i] == false){
            break;
        }
        malloccount++;
    }
    for(i = 0; i < malloccount; i++){
        free(a[i]);
    }
		gettimeofday(&end, NULL);
		printf("\nTime took for test case A: %ld microseconds\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

void testCaseB(){
    printf("\n\n\n\n\n\n\n\n------------------------------\n");
    printf("Running malloc of one byte then immediately free it. 1000 times.\n");
    void* a = malloc(1);
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for(i = 0; i < 1000; i++){
        free(a);

        a = malloc(1);
    }
    free(a);

	gettimeofday(&end, NULL);
	printf("\nTime took for test case B: %ld microseconds\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

void testCaseC(){
    printf("\n\n\n\n\n\n\n------------------------------\n");
    printf("Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer - do this 1000 times.\n");
    printf("- Keep track of each operation so that you eventually malloc() 1000 times.\n");
    printf("- Keep track of each operation so that you eventaully free() all pointers\n");
    i = 0;
    int malloccount = 0;
    int random;
    void * a[1000];
    struct timeval start, end;
    gettimeofday(&start, NULL);
    while(malloccount < 1000){
        random = rand() % 2;
        if(random){
            a[i] = malloc(1);
            if(a[i] != 0){
                malloccount++;
                i++;
            }
        } else{
            if(i != 0){
                free(a[--i]);
            } else{
            }
        }
    }
    printf("There are leftover %d stuff. So I'm gonna free them.\n", i);
    int j;
    for(j = 0; j < i; j++){
        free(a[j]);
    }
    gettimeofday(&end, NULL);
	printf("\nTime took for test case C: %ld microseconds\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

void testCaseD(){
    printf("\n\n\n\n\n\n------------------------------\n");
    printf("Randomly choosing between randomly-sized malloc() or free()ing a pointer.\n");
    printf("- Keep track of each malloc so all mallocs do not exceed total memory capacity.\n");
    printf("- Keep track of each operation so that you eventually malloc() 1000 times.\n");
    printf("- Keep track of each operation so that you eventually free() all pointers.\n");
    printf("- Choose a random allocation size between 1 and 64 bytes.\n");
    int randomOper, randomSize, mallocCount = 0;
    void * a[1000];
    i=0;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    while(mallocCount < 1000){
        randomOper = rand() % 2;
        if(randomOper){
            randomSize = rand() % 64 + 1;
            a[i] = malloc(randomSize);
            if(a[i] != 0){
                mallocCount++;
                i++;
            }
        } else{
            if(i != 0){
                free(a[--i]);
            }
        }
    }
    printf("There are leftover %d stuff. So I'm gonna free them.\n", i);
    int j;
    for(j = 0; j < i; j++){
        free(a[j]);
    }
    gettimeofday(&end, NULL);
	printf("\nTime took for test case D: %ld microseconds\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

void testCaseE(){
    printf("\n\n\n\n\n------------------------------\n");
    printf("Malloc an incrementing size. When reached capacity, free all BACKWARD.\n");
    void * a[50];
    i = 0;
    int cont = 0;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    do{
        a[i] = malloc(i);
        if(a[i] == 0){
            cont = 1;
        }
        i++;
    }while(cont == 0);
    int j;
    for(j = 0; j < i-1; j++){
        free(a[j]);
    }
    gettimeofday(&end, NULL);
	printf("\nTime took for test case E: %ld microseconds\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

void testCaseF(){
    printf("\n\n\n\n------------------------------\n");
    printf("Malloc an decrementing size. When reached capacity, free all FORWARD.\n");
    void * a[50];
    i = 100;
    int j;
    int cont = 0;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    do{
        a[i] = malloc(i);
        if(a[i] == 0){
            cont = 1;
        }
        i--;
    }while(cont == 0 && i > 0);
    for(j = 50; j > i; j--){
        free(a[j-1]);
    }
    gettimeofday(&end, NULL);
	printf("\nTime took for test case F: %ld microseconds\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

int main(int argc, char**argv){
    testCaseA();
    testCaseB();
    testCaseC();
    testCaseD();
    testCaseE();
    testCaseF();
}
