#ifndef BUDDY_H
#define BUDDY_H

int binit(void *, int);
void *balloc(int);
void bfree(void *);
void bprint(void); 
long int allocate_at_level(int level);
#endif
