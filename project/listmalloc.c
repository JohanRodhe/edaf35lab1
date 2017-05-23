#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "listmalloc.h"

void* head = NULL;

block_info* find_available_block(block_info** last, size_t size) {
	block_info* current = head;
	while(current && !(current->is_free && current->size >= size)) {
		*last = current;
		current = current->next;
	} 
	return current;
}

block_info* req_space(block_info* last, size_t size) {
	block_info *block;
	block = sbrk(0);
	void* req = sbrk(size + BLOCK_SIZE);
	if (req == (void*) -1) {
		return NULL;
	}
	if (last) {
		last->next = block;
	}

	block->size = size;
	block->next = NULL;
	block->is_free = 0;
	return block;
}

void* malloc(size_t size) {
	block_info* block;

	if (!head) {
		block = sbrk(0);
		void* req = sbrk(size + BLOCK_SIZE);
		if (req == (void*) -1) {
			return NULL;
		}
		block->size = size;
		block->next = NULL;
		block->is_free = 0;

		head = block;
	} else {
		block_info* last = head;
		block = find_available_block(&last, size);
		if (!block) {
			block = req_space(last, size);
			if (!block) {
				return NULL;
			}
		} else {
			block->is_free = 0;
		}
	}
	return (block + 1);
}



void free(void* ptr) {
	if (!ptr) {
		return;
	}
	block_info* block = (block_info*) ptr - 1;
	if (block == head){
		block->is_free = 1;
		//sbrk(-block->size - BLOCK_SIZE)
		//head = NULL;
	} else {
		block_info* prev = head;
		block_info* next = block->next;
		while(prev->next != block) {
			prev = prev->next;
		}
		block->is_free = 1;
	}
	return;
}

void* realloc(void* ptr, size_t new_size) {
	
}

void* calloc(size_t size) {

}
	