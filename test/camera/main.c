#include <stdio.h>

#include "cbox_camera.h"

int main() {
	int count = 0;

	cbox_camera_init();

	[[maybe_unused]]
	cbox_camera_t **cameras = cbox_get_cameras(&count);

	printf("count: %d, cameras: %p\n",count, cameras);

	cbox_free_cameras(cameras);
	return 0;
}
