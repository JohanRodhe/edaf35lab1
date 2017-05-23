#ifndef LIST_MALLOC 
#define LIST_MALLOC

#include <stddef.h>

#define BLOCK_SIZE sizeof(struct block_info)

typedef struct block_info block_info;

struct block_info {
	size_t size;
	struct block_info* next;
	int is_free;
};

block_info* find_available_block(block_info**, size_t);

block_info* req_space(block_info*, size_t);

void *malloc(size_t);

void free(void*);

void* realloc(void*, size_t);

void* calloc(size_t);

#endif