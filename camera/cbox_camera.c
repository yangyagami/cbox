#include "cbox_camera.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "cbox_camera_internal.h"

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

	return true;
}

void cbox_camera_destroy() {
	free(api);
}

cbox_camera_t **cbox_get_cameras(int *count) {
	assert(api);

	return NULL;
}

void cbox_free_cameras(cbox_camera_t **cameras) {
	assert(api);

}
