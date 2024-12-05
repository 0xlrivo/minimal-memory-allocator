#include <stdio.h>
#include <string.h>
#include "alloc.h"

/* Testing my memory allocator  library  */
int main() {
	int *arr = (int*)heap_alloc(5 * sizeof(int));
	printf("%p\n", arr);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	arr[4] = 5;
	for(int i = 0; i < 5; i++) {
		printf("%d, ", arr[i]);
	}
	printf("\n");
	
	char *str = (char*)heap_alloc(10 * sizeof(char));
	strcpy(str, "AAAAAAAAA\0");
	printf("%s\n", str);
	
	printf("\nallocated heap:\n");
	heap_print_chunks();
	
	// free the array
	heap_free(arr);
	
	printf("\nafter first free:\n");
	heap_print_chunks();
	
	printf("\n after int[40] allocation:\n");
	int *arr2 = (int*)heap_alloc(40 * sizeof(int));
	heap_print_chunks();

	heap_free(str);

	printf("\nafter second free:\n");
	heap_print_chunks();

	// request the array again, should give me the same address
	arr = (int*)heap_alloc(30 * sizeof(int));
	printf("%p\n", arr);

	printf("\nafter final allocation:\n");
	heap_print_chunks();

	printf("\nfree the 120 byte chunk and coalesce with the final chunk:\n");
	heap_free(arr);
	heap_print_chunks();

	printf("\nfree the 160 byte chunk and coalesce the entire list:\n");
	heap_free(arr2);
	heap_print_chunks();

	return 0;
}
