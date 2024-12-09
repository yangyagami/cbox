#ifndef CBOX_CAMERA_V4L2_CAMERA_V4L2_H_
#define CBOX_CAMERA_V4L2_CAMERA_V4L2_H_

#include "cbox_camera.h"
#include "cbox_array.h"

struct cbox_v4l2_camera_handler {
	char device_path[512];
	int fd;
};
typedef struct cbox_v4l2_camera_handler cbox_v4l2_camera_handler_t;

bool cbox_v4l2_try_ioctl(int fd, int request, void *output);
cbox_array_t *cbox_v4l2_get_cameras();
void cbox_v4l2_free_cameras(cbox_array_t *cameras);

#endif  // CBOX_CAMERA_V4L2_CAMERA_V4L2_H_
