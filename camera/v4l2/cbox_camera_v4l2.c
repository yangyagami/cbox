#include "v4l2/cbox_camera_v4l2.h"

#include <stdio.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <libv4l2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "cbox_camera.h"
#include "cbox_camera_internal.h"
#include "cbox_array.h"

#define CBOX_V4L2_CAMERA_BUFFER_COUNT (6)

static cbox_array_t *cameras = NULL;

bool cbox_v4l2_try_ioctl(int fd, int request, void *output) {
	int ret;
	int err;
	struct timeval tv;
	fd_set fdset;
	int select_ret;

ioctl_retry:
	ret = ioctl(fd, request, output);

	if (ret != -1) {
		return true;
	}

	err = errno;

	if (err == EINPROGRESS || err == EAGAIN || err == EBUSY) {

		FD_ZERO(&fdset);
		FD_SET(fd, &fdset);

		tv.tv_sec = 5;
		tv.tv_usec = 0;

		select_ret = select(fd + 1, &fdset, NULL, NULL,
					&tv);
		if (select_ret == -1) {
			goto ioctl_retry;
		}

		err = errno;

		if (err == EBADF) {
			cbox_camera_errno = CBOX_CAMERA_NO_SUCH_DEVICE;
		} else if (err == EINVAL) {
			cbox_camera_errno = CBOX_CAMERA_INVALID_PARAM;
		} else {
			// TODO(yangsiyu): Handle error
		}

		CBOX_CAMERA_LOG_ERROR("select failed: %s", strerror(err));

		return false;
	} else {
		if (err == EBADF || err == ENOENT ||
			err == ENODEV || err == EPIPE) {
			// TODO(yangsiyu): Handle error
			cbox_camera_errno = CBOX_CAMERA_NO_SUCH_DEVICE;
		}
	}

	// TODO(yangsiyu): Handle errno
	if (err == EINVAL) {
		cbox_camera_errno = CBOX_CAMERA_INVALID_PARAM;
	}
	return false;
}

cbox_array_t *cbox_v4l2_get_cameras() {
	if (!cameras) {
		cameras = cbox_create_array(12,
					sizeof(cbox_camera_t*));
	}

	DIR *dir = opendir("/dev");
	if (dir == NULL) {
		// TODO(yangsiyu): Handle error;
		CBOX_CAMERA_LOG_ERROR("Could not open /dev directory: %s",
				strerror(errno));
		return cameras;
	}

	char device_path[512];
	struct dirent *ent;
	while ((ent = readdir(dir)) != NULL) {
		int fd = -1;
		if (strncmp(ent->d_name, "video", 5) != 0) {
			goto out;
		}

		sprintf(device_path, "/dev/%s", ent->d_name);

		fd = open(device_path, O_RDWR);
		if (fd == -1) {
			// TODO(yangsiyu): Handle error
			goto out;
		}

		struct v4l2_capability cap;
		if (!cbox_v4l2_try_ioctl(fd, VIDIOC_QUERYCAP, &cap)) {
			goto out;
		}

		uint32_t capabilities = cap.capabilities;
		if (capabilities & V4L2_CAP_DEVICE_CAPS)
			capabilities = cap.device_caps;

		if (!(capabilities & V4L2_CAP_VIDEO_CAPTURE) &&
			!(capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) {
			goto out;
		}

		cbox_camera_t *camera = NULL;

		int cameras_size = cbox_get_array_size(cameras);
		for (int i = 0; i < cameras_size; ++i) {
			cbox_camera_t *tmp =
				cbox_array_get_element(cameras, i);
			if (strcmp(tmp->bus_info, (const char *)cap.bus_info) == 0) {
				cbox_v4l2_camera_handler_t *handler = tmp->handler;
				strcat(handler->device_path, ";");
				strcat(handler->device_path, device_path);

				camera = tmp;

				break;
			}
		}

		if (!camera) {
			camera = calloc(1, sizeof(*camera));

			cbox_v4l2_camera_handler_t *handler = NULL;
			handler = calloc(1, sizeof(*handler));

			handler->video_fd = -1;

			camera->handler = handler;

			strcpy(camera->device_name, (char *)cap.card);
			strcpy(camera->driver_name, (char *)cap.driver);
			strcpy(handler->device_path, (char *)device_path);
			strcpy(camera->bus_info, (char *)cap.bus_info);

			cbox_array_add(cameras, &camera);
		}

		assert(camera);

		struct v4l2_format fmt;
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (!cbox_v4l2_try_ioctl(fd, VIDIOC_G_FMT, &fmt)) {
			goto out;
		}

		if (fmt.type == V4L2_BUF_TYPE_VIDEO_CAPTURE ||
			fmt.type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
			cbox_v4l2_camera_handler_t *handler = camera->handler;
			handler->video_fd = fd;

			continue;
		}

        out:
		if (fd != -1) {
			close(fd);
		}
		continue;
	}

	closedir(dir);

	return cameras;
}

static void query_frame_interval(cbox_v4l2_camera_handler_t *handler,
                                uint32_t pixelfmt, int width, int height) {
	assert(handler);

	struct v4l2_frmivalenum frmival;
	frmival.index = 0;
	frmival.pixel_format = pixelfmt;
	frmival.width = width;
	frmival.height = height;

	int fps_list[512] = { 0 };
	int fps_count = 0;
	while (cbox_v4l2_try_ioctl(handler->video_fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival)) {
		// TODO(yangsiyu): Handle different type...
		if (frmival.type != V4L2_FRMIVAL_TYPE_DISCRETE) {
			break;
		}

		int fps = frmival.discrete.denominator / frmival.discrete.numerator;

		for (uint32_t i = 0; i < frmival.index; i++) {
			if (fps_list[i] == fps) {
				goto out;
			}
		}

		fps_list[frmival.index] = fps;
		fps_count++;

	out:
		frmival.index++;
	}

	if (frmival.index <= 0) {
		return;
	}

	char buffer[512] = { 0 };
	for (int i = 0; i < fps_count; i++) {
		char tmp[20] = { 0 };
		sprintf(tmp, "%d ", fps_list[i]);
		strcat(buffer, tmp);
	}

	CBOX_CAMERA_LOG_INFO_PLAIN(
		"\t\t\tfps: %s",
		buffer);
}

static void query_frame_size(cbox_v4l2_camera_handler_t *handler,
                             uint32_t pixelfmt) {
	assert(handler);

	struct v4l2_frmsizeenum frmsize;
	frmsize.pixel_format = pixelfmt;
	frmsize.index = 0;

	while (cbox_v4l2_try_ioctl(handler->video_fd, VIDIOC_ENUM_FRAMESIZES, &frmsize)) {
		CBOX_CAMERA_LOG_INFO_PLAIN(
			"\t\twidth: %d, height: %d",
			frmsize.discrete.width,
			frmsize.discrete.height);
		query_frame_interval(
			handler,
			pixelfmt,
			frmsize.discrete.width,
			frmsize.discrete.height);
		frmsize.index++;
	}
}

bool cbox_v4l2_open_camera(cbox_camera_t *camera, cbox_camera_param_t *param) {
	(void) param;

	assert(camera && camera->handler);

	cbox_v4l2_camera_handler_t *handler = camera->handler;

	if (handler->video_fd == -1) {
		// TODO(yangsiyu): Handler error
		return false;
	}

	struct v4l2_fmtdesc fmtdesc;
        fmtdesc.index = 0;
        fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	CBOX_CAMERA_LOG_INFO_PLAIN("Camera: %s.", camera->device_name);
	CBOX_CAMERA_LOG_INFO_PLAIN("Driver: %s.", camera->driver_name);
	while (cbox_v4l2_try_ioctl(handler->video_fd, VIDIOC_ENUM_FMT, &fmtdesc)) {
		CBOX_CAMERA_LOG_INFO_PLAIN(
			"\tFormat: %c %c %c %c.\n"
			"\tFormat description: %s.",
			fmtdesc.pixelformat & 0xFF,
			(fmtdesc.pixelformat >> 8) & 0xFF,
			(fmtdesc.pixelformat >> 16) & 0xFF,
			(fmtdesc.pixelformat >> 24) & 0xFF,
			fmtdesc.description
			);
		query_frame_size(handler, fmtdesc.pixelformat);
		fmtdesc.index++;
	}

	if (param != NULL) {
		// TODO(yangsiyu): Handle param...
	}



	// TODO(yangsiyu): Handle error

	return true;
}

static void destroy_camera(cbox_camera_t *camera) {
	if (camera) {
		if (camera->handler) {
			free(camera->handler);
		}
		free(camera);
	}
}

void cbox_v4l2_free_cameras() {
	for (size_t i = 0; i < cbox_get_array_size(cameras); ++i) {
		cbox_camera_t *camera =
			*(cbox_camera_t**)cbox_array_get_element(cameras, i);

		destroy_camera(camera);
	}

	cbox_destroy_array(cameras);
}

#undef CBOX_V4L2_CAMERA_BUFFER_COUNT
