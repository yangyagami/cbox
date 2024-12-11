#include "cbox_camera.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "cbox_camera_internal.h"
#include "v4l2/cbox_camera_v4l2.h"
#include "cbox_array.h"

struct api {
	cbox_array_t *(*get_cameras)();
	bool(*open_camera)(cbox_camera_t *, cbox_camera_param_t *);
};
typedef struct api api_t;

cbox_camera_error_t cbox_camera_errno;

static api_t *api = NULL;

bool cbox_camera_init() {
	api = calloc(1, sizeof(*api));

	assert(api);

	api->get_cameras = cbox_v4l2_get_cameras;

	return true;
}

void cbox_camera_destroy() {
	free(api);
}

cbox_array_t *cbox_get_cameras() {
	assert(api && api->get_cameras);

	return api->get_cameras();
}

bool cbox_camera_open(cbox_camera_t *camera, cbox_camera_param_t *param) {
	(void) camera;
	(void) param;
	return true;
}

bool cbox_camera_close(cbox_camera_t *camera) {
	(void) camera;
	return true;
}
