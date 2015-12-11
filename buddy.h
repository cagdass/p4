#ifndef BUDDY_H
#define BUDDY_H

int binit(void *, int);
void *balloc(int);
void bfree(void *);
void bprint(void); 
long int allocate_at_level(int level);
int index_in_level_of(long int address, int level);
long int buddy_address_of_index(int index, int level);
#endif
