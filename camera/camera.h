#ifndef CBOX_CAMERA_H_
#define CBOX_CAMERA_H_

#ifdef __cpluscplus
extern "C" {
#endif  // __cpluscplus

#include <stdbool.h>
#include <unistd.h>

struct cbox_camera_buffer {
	void *buffer;
	size_t size;
};
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

cbox_camera_t **cbox_get_cameras(int *count);
void cbox_free_cameras(cbox_camera_t **cameras);

// bool cbox_camera_open(cbox_camera_t *camera, cbox_camera_param_t *param);

#ifdef __cpluscplus
}
#endif  // __cpluscplus

#endif  // CBOX_CAMERA_H_

#ifdef CBOX_CAMERA_IMPLEMENTATION

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

#ifdef CBOX_CAMERA_BACKEND_V4L2

#include <stdio.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

struct cbox_camera {
	char bus_info[512];
	char device_path[512];
	char device_name[512];
	char driver_name[512];
	bool stream_opened;
	int fd;
};

static cbox_camera_error_t cbox_camera_error;

static bool cbox_try_ioctl(int fd, int request, void *output) {
ioctl_retry:
	int ret = ioctl(fd, request, output);
	if (ret != -1) {
		return true;
	}
	int err = errno;
	if (err == EINPROGRESS || err == EAGAIN || err == EBUSY) {
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(fd, &fdset);
		struct timeval tv;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		int select_ret = select(fd + 1, &fdset, NULL, NULL,
					&tv);
		if (select_ret > 0) {
			goto ioctl_retry;
		} else {
			if (errno == EBADF) {
				cbox_camera_error = CBOX_CAMERA_NO_SUCH_DEVICE;
			} else {
				cbox_camera_error = CBOX_CAMERA_INVALID_PARAM;
			}
		        CBOX_CAMERA_LOG_ERROR("select failed: %s", strerror(errno));
			return false;
		}
	} else {
		if (errno == EBADF || errno == ENOENT ||
			errno == ENODEV || errno == EPIPE) {
			cbox_camera_error = CBOX_CAMERA_NO_SUCH_DEVICE;
		}
	}
	return false;
}

cbox_camera_t **cbox_get_cameras(int *count) {
	cbox_camera_t **cameras = NULL;
	int cameras_size = 0;
	int c = 0;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("/dev")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strncmp(ent->d_name, "video", 5) == 0) {
				char device_path[512];
				sprintf(device_path, "/dev/%s", ent->d_name);
				int fd = open(device_path, O_RDONLY);
				if (fd != -1) {
					struct v4l2_capability cap;
					if (cbox_try_ioctl(fd, VIDIOC_QUERYCAP, &cap)) {
						for (int i = 0; i < cameras_size; i++) {
							if (cameras[i] != NULL && strcmp(cameras[i]->bus_info, (const char *)cap.bus_info) == 0) {
								strcat(cameras[i]->device_path, ";");
								strcat(cameras[i]->device_path, device_path);
								goto out;
							}
						}

						cbox_camera_t *camera = NULL;
						camera = calloc(1, sizeof(*camera));

						strcpy(camera->device_name, (char *)cap.card);
						strcpy(camera->driver_name, (char *)cap.driver);
						strcpy(camera->device_path, (char *)device_path);
						strcpy(camera->bus_info, (char *)cap.bus_info);

						c++;

						if (cameras == NULL) {
							cameras_size = c * 3;
							cameras = calloc(cameras_size, sizeof(*camera));
						}

						if (c - 1 >= cameras_size - 1) {
							cameras_size = c * 3;
							cameras = realloc(cameras, sizeof(*camera) * cameras_size);
						}

						cameras[c - 1] = camera;
					}
				out:
					close(fd);
				}
			}
		}
		closedir(dir);
	} else {
		CBOX_CAMERA_LOG_ERROR("Could not open /dev directory: %s", strerror(errno));
	}

	*count = c;

	return cameras;
}

void cbox_free_cameras(cbox_camera_t **cameras) {
	cbox_camera_t **it = cameras;
	while (true) {
		if (*it == NULL) {
			break;
		}
		free(*it);

	        it++;
	}
	free(cameras);
}

#else  // CBOX_CAMERA_BACKEND_V4l2
#error "Unsupported backend, currently only supports v4l2."
#endif

#endif  // CBOX_CAMERA_IMPLEMENTATION
