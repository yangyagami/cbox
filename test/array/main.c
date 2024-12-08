#include <stdio.h>

#include "cbox_array.h"

int main() {
	cbox_array_t *array = cbox_create_array(5, sizeof(int));

	for (int i = 0; i < 10; ++i) {
		cbox_array_add(array, &i);
	}

	printf("Array: ");
	for (int i = 0; i < 10; ++i) {
		printf("%d ", *(int *)cbox_array_get_element(array, i));
	}
	printf("\n");

	printf("Len: %ld\n", cbox_get_array_size(array));

	cbox_array_remove(array, -1);

	printf("After remove array: ");
	for (int i = 0; (size_t)i < cbox_get_array_size(array); ++i) {
		printf("%d ", *(int *)cbox_array_get_element(array, i));
	}
	printf("\n");

	printf("Len: %ld\n", cbox_get_array_size(array));

	cbox_destroy_array(array);
	return 0;
}
