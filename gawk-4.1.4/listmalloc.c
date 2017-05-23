#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define META_SIZE sizeof(struct block_info)
void *head = NULL;
typedef struct block_info block_info;

struct block_info {
	size_t size;
	struct block_info *next;
	int is_free;
};

block_info* find_available_block(block_info **last, size_t size) {
	block_info* current = head;
	while(current && !(current->is_free && current->size >= size)) {
		//printf("looping through list\n");
		*last = current;
		current = current->next;
	} 
	return current;
}

block_info *req_space(block_info* last, size_t size) {
	block_info *block;
	block = sbrk(0);
	void *req = sbrk(size + META_SIZE);
	if (req == (void*) -1) {
		//printf("sbrk failed \n");
		return NULL;
	}
	//printf("sbrk used\n");
	if (last) {
		last->next = block;
	}

	block->size = size;
	block->next = NULL;
	block->is_free = 0;
	return block;
}

void *malloc(size_t size) {
	block_info *block;

	if (!head) {
		block = sbrk(0);
		void *req = sbrk(size + META_SIZE);
		if (req == (void*) -1) {
			return NULL;
		}
		//printf("first sbrk used\n");

		block->size = size;
		block->next = NULL;
		block->is_free = 0;

		head = block;
	} else {
		block_info *last = head;
		block = find_available_block(&last, size);
		if (!block) {
			//printf("did not find available block\n");
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
	block_info* block = (block_info*)ptr - 1;
	block->is_free = 1;
	//printf("%d\n",block->size);
	//printf("memory set free \n");
	return;
}

void* realloc(void* ptr, size_t size) {
    if (!ptr) {
    // NULL ptr. realloc should act like malloc.
        return malloc(size);
    }

    struct block_info* block = (block_info*)ptr - 1;
    if (block->size >= size) {
    // We have enough space. Could free some once we implement split.
        return ptr;
    } 

  // Need to really realloc. Malloc new space and free old space.
  // Then copy old data to new space.
    void *new_ptr;
    new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL; // TODO: set errno on failure.
    }
    memcpy(new_ptr, ptr, block->size);
    free(ptr);
    return new_ptr;
    }

/*void* calloc(size_t size) {

}

int main()
{
    int* ptr1 = malloc(8);
    assert(ptr1);
	int* ptr2 = malloc(8);
	assert(ptr2);
	free(ptr1);
	int* ptr3 = malloc(4);
	assert(ptr3);
}*/
