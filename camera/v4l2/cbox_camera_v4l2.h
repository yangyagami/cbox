#ifndef CBOX_CAMERA_V4L2_CAMERA_V4L2_H_
#define CBOX_CAMERA_V4L2_CAMERA_V4L2_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "cbox_camera.h"
#include "cbox_array.h"

struct cbox_v4l2_camera_handler {
	char device_path[512];
	int video_fd;
};
typedef struct cbox_v4l2_camera_handler cbox_v4l2_camera_handler_t;

bool cbox_v4l2_try_ioctl(int fd, int request, void *output);
cbox_array_t *cbox_v4l2_get_cameras();
bool cbox_v4l2_open_camera(cbox_camera_t *camera, cbox_camera_param_t *param);
void cbox_v4l2_free_cameras();

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // CBOX_CAMERA_V4L2_CAMERA_V4L2_H_
