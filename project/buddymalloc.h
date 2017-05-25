#ifndef BUDDY_MALLOC 
#define BUDDY_MALLOC
#define MAX_POOL (20)
#define BLOCK_SIZE sizeof(struct block_info)
#include <stddef.h>

typedef struct block_info block_info;

struct block_info {
	size_t kval;
	struct block_info* next;
	struct block_info* prev;
	int free;
};


void* malloc(size_t);

void free(void*);

void* realloc(void*, size_t);
 
void* calloc(size_t, size_t);


#endif
