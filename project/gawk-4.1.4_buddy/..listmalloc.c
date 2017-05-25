#include <unistd.h>
#include "listmalloc.h"


void* head = NULL;
typedef struct block_info block_info;

	/*
	finds the next available block with enough size
	*/
block_info* find_available_block(block_info** last, size_t size) 
{
	block_info* current;

	current = head;
	while(current && !(current->free && current->size >= size)) {
		*last = current;
		current = current->next;
	} 
	return current;
}

	/*
	requests space at the end of the list
	*/
block_info* req_space(block_info* last, size_t size) 
{
	block_info* block;
	void* req;

	block = sbrk(0);
	req = sbrk(size + BLOCK_SIZE);
	if (req == (void*) -1) {
		return NULL;
	}
	if (last) {
		last->next = block;
	}
	if (!head) {
		head = block;
	}

	block->size = size;
	block->next = NULL;
	block->prev = last;
	block->free = 0;
	return block;
}

	/*
	merges block with its next block
	*/
void merge_with_next(block_info* block)
{
	block->size += block->next->size + BLOCK_SIZE;
	block->next = block->next->next;
	if (block->next){
		block->next->prev = block;	
	}
}

	/*
	splits block into two
	*/
void split_block(block_info* block, size_t size)
{
	block_info* new_block;

	new_block = (void*)block + size + BLOCK_SIZE;
	new_block->size = block->size - size - BLOCK_SIZE;
	new_block->next = block->next;
	new_block->prev = block;
	new_block->free = 1;
	block->next = new_block;
	block->size = size;
	if (new_block->next) {
		new_block->next->prev = new_block;
		if (new_block->next->free){
			merge_with_next(new_block);
		}
	}
	
}


void* malloc(size_t size) 
{
	block_info* last;
	block_info* block;
	
	last = head;
	block = find_available_block(&last, size);
	
	if (!block) {
		block = req_space(last, size);
		if (!block) {
			return NULL;
		}
	} else {
		if (block->size > ((size + BLOCK_SIZE) * 2)){
			split_block(block, size);
		}
		block->free = 0;
	}
	return (block + 1);
}


void free(void* ptr) 
{
	block_info* block;

	if (!ptr) {
		return;
	}
	block = (block_info*) ptr - 1;
	block->free = 1;
	if (block->next && block->next->free) {
		merge_with_next(block);
	}
	if (block->prev && block->prev->free) {
		merge_with_next(block->prev);
		block = block->prev;
	}
	if (!block->next){
		if (block == head){
			head = NULL;
		} else {
			block->prev->next = NULL;
		}
		if (sbrk(-block->size - BLOCK_SIZE) == (void*) -1){
			// TODO: set errno on failure.
			return;
		}
	}
	return;
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

	if (block->size >= new_size){
		return ptr;
	} 
	if (!block->next){
		void* req = sbrk(new_size - block->size);
		if (req == (void*) -1) {
			return NULL;
		}
		block->size = new_size;
		return ptr;
	}
	if (block->next->free && block->size + block->next->size >= new_size) {
		merge_with_next(block);
		return ptr;
	}
	//require new block and copy elements
	char* new_ptr = malloc(new_size);
	if (new_ptr) {
		tmp = ptr;
		for (i = 0; i < block->size; i++){
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
	if (ptr){
		//fill elements with 0
		for (i = 0; i < size; i++){
			ptr[i] = 0;
		}
	}
	return ptr;
}
