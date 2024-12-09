#ifndef CBOX_CBOX_CAMERA_H_
#define CBOX_CBOX_CAMERA_H_

#ifdef CBOX_CAMERA_ENABLE_LOG
#define CBOX_CAMERA_LOG_ERROR(msg, ...) \
	do { \
		fprintf(stderr, "\x1B[31mERROR: [%s][%d] " msg "\e[0m\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	} while (0)

#define CBOX_CAMERA_LOG_INFO(msg, ...) \
	do { \
		fprintf(stdout, "INFO: [%s][%d] " msg "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	} while (0)

#define CBOX_CAMERA_LOG_WARN(msg, ...) \
	do { \
		fprintf(stderr, "\x1B[32mWARN: [%s][%d] " msg "\e[0m\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	} while (0)
#else
#define CBOX_CAMERA_LOG_ERROR(msg, ...)
#define CBOX_CAMERA_LOG_INFO(msg, ...)
#define CBOX_CAMERA_LOG_WARN(msg, ...)
#endif  // CBOX_CAMERA_ENABLE_LOG

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
	int width;
	int height;
	cbox_camera_frame_pixel_format_t pixel_fmt;
};
typedef struct cbox_camera_param cbox_camera_param_t;

enum cbox_camera_error {
	CBOX_CAMERA_UNKNOWN = 0,
	CBOX_CAMERA_INVALID_PARAM,
	CBOX_CAMERA_NO_SUCH_DEVICE,
	CBOX_CAMERA_OVER_TIME,
};
typedef enum cbox_camera_error cbox_camera_error_t;

struct cbox_camera;
typedef struct cbox_camera cbox_camera_t;

/* cbox_camera_t *cbox_camera_create(); */
/* void cbox_camera_destory(cbox_camera_t *camera); */

cbox_camera_error_t cbox_camera_error();
const char *cbox_camera_strerror();

extern bool cbox_camera_init();
extern void cbox_camera_destroy();

extern bool cbox_camera_iterate();

extern cbox_array_t *cbox_get_cameras();
extern void cbox_free_cameras(cbox_array_t *cameras);

// bool cbox_camera_open(cbox_camera_t *camera, cbox_camera_param_t *param);

#ifdef __cpluscplus
}
#endif  // __cpluscplus

#endif  // CBOX_CBOX_CAMERA_H_
