#ifndef BUDDY_MALLOC 
#define BUDDY_MALLOC

#define BLOCK_SIZE sizeof(struct block_info)

typedef struct block_info block_info;

struct block_info {
	struct block_info *next;
	struct block_info *prev;
	int free;
};

void* malloc(size_t);

void free(void*);

void* realloc(void*, size_t);
 
void* calloc(size_t, size_t);

#endif