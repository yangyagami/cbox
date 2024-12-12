#ifndef CBOX_ARRAY_CBOX_ARRAY_H_
#define CBOX_ARRAY_CBOX_ARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stddef.h>

struct cbox_array;
typedef struct cbox_array cbox_array_t;


extern cbox_array_t *cbox_create_array(size_t size, size_t element_size);
extern void cbox_destroy_array(cbox_array_t *array);

extern void *cbox_array_get_element(cbox_array_t *array, int index);

extern void cbox_array_add(cbox_array_t *array, void *element);
extern void cbox_array_remove(cbox_array_t *array, int index);

extern size_t cbox_get_array_size(cbox_array_t *array);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // CBOX_ARRAY_CBOX_ARRAY_H_
