#include <unistd.h>
#include "buddymalloc.h"

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
void split_block(block_info* block, size_t size)
{

}


void* malloc(size_t size) 
{

}


void free(void* ptr) 
{
	
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
