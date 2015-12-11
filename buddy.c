#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "buddy.h"

// printfs are for debugging; remove them when you use/submit your library
#define MAX_LEVELS 17 //assuming 
#define MIN_REQ_SIZE 256
#define INDICATE_ALLOCATED 56
//size of the contiguous memory chunk
int size;
//start address of the memory chunk
long int * start_address;
//pointers to the start addresses of the first free blocks of each level
long int free_block_lists [MAX_LEVELS];
int num_of_levels;
int powers_of_two[17];



int binit(void *chunkpointer, int chunksize) {
	
	chunksize *= 1024;
	start_address =  (long int *) chunkpointer;

	printf("binit called with chunksize\n%d\nChunkPtr\n%ld\n\n", chunksize, (long int) start_address);

	//set all blocks to not free
	memset(start_address, 0, chunksize);


	//precompute powers of two for faster performance
	int current_power;
	int i = 0;
	printf("i\t:\tcurrent_power\n");
	for(current_power = chunksize; current_power >= MIN_REQ_SIZE; current_power /= 2){
		printf("%d\t:\t%d\n", i, current_power);
		powers_of_two[i] = current_power;
		i++;
	}

	//indicate that the chunk is just a free block of the 0th level by setting the first word to 0 
	start_address[0] = 0;
	//adding the address of the block of the 0th level to the list of free blocks at level 0
	free_block_lists[0] = (long int) start_address;
	//initilize the free block lists of each level to NULL except the 0th level
	for (i = 1; i < MAX_LEVELS; i++){
		free_block_lists[i] = (long int) NULL;
	}

	//save into global variables
	size = chunksize;
	num_of_levels = (int) log2((double) (size / MIN_REQ_SIZE) );

	return (0);		// if success
}

void *balloc(int objectsize) {

	printf("balloc called with objectsize = %d\n", objectsize);

	// consider storage cost of additional preample tag that indicates whether the block is free or allocated
	objectsize+= sizeof(long int);

	// check now that it is within allowed sizes. If not, then return -1 error message
	if (objectsize > size || objectsize - 1 < MIN_REQ_SIZE)
		return (void *) -1;

	// find the appropriate level to allocate from
	int level = 0;
	while (objectsize < powers_of_two[level+1])
		level++; 
	
	// allocate a block at the level
	long int block_address = allocate_at_level(level);
	 
	// if it is -1 (error) return error message
	if (block_address < 0)
	    return (void *)-1;
    // otherwise, return the address incremented by one to protect the word that is indicating that the block is not free and is actaully allocated
    else{
    	block_address++;
        return (void *) block_address;
    }
}

long int allocate_at_level(int level){

	// base case: trying to allocate at level higher than 0!
	if (level < 0)
		return -1;

	// checking if there are no free blocks at the wanted level 
	if (free_block_lists[level] == (long int) NULL){
		// if so is the case, get a bigger block, recursively
		long int bigger_block = allocate_at_level(level-1);
		
		// if no bigger block availble return error
		if (bigger_block < 0)
			return (long int) -1;
		
		// split the bigger block
		int wanted_block_size = powers_of_two[level];
		long int first_block = bigger_block;
		long int second_block = bigger_block + wanted_block_size;

		// set the indicators for free and allocated blocks. 
		// Also set the next and prev pointers of the free block to NULL,
		// since it is the only one in the list of free blocks at this level
		start_address[(first_block - (long int) start_address) / sizeof(long int)] = level; //indicate that it is full
		start_address[(second_block - (long int) start_address) / sizeof(long int)] = -level; //indicate that it is free and record its level, both as -level
		start_address[(second_block - (long int) start_address) / sizeof(long int) + 1] = (long int) NULL; //pointer to next free block. Null currently
		start_address[(second_block - (long int) start_address) / sizeof(long int) + 2] = (long int) NULL; //pointer to previous. Null currently
		
		//add second block to list of free blocks at this level
		free_block_lists[level] = second_block; 
		return first_block; //return the first block  

	} else { // else there is a free block at this level, then use it 
		//get the first block in the list of free blocks of this level
		long int * block_address = (long int *) free_block_lists[level];

		//make the free block list head point the 2nd block in the list, which is possibly NULL, making it the 1st in the list
		free_block_lists[level] = block_address[1]; 
		// set what is now the first free block's prev ptr to NULL, since the block is the first one in the list. But check that the block is actually there
	    if (free_block_lists[level] != (long int) NULL){ 
	    	start_address[(free_block_lists[level] - (long int) start_address)/sizeof(long int) + 2] = (long int) NULL;
	    }

		block_address[0] = level; //tag this block as allocated
		return (long int) block_address;		
	}


}

void bfree(void *objectptr) {
	objectptr--;
	long int address = (long int) objectptr;
	printf("====bfree is called for obj at relative address: %ld =====\n", address - (long int)start_address);

	// The computed address needs to be within the memory chunk
	if (address < (long int) start_address || address > (long int) start_address + size){
		return;
	}

	// create a variable to point to the start of this block
	long int* ptr = (long int *) address;

	// level of this block to be deallocated
	int level = (int) ptr[0];
	printf("Level = %d\n", level);

	//special case for level 0 since it has no buddies
	if (level == 0){
		free_block_lists[level] = address;
		return;
	}

	//finding the address of the buddy block
	int my_index  = index_in_level_of(address, level);
	long int buddy_address = buddy_address_of_index(my_index, level);
	printf("My index = %d\n", my_index);
	printf("buddy relative address = %d\n", buddy_address - (long int) start_address);
	//checking if the buddy is split or not, and if it is not split, then whether it is allocated or free
	long int * buddy_ptr = (long int *) buddy_address;	
	//it is at the same level, and is free
	if (buddy_ptr[0] == -level){
		ptr[0] = 0; // indicate that this block no longer exists

		//remove the buddy from the free blocks list at this level
		//special case for if the buddy was the 1st block in the list
		if (free_block_lists[level] == (long int)buddy_address){
			free_block_lists[level] = buddy_ptr[1];
			if(buddy_ptr[1] != (long int) NULL){
				long int* next_block = (long int *) buddy_ptr[1];
				next_block[2] = (long int) NULL;
			}
			
		} else {
			long int * prev = (long int *) buddy_ptr[2];
			prev[1] = (long int) buddy_ptr[1];
			if ( buddy_ptr[1] != (long int) NULL){
				long int * next = (long int *) buddy_ptr[1];
				next[2] = (long int) prev;
			}
		}

		long int * first_of_the_buddies = buddy_ptr;
		if (my_index % 2 == 0){
			first_of_the_buddies = ptr;
		}

		first_of_the_buddies[0] = level - 1; // this block will now be a block of the previous level
		first_of_the_buddies++; //to account for the word storing the level info
		bfree((void *) first_of_the_buddies);
	} else {
		ptr[0] = -level; // indicate that it is free at this level
		ptr[1] = free_block_lists[level]; // make its next ptr point to the first block in the list of free blocks at this level
		ptr[2] = (long int) NULL; // its prev ptr is NULL since it will be the first in the list of free blocks at this level
		free_block_lists[level] = address; // make it the 1st block
		if(ptr[1] != (long int) NULL){ // make the prev ptr of what used to be the 1st block, if it exists of course, point to it
			long int * used_to_be_first = (long int *) ptr[1];
			used_to_be_first[2] = address;
		}
	}
	
	return;
}

void bprint(void) {
	printf("%s","---->\tbprint called\n");
	

	long int * i = start_address;
	if (i[0] == 0 && free_block_lists[0] != (long int)NULL){
		printf("The chunk is completely free\n");
		return;
	}

	printf("%-20s%-25s%-10s%-25s\n", "Free/Allocated?", "Block Size", "Level", "Relative address");

	while ( (long int) i <  (long int) start_address + size) {
		long int level = i[0];
		if (level <= 0){
			printf("%-20s%-25d%-10ld%-25ld\n", "F", powers_of_two[-level], -level, (long int) i - (long int)start_address);
			i += powers_of_two[-level]/sizeof(long int);
		} else {
			printf("%-20s%-25d%-10ld%-25ld\n", "A", powers_of_two[level], level, (long int) i - (long int)start_address);
			i += powers_of_two[level]/sizeof(long int);
		}		
	}
	return;
}



int index_in_level_of(long int address, int level){
	return (address - (long int) start_address) / (size / (1<<level));
}




long int buddy_address_of_index(int index, int level){
	
	if (index % 2 == 0)
		index++;
	else
		index--;

	long int address = (long int) index * (long int) (size / (1<<level) ) + (long int) start_address; 

	return address;
} 









