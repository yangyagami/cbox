#include "cbox_camera.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "cbox_camera_internal.h"
#include "v4l2/cbox_camera_v4l2.h"
#include "cbox_array.h"

struct api {
	cbox_array_t *(*get_cameras)();
	void(*cbox_free_cameras)(cbox_camera_t **);
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
	assert(api);

	return api->get_cameras();
}

void cbox_free_cameras(cbox_array_t *cameras) {
	(void) cameras;
	assert(api);
}
