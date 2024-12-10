#include <stdio.h>

#include "cbox_camera.h"
#include "cbox_array.h"

int main() {
	cbox_camera_init();

	[[maybe_unused]]
	cbox_array_t *cameras = cbox_get_cameras();

	printf("cameras: %p\n", cameras);

	printf("count: %ld\n", cbox_get_array_size(cameras));

	cbox_free_cameras(cameras);
	return 0;
}
