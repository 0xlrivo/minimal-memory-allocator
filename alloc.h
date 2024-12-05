// @author 0xlrivo
#include <stddef.h>

/* entries of the free list */
typedef struct __node_t {
	short is_allocated;			// 0 = free  1 = allocated
	size_t size;  				// size in byte of this chunk
	struct __node_t *next;		// next node in the free list 
} node_t;

// debug function that prints all of the chunks
void heap_print_chunks();

// request size bytes of memory
void* heap_alloc(size_t size);

// free the provided memory block
void heap_free(void *ptr);
