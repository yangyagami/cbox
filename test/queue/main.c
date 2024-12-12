#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "minunit.h"

#include "cbox_queue.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

cbox_queue_t *queue = NULL;

int arr[] = {
	1, 2, 3, 4, 5, 6,
};

MU_TEST(create_queue) {
        queue = cbox_create_queue(sizeof(int));
	assert(queue);

	mu_check(cbox_queue_empty(queue));
	mu_check(!cbox_queue_fill(queue));
}

MU_TEST(destroy_queue) { cbox_destroy_queue(queue); }

MU_TEST(enqueue) {
	mu_check(cbox_queue_empty(queue));
	mu_check(!cbox_queue_fill(queue));

	for (size_t i = 0; i < ARRAY_SIZE(arr); ++i) {
		cbox_queue_enqueue(queue, arr + i);
	}

	mu_check(!cbox_queue_empty(queue));
	mu_check(!cbox_queue_fill(queue));
}

MU_TEST(dequeue) {
	mu_check(!cbox_queue_empty(queue));
	mu_check(!cbox_queue_fill(queue));

	size_t i = 0;
	while (!cbox_queue_empty(queue)) {
		int *tmp = cbox_queue_dequeue(queue);
		mu_check(*tmp == arr[i++]);
		free(tmp);
	}

	mu_check(cbox_queue_empty(queue));
	mu_check(!cbox_queue_fill(queue));
}

MU_TEST_SUITE(test1) {
	MU_RUN_TEST(create_queue);
	MU_RUN_TEST(enqueue);
	MU_RUN_TEST(dequeue);
	MU_RUN_TEST(destroy_queue);
}

int main() {
	MU_RUN_SUITE(test1);
	MU_REPORT();
	return MU_EXIT_CODE;
}
