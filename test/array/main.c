#include <stdio.h>

#include "minunit.h"

#include "cbox_array.h"

cbox_array_t *array = NULL;

MU_TEST(create_array) {
        array =
		cbox_create_array(5, sizeof(size_t));
	mu_check(array);
}

MU_TEST(array_add) {
	mu_check(array);

	size_t size = 50;

	for (size_t i = 0; i < size; i++) {
		cbox_array_add(array, &i);
	}

	mu_check(cbox_get_array_size(array) == size);

	for (size_t i = 0; i < size; ++i) {
		mu_check(*(size_t*)cbox_array_get_element(array, i) == i);
	}

	for (size_t i = -1; i >= -size; --i) {
		mu_check(*(size_t*)cbox_array_get_element(array, i) == size + i);
	}
}

MU_TEST(array_remove) {
	mu_check(array);

	size_t size = cbox_get_array_size(array);

	for (size_t i = 0; i < size; i++) {
		cbox_array_remove(array, 0);
	}

	mu_check(cbox_get_array_size(array) == 0);
}

MU_TEST(destroy_array) {
	cbox_destroy_array(array);
}

MU_TEST_SUITE(s1) {
	MU_RUN_TEST(create_array);
	MU_RUN_TEST(array_add);
	MU_RUN_TEST(array_remove);
	MU_RUN_TEST(destroy_array);
}

MU_TEST_SUITE(s2) {
	MU_RUN_TEST(create_array);
	MU_RUN_TEST(array_add);
	MU_RUN_TEST(array_remove);
	MU_RUN_TEST(destroy_array);
}

int main() {
	MU_RUN_SUITE(s1);
	MU_RUN_SUITE(s2);
	MU_REPORT();
	return MU_EXIT_CODE;
}
