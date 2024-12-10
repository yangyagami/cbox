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

MU_TEST(array_remove_reverse) {
	mu_check(array);

	// [0 ... 49]
	// 1:
	//    [ 0 ... 46 47 49 ]
	// 2:
	//    [ 0 ... 42 43 44 45 46 49 ]
	// 3:
	//    [ 0 ... 42 43 44 45 49 ]
	// 4:
	//    [ 0 ... 42 43 44 49 ]
	// 5:
	//    [ 0 ... 42 43 49 ]
	for (int i = 0; i < 5; i++) {
		cbox_array_remove(array, -2);
	}

	size_t tmp[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 49
	};

	for (size_t i = 0; i < 45; i++) {
		mu_check(*(size_t*)cbox_array_get_element(array, i) == tmp[i]);
	}
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
	MU_RUN_TEST(array_remove_reverse);
	MU_RUN_TEST(destroy_array);
}

int main() {
	MU_RUN_SUITE(s1);
	MU_RUN_SUITE(s2);
	MU_REPORT();
	return MU_EXIT_CODE;
}
