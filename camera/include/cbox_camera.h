#ifndef CBOX_CBOX_CAMERA_H_
#define CBOX_CBOX_CAMERA_H_

#ifdef __cpluscplus
extern "C" {
#endif  // __cpluscplus

#include <stdbool.h>
#include <unistd.h>

#include "cbox_array.h"

struct cbox_camera_buffer;
typedef struct cbox_camera_buffer cbox_camera_buffer_t;

enum cbox_camera_frame_pixel_format {
	MJPEG = 0,
	YUYV,
};
typedef enum cbox_camera_frame_pixel_format cbox_camera_frame_pixel_format_t;

struct cbox_camera_param {
	cbox_camera_frame_pixel_format_t pixel_fmt;
	int width;
	int height;
	int fps;
};
typedef struct cbox_camera_param cbox_camera_param_t;

enum cbox_camera_error {
	CBOX_CAMERA_NO_ERROR = 0,
	CBOX_CAMERA_INVALID_PARAM,
	CBOX_CAMERA_NO_SUCH_DEVICE,
	CBOX_CAMERA_OVER_TIME,
	CBOX_CAMERA_UNKNOW,
};
typedef enum cbox_camera_error cbox_camera_error_t;

struct cbox_camera;
typedef struct cbox_camera cbox_camera_t;

cbox_camera_error_t cbox_camera_error();
const char *cbox_camera_strerror();

extern bool cbox_camera_init();
extern void cbox_camera_destroy();

extern void cbox_camera_iterate();

extern cbox_array_t *cbox_get_cameras();

extern bool cbox_camera_open(cbox_camera_t *camera, cbox_camera_param_t *param);
extern bool cbox_camera_close(cbox_camera_t *camera);

#ifdef __cpluscplus
}
#endif  // __cpluscplus

#endif  // CBOX_CBOX_CAMERA_H_
