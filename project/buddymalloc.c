#include <unistd.h>
#include <stdio.h>
#include "buddymalloc.h"

static block_info* freelist[MAX_POOL + 1];
static block_info* start = NULL;

	/*
	finds the next available block with enough size
	*/
block_info* find_available_block(size_t size) 
{

}

	/*
	merges block with its buddy block
	*/
void merge_with_buddy(block_info* block)
{


}

	/*
	splits block into two
	*/
block_info* split_block(size_t log_two)
{
	block_info* block;
	block_info* new_block;
	int i;

	for (i = log_two; i <= MAX_POOL; ++i){
		if (freelist[i]){
			block = freelist[i];
			freelist[i] = block->next;
			break;
		}	
	}
	if (!block) {
		return NULL;
	}
	while(block->kval != log_two){
		block->kval -= 1;
		new_block = (void*) block + (1 << block->kval);
		new_block->kval = block->kval;
		new_block->free = 1;
		new_block->next = freelist[block->kval];
		if (new_block->next) {
			new_block->next->prev = new_block;
		}
		freelist[block->kval] = new_block;
	}	
	block->free = 0;
	return block;
}


size_t find_closest_log(size_t size) 
{
	size_t k = 2;
	while ((1 << k) < (size + BLOCK_SIZE)) {
		k++;
	}
	return k;

}

void* malloc(size_t size) 
{
	void* req;
	size_t log_two;
	if (!start) {
		start = sbrk(0);
		req = sbrk(1 << MAX_POOL);
		if (req == (void*) -1){
			return NULL;
		}
		start->free = 1;
		start->next = NULL;
		start->prev = NULL;
		start->kval = MAX_POOL;
		freelist[MAX_POOL] = start;			
	}

	log_two = find_closest_log(size);		
	
	if (freelist[log_two] != NULL) {
		block_info *block = freelist[log_two];	
		block->free = 0;
		if (block->next) {
			block->next->prev = NULL;	
		}
		freelist[log_two] = block->next;
		
		return (block + 1);
	} else {
		return split_block(log_two);
	}	
}


void free(void* ptr) 
{
	if (!ptr) {
		return;
	}
	block_info* block;
	block_info* buddy;
	
	block = ptr - BLOCK_SIZE;
	buddy = (void*) start + ((block - start) ^ (1<<block->kval));
	if (buddy->free == 1 && block->kval == buddy->kval){
		if (buddy->prev) {
			buddy->prev->next = block->next;
			if (buddy->next) {
				buddy->next->prev = buddy->prev;
			}
		} else {
			freelist[buddy->kval] = buddy->next;
			if (buddy->next) {
				buddy->next->prev = NULL;
			}	
		}

	} else {
		block->free = 1;
		block->next = freelist[block->kval];
		if (block->next) {
			block->next->prev = block;
		}
		freelist[block->kval] = block;	
	}
}


void* realloc(void* ptr, size_t new_size) 
{	
	
}


void* calloc(size_t num_elems ,size_t elem_size) 
{
	size_t size;
	char* ptr;
	int i;

	size = num_elems * elem_size;
	ptr = malloc(size);
	if (ptr){
		//fill elements with 0
		for (i = 0; i < size; i++){
			ptr[i] = 0;
		}
	}
	return ptr;
}
