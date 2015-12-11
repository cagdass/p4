#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "buddy.h"

/*
	This is a modified version of app.c, modified to take command arguments so 
	that it can be tested very quickly with a simple bash script.
*/

clock_t begin, end;
double time_spent;

int main(int argc, char *argv[])
{
	begin = clock();
	void *chunkptr;
	void *endptr;
	char *charptr;
	int ret;
	int i;
	int size;
	int num_of_objects;
	int blocksize;
	// void *x1, *x2, *x3, *x4;	// object pointers
	// a list of object pointers
	void ** object_pointers;
	// a list of block sizes

	if (argc < 3) {
		printf("usage: app <size in KB>\n");
		exit(1);
	}

	size = atoi(argv[1]);
	blocksize = atoi(argv[2]);
	num_of_objects = (size * 1024) / 256;
	object_pointers = (void**)malloc(sizeof(void*) * num_of_objects);


	// allocate a chunk 
	chunkptr = sbrk(0);	// end of data segment
	sbrk(size * 1024);	// extend data segment by indicated amount (bytes)
	endptr = sbrk(0);	// new end of data segment

	printf("chunkstart=%lx, chunkend=%lx, chunksize=%lu bytes\n",
	       (unsigned long)chunkptr,
	       (unsigned long)endptr, (unsigned long)(endptr - chunkptr));

	//test the chunk 
	printf("---starting testing chunk\n");
	charptr = (char *)chunkptr;
	for (i = 0; i < size; ++i)
		charptr[i] = 0;
	printf("---chunk test ended - success\n");

	ret = binit(chunkptr, size);
	if (ret == -1) {
		printf("could not initialize \n");
		exit(1);
	}

	// bprint();
	for(i = 0; i < num_of_objects; i++){
		object_pointers[i] = balloc(blocksize);
		// bprint();
	}
	for(i = 0; i < num_of_objects; i++){
		bfree(object_pointers[i]);
		// bprint();
	}

	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time spent: %f ms\n", time_spent);

	return 0;
}
