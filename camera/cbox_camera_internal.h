#ifndef CBOX_CAMERA_CBOX_CAMERA_INTERNAL_H_
#define CBOX_CAMERA_CBOX_CAMERA_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "cbox_camera.h"

#ifdef CBOX_CAMERA_ENABLE_LOG
#define CBOX_CAMERA_LOG_ERROR(msg, ...) \
	do { \
		fprintf(stderr, "\x1B[31mERROR: " msg "\e[0m\n", ##__VA_ARGS__); \
	} while (0)

#define CBOX_CAMERA_LOG_INFO(msg, ...) \
	do { \
		fprintf(stdout, "INFO: " msg "\n", ##__VA_ARGS__); \
	} while (0)

#define CBOX_CAMERA_LOG_INFO_PLAIN(msg, ...) \
	do { \
		fprintf(stdout, "" msg "\n", ##__VA_ARGS__); \
	} while (0)

#define CBOX_CAMERA_LOG_WARN(msg, ...) \
	do { \
		fprintf(stderr, "\x1B[32mWARN: " msg "\e[0m\n", ##__VA_ARGS__); \
	} while (0)
#else
#define CBOX_CAMERA_LOG_ERROR(msg, ...)
#define CBOX_CAMERA_LOG_INFO(msg, ...)
#define CBOX_CAMERA_LOG_INFO_PLAIN(msg, ...)
#define CBOX_CAMERA_LOG_WARN(msg, ...)
#endif  // CBOX_CAMERA_ENABLE_LOG

struct cbox_camera_buffer {
	size_t size;
	void *buffer;
};

struct cbox_camera {
	char bus_info[512];
	char device_name[512];
	char driver_name[512];
	bool stream_opened;
	void *handler;
};

extern cbox_camera_error_t cbox_camera_errno;

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // CBOX_CAMERA_CBOX_CAMERA_INTERNAL_H_
