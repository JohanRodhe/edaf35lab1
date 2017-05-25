#include <unistd.h>
#include <stdio.h>
#include "buddymalloc.h"

static block_info* freelist[MAX_POOL + 1];
static block_info* start = NULL;


block_info* split_block(size_t kval)
{
	block_info* block;
	block_info* new_block;
	int i;

	for (i = kval; i <= MAX_POOL; ++i) {
		if (freelist[i]) {
			block = freelist[i];
			freelist[i] = block->next;
			break;
		}	
	}
	if (!block) {
		return NULL;
	}
	while (block->kval != kval) {
		block->kval -= 1;
		new_block = (void*) block + (1 << block->kval);
		
		new_block->kval = block->kval;
		new_block->next = freelist[block->kval];
		new_block->prev = NULL;
		new_block->free = 1;
		
		if (new_block->next) {
			new_block->next->prev = new_block;
		}
		freelist[block->kval] = new_block;
	}	
	block->free = 0;
	return block;
}


size_t find_next_kval(size_t size) 
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
	size_t kval;
	block_info* block;

	if (!start) {
		start = sbrk(0);
		req = sbrk(1 << MAX_POOL);
		if (req == (void*) -1) {
			return NULL;
		}
		start->kval = MAX_POOL;
		start->next = NULL;
		start->prev = NULL;
		start->free = 1;
		
		freelist[MAX_POOL] = start;			
	}

	kval = find_next_kval(size);		
	
	if (freelist[kval] != NULL) {
		block = freelist[kval];	
		block->free = 0;
		if (block->next) {
			block->next->prev = NULL;	
		}
		freelist[kval] = block->next;
		
		return (block + 1);
	} 
	block = split_block(kval);
	if (block) {
		return (block + 1);
	}	
	return NULL;
}


void free(void* ptr) 
{
	block_info* block;
	block_info* buddy;

	if (!ptr) {
		return;
	}	
	block = ptr - BLOCK_SIZE;
	if (block->kval < MAX_POOL){
		buddy = (void*) start + ((block - start) ^ (1<<block->kval));
	} 
	if (buddy && buddy->free == 1 && block->kval == buddy->kval) {
		if (buddy->prev) {
			buddy->prev->next = buddy->next;
			if (buddy->next) {
				buddy->next->prev = buddy->prev;
			}
		} else {
			freelist[buddy->kval] = buddy->next;
			if (buddy->next) {
				buddy->next->prev = NULL;
			}	
		}
		if (buddy < block) {
			block = buddy;
		}
	}
	block->next = freelist[block->kval];
	block->prev = NULL;
	block->free = 1;

	if (block->next) {
		block->next->prev = block;
	}
	freelist[block->kval] = block;	
	
}


void* realloc(void* ptr, size_t new_size) 
{	
	block_info* block;
	char* tmp;
	int i;

	if (!ptr) {
		return malloc(new_size);
	}
	block = (block_info*) ptr - 1;

	if ((1 << block->kval) - BLOCK_SIZE >= new_size){
		return ptr;
	} 
	//require new block and copy elements
	char* new_ptr = malloc(new_size);
	if (new_ptr) {
		tmp = ptr;
		for (i = 0; i < (1 << block->kval) - BLOCK_SIZE; i++){
			new_ptr[i] = tmp[i];
		}
		free(ptr);
    	return new_ptr;
    }	
	return NULL;
}


void* calloc(size_t num_elems ,size_t elem_size) 
{
	size_t size;
	char* ptr;
	int i;

	size = num_elems * elem_size;
	ptr = malloc(size);
	if (ptr) {
		//fill elements with 0
		for (i = 0; i < size; i++) {
			ptr[i] = 0;
		}
	}
	return ptr;
}
