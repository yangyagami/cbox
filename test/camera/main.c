#include <stdio.h>

#include "cbox_camera.h"
#include "cbox_array.h"

int main() {
	cbox_camera_init();

	// Don't write it manually
	cbox_array_t *cameras = cbox_get_cameras();

	size_t size = cbox_get_array_size(cameras);
	printf("count: %ld\n", size);

	for (size_t i = 0; i < size; ++i) {
		cbox_camera_t *camera = *(cbox_camera_t**)cbox_array_get_element(
			cameras, i);
		cbox_camera_open(camera, NULL);
	}

	cbox_camera_destroy();

	return 0;
}
