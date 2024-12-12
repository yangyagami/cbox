#include "cbox_queue.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#define CBOX_QUEUE_MAX_SIZE (256)

struct cbox_queue {
	int front;
	int back;
	void *buffer[CBOX_QUEUE_MAX_SIZE];
	size_t element_size;
};

cbox_queue_t *cbox_create_queue(size_t element_size) {
	cbox_queue_t *queue = NULL;
	queue = calloc(1, sizeof(*queue));

	assert(queue);

	queue->element_size = element_size;

	return queue;
}

void cbox_destroy_queue(cbox_queue_t *queue) {
	for (int i = 0; i < CBOX_QUEUE_MAX_SIZE; ++i) {
		free(queue->buffer[i]);
	}
	free(queue);
}

void cbox_queue_enqueue(cbox_queue_t *queue, void *element) {
	assert(!cbox_queue_fill(queue));

	void *tmp = NULL;
	tmp = calloc(1, queue->element_size);
	memcpy(tmp, element, queue->element_size);

	queue->buffer[queue->back] = tmp;
	queue->back = (queue->back + 1) % CBOX_QUEUE_MAX_SIZE;
}

void *cbox_queue_dequeue(cbox_queue_t *queue) {
	assert(!cbox_queue_empty(queue));

	void *tmp = queue->buffer[queue->front];
	queue->front = (queue->front + 1) % CBOX_QUEUE_MAX_SIZE;

	return tmp;
}

bool cbox_queue_empty(cbox_queue_t *queue) {
	assert(queue);

	return queue->back == queue->front;
}

bool cbox_queue_fill(cbox_queue_t *queue) {
	return ((queue->back + 1) % CBOX_QUEUE_MAX_SIZE) == queue->front;
}

#undef CBOX_QUEUE_MAX_SIZE
