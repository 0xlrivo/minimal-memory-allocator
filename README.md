# Minimal Memory Allocator

A super simple memory allocator library made for learning purposes, it exposes three functions:
1. void* heap_alloc(size_t size): allocates a chunk of size bytes and returns a void pointer to the beginning of it  
2. void heap_free(void *ptr): free the given chunk
3. void heap_print_chunks(): prints all of the chunks, you should run it to understand how it works

The main.c file already contain a basic test case that shows how the library behaves.
