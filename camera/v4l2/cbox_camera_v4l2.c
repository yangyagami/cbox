#include "v4l2/cbox_camera_v4l2.h"

#include <stdio.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#include "cbox_camera_internal.h"

bool cbox_v4l2_try_ioctl(int fd, int request, void *output) {
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
				cbox_camera_errno = CBOX_CAMERA_NO_SUCH_DEVICE;
			} else {
				cbox_camera_errno = CBOX_CAMERA_INVALID_PARAM;
			}
		        CBOX_CAMERA_LOG_ERROR("select failed: %s", strerror(errno));
			return false;
		}
	} else {
		if (errno == EBADF || errno == ENOENT ||
			errno == ENODEV || errno == EPIPE) {
			cbox_camera_errno = CBOX_CAMERA_NO_SUCH_DEVICE;
		}
	}
	return false;
}

static cbox_camera_t *cbox_v4l2_create_camera(int fd, cbox_camera_t **cameras,
					const char *device_path) {
	cbox_camera_t *camera = NULL;

	struct v4l2_capability cap;
	if (cbox_v4l2_try_ioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		uint32_t capabilities = cap.capabilities;
		if (capabilities & V4L2_CAP_DEVICE_CAPS)
			capabilities = cap.device_caps;

		if (!(capabilities & V4L2_CAP_VIDEO_CAPTURE) &&
			!(capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) {
			goto out;
		}

		if (cameras != NULL) {
			for (cbox_camera_t *tmp = *cameras; tmp != NULL; ++tmp) {
				if (strcmp(tmp->bus_info,
						(const char *)cap.bus_info) == 0) {
					cbox_v4l2_camera_handler_t *handler =
						tmp->handler;
					strcat(handler->device_path, ";");
					strcat(handler->device_path, device_path);

					goto out;
				}
			}
		}

		camera = calloc(1, sizeof(*camera));

		cbox_v4l2_camera_handler_t *handler = NULL;
		handler = calloc(1, sizeof(*handler));

		handler->fd = -1;

		camera->handler = handler;

		strcpy(camera->device_name, (char *)cap.card);
		strcpy(camera->driver_name, (char *)cap.driver);
		strcpy(handler->device_path, (char *)device_path);
		strcpy(camera->bus_info, (char *)cap.bus_info);
	}

out:
	return camera;
}

static int cbox_v4l2_readdir(DIR *dir, char device_path[512]) {
	struct dirent *ent;
	if ((ent = readdir(dir)) != NULL) {
		if (strncmp(ent->d_name, "video", 5) == 0) {
			sprintf(device_path, "/dev/%s", ent->d_name);
			int fd = open(device_path, O_RDONLY);

			return fd;
		}
	}

	return -1;
}

cbox_camera_t **cbox_v4l2_get_cameras(int *count) {
	cbox_camera_t **cameras = NULL;
	int cameras_size = 0;
	int c = 0;

	DIR *dir;
	if ((dir = opendir("/dev")) != NULL) {
		char device_path[512];
		int fd = -1;
		while ((fd = cbox_v4l2_readdir(dir, device_path)) != -1) {
			cbox_camera_t *camera =
				cbox_v4l2_create_camera(fd, cameras, device_path);
			if (camera != NULL) {
				c++;

				if (cameras == NULL) {
					cameras_size =
						5;
					cameras = calloc(cameras_size, sizeof(*camera));
				}

				if (c - 1 >= cameras_size - 1) {
					cameras_size += 3;
					cameras = realloc(cameras, sizeof(*camera) * cameras_size);
				}

				cameras[c - 1] = camera;
			}
			close(fd);
		}
		closedir(dir);
	} else {
		CBOX_CAMERA_LOG_ERROR("Could not open /dev directory: %s", strerror(errno));
	}

	*count = c;

	return cameras;
}

cbox_camera_t **cbox_v4l2_free_cameras(cbox_camera_t **cameras) {
	return NULL;
}

