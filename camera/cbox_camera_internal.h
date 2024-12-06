#ifndef CBOX_CAMERA_CBOX_CAMERA_INTERNAL_H_
#define CBOX_CAMERA_CBOX_CAMERA_INTERNAL_H_

#include "cbox_camera.h"

struct cbox_camera {
	char bus_info[512];
	char device_name[512];
	char driver_name[512];
	bool stream_opened;
	void *handler;
};
extern cbox_camera_error_t cbox_camera_errno;

#endif  // CBOX_CAMERA_CBOX_CAMERA_INTERNAL_H_
