#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// printfs are for debugging; remove them when you use/submit your library
#define MAX_LEVELS 17
#define MIN_REQ_SIZE 256
#define INDICATE_FREE 56
//size of the contiguous memory chunk
int size;
//start address of the memory chunk
long int * start_address;
//pointers to the start addresses of the first free blocks of each level
long int free_block_lists [MAX_LEVELS];
int num_of_levels;
int powers_of_two[17];

int binit(void *chunkpointer, int chunksize) {

	printf("binit called\n");

	//set all blocks to not free
	memset(chunkpointer, -1, chunksize);

	//indicate that the chunk is free by setting the first byte to 0
	chunkpointer[0] = 0;
	free_block_lists[0] = (long int) chunkpointer;

	int current_power = chunksize;
	int i;
	for(i = 0; current_power >= MIN_REQ_SIZE; current_power /= 2){
		powers_of_two[i] = current_power;
		i++;
	}

	//initilize the free block lists of each level	
	for (i = 1; i < MAX_LEVELS; i++){
		free_block_lists[i] = NULL;
	}

	//save into global variables
	size = chunksize;
	start_address =  (long int *) chunkpointer;
	num_of_levels = log2(size / MIN_REQ_SIZE);

	return (0);		// if success
}

void *balloc(int objectsize) {

	printf("balloc called\n");

	//find the appropriate level to allocate from
	objectsize+= sizeof(long int *);

	if (objectsize > chunksize || objectsize < MIN_REQ_SIZE)
		return -1;

	int level = 0;
	while (objectsize < powers_of_two[level+1])
		level++; 
	
	//allocate a block at the level
	long int block_address = allocate_at_level(level);
	
	//if it is -1 (error) return error message
	if (block_address < 0)
	    return -1;
    //otherwise, return the address incremented by one to protect the word indicating that the block is not free and is allocated
    else
        return (void *) (block_address + 1);
}

long int allocate_at_level(int level){

	if (level < 0)
		return -1;

	// checking if there are no free blocks at the wanted level 
	if (free_block_lists[level] == NULL){
		// get a bigger block, recursively
		long int bigger_block = allocate_at_level(level-1);
		
		// if no bigger block availble return error
		if (bigger_block < 0)
			return -1;
		
		// split the bigger block
		int block_size = powers_of_two[level];
		long int first_block = bigger_block;
		long int second_block = bigger_block + block_size;

		// set the indicators for free and unfree blocks. 
		// Also set the next and prev pointers of the free block to NULL 
		// since it is the only one in the list of free blocks at this level
		start_address[(first_block - start_address) / sizeof(long int *)] = 1; //indicate that it is full
		start_address[(second_block - start_address) / sizeof(long int *)] = -level; //indicate that it is free and record its level
		start_address[(second_block - start_address) / sizeof(long int *) + 1] = NULL; //pointer to next free block. Null currently
		start_address[(second_block - start_address) / sizeof(long int *) + 2] = NULL; //pointer to previous. Null currently

		free_block_lists[level] = second_block; //add second block to list of free blocks at this level
		return first_block; //return the first block  

	}

	// if there is a free block at this level, then use it
	long int * block_address = (long int *) free_block_lists[level];
	free_block_lists[level] = block_address[1]; //make the free block list head point the 2nd block in the list, possibly NULL
    if (free_block_lists[level] != NULL){ // make the now first free block's prev ptr NULL
    	start_address[(free_block_lists[level] - start_address)/sizeof(long int *) + 2] = NULL;
    }

	block_address[0] = 1; //tag this block as allocated

	return (long int) block_address;


	
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
