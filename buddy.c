

#include <stdlib.h>
#include <stdio.h>


// printfs are for debugging; remove them when you use/submit your library


int binit(void *chunkpointer, int chunksize)
{

  printf ("binit called\n"); 

  return (0); // if success
}

void *balloc (int objectsize)
{

  printf ("balloc called\n"); 

  return (NULL); // if not success
}


void bfree(void *objectptr)
{

  printf ("bfree called\n"); 

  return; 
}


