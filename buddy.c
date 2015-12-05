#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// printfs are for debugging; remove them when you use/submit your library

//size of the contiguous memory chunk
int size;
//start address of the memory chunk
long int start_address;
//pointers to the start addresses of the first free blocks of each level
long int free_block_lists [17];
int num_of_levels;

int binit(void *chunkpointer, int chunksize) {

	printf("binit called\n");

	//set all blocks to not free
	memset(chunkpointer, -1, chunksize)

	//indicate that the chunk is free by setting the first byte to 0
	chunkpointer[0] = 0;

	size = chunksize;
	start_address = chunkpointer;
	num_of_levels = log2(size / 256);
	//initilize the free block lists of each level
	int i;
	for (i = 0; i < num_of_levels; i++){
		free_block_lists[i] = NULL;
	}
	free_block_lists[0] = start_address;


	return (0);		// if success
}

void *balloc(int objectsize) {

	printf("balloc called\n");

	long int free_block = 

	return (NULL);		// if not success
}

void bfree(void *objectptr) {

	printf("bfree called\n");

	return;
}

void bprint(void) {
	printf("bprint called\n");
	return;
}



int index_in_level_of(p,n){
	return (p - start_address) / (size / (1<<n));
} 