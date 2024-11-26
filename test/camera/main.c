#include <stdio.h>

#define CBOX_CAMERA_IMPLEMENTATION
#define CBOX_CAMERA_BACKEND_V4L2
#include "camera/camera.h"

int main() {
	int count = 0;
	[[maybe_unused]]
	cbox_camera_t **cameras = cbox_get_cameras(&count);

	printf("count: %d\n",count);

	cbox_free_cameras(cameras);
	return 0;
}
