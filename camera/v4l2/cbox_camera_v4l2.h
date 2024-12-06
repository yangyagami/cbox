#ifndef CBOX_CAMERA_V4L2_CAMERA_V4L2_H_
#define CBOX_CAMERA_V4L2_CAMERA_V4L2_H_

#include "cbox_camera.h"

struct cbox_v4l2_camera_handler {
	char device_path[512];
	char actual_device_path[256];
	int fd;
};
typedef struct cbox_v4l2_camera_handler cbox_v4l2_camera_handler_t;

bool cbox_v4l2_try_ioctl(int fd, int request, void *output);
cbox_camera_t **cbox_v4l2_get_cameras(int *count);
cbox_camera_t **cbox_v4l2_free_cameras(cbox_camera_t **cameras);

#endif  // CBOX_CAMERA_V4L2_CAMERA_V4L2_H_
