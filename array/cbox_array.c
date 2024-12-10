#include "cbox_array.h"

#include <memory.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

struct cbox_array {
	size_t element_size;
        char *buffer;
	size_t capacity;
        size_t size;
};

cbox_array_t *cbox_create_array(size_t size, size_t element_size) {
	cbox_array_t *array = NULL;
	array = calloc(1, sizeof(*array));
	assert(array);

	array->element_size = element_size;
	array->capacity = size;
	if (size > 0) {
		array->buffer = calloc(size, element_size);
	}

	return array;
}

void cbox_destroy_array(cbox_array_t *array) {
	assert(array);

	free(array->buffer);
	free(array);
}

static int cbox_array_get_index(cbox_array_t *array, int index) {
	int idx = index < 0 ? (int)(array->size + index) : index;
	assert(idx >= 0 && (size_t)idx < array->size);

	return idx;
}

void *cbox_array_get_element(cbox_array_t *array, int index) {
	assert(array && array->buffer);

        int idx = cbox_array_get_index(array, index);

	return array->buffer + idx * array->element_size;
}

void cbox_array_add(cbox_array_t *array, void *element) {
	assert(array);

	if (!array->buffer) {
		array->capacity = 2;
		array->buffer = calloc(array->capacity,
				array->element_size);
	}

	if (array->size >= array->capacity) {
		array->capacity *= 2;
		array->buffer = realloc(array->buffer,
					array->capacity * array->element_size);
	}

	memcpy(array->buffer + (array->size++ * array->element_size),
		element,
		array->element_size);
}

void cbox_array_remove(cbox_array_t *array, int index) {
	assert(array && array->buffer && array->size > 0);
	int idx = cbox_array_get_index(array, index);

	if ((size_t)idx + 1 < array->size) {
		memmove(array->buffer + idx * array->element_size,
			array->buffer + (idx + 1) * array->element_size,
			(array->size - idx - 1) * array->element_size);
	}

	array->size--;
}


size_t cbox_get_array_size(cbox_array_t *array) {
	assert(array);
	return array->size;
}
