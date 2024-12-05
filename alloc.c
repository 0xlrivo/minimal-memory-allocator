#include "alloc.h"
#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>

// head of the free list
node_t *free_list = NULL;

void* heap_alloc(size_t size) {
	// initialize the library before the first call
	if(free_list == NULL) {
		// request the OS for 4096 bytes 
		free_list = mmap(
			NULL,  						// let the OS decide where to place this request in memory 
			4096,						// how many bytes we're asking the OS for 
			PROT_READ | PROT_WRITE,		// allow reading and writing on this memory 
			MAP_PRIVATE | MAP_ANON,		// this memory will be private to this process, no sharing
			-1,							// since we're not mapping a file
			0							// offset 
		);	
		
		// if mmap succeded configure the free_list
		free_list->is_allocated = 0;
		free_list->size = 4096 - sizeof(node_t);
		free_list->next = NULL;
	}
	
	// search for a free and large enough node (first-fit  algorithm)
	node_t *curr = free_list;
	while(curr != NULL) {
		if(!curr->is_allocated && curr->size >= size + sizeof(node_t))
			break;
		curr = curr->next;
	}
	
	// get the address of the next chunk (cast in size_t for convenience)
	size_t next_chunk = (size_t)curr + size + sizeof(node_t);
	
	// initialize  the next chunk
	((node_t*)next_chunk)->size = curr->size - size - sizeof(node_t);
	((node_t*)next_chunk)->is_allocated = 0;
	((node_t*)next_chunk)->next = NULL;

	// update the curr chunk
	curr->size = size; 
	curr->is_allocated = 1;
	curr->next = (node_t*)next_chunk;

	// in pointer arithmetics  +1 means +sizeof(self) , 20 bytes in this case
	return (void*)(curr + 1);
}

void heap_free(void *ptr) {
	node_t *chunk = (node_t*)(ptr-sizeof(node_t));
	
	// reject non-allocated  chunks
	if(!chunk->is_allocated)
		return;
	
	// mark this chunk as not-allocated
	chunk->is_allocated = 0;

	// coalesce the free_list
	chunk = free_list;
	while(chunk->next != NULL) {
		// if we find two consecutive  free chunks
		if(!chunk->is_allocated && !chunk->next->is_allocated) {
			chunk->size += chunk->next->size + sizeof(node_t);
			chunk->next = chunk->next->next;
			// don't update chunk in this case, so that we can coalesce other free blocks after the two we've just coalesced
		}
		else {
			chunk = chunk->next;
		}
	}

	return;
}

void heap_print_chunks() {
	int i = 0;
	node_t* curr = free_list;
	while(curr != NULL) {
		printf("-- Chunk %d ---\naddr: %p\tis_alloc: %d\tsize: %d\tnext: %p\n", i++, curr, curr->is_allocated, curr->size, curr->next);
		curr = curr->next;
	}
}
