#include "cbox_camera.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "cbox_camera_internal.h"
#include "v4l2/cbox_camera_v4l2.h"

struct api {
	cbox_camera_t **(*get_cameras)(int *);
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

cbox_camera_t **cbox_get_cameras(int *count) {
	assert(api);

	return api->get_cameras(count);
}

void cbox_free_cameras(cbox_camera_t **cameras) {
	assert(api);
}