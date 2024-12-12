#ifndef CBOX_QUEUE_CBOX_QUEUE_H_
#define CBOX_QUEUE_CBOX_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

struct cbox_queue;
typedef struct cbox_queue cbox_queue_t;

extern cbox_queue_t *cbox_create_queue(size_t element_size);
extern void cbox_destroy_queue(cbox_queue_t *queue);

extern void cbox_queue_enqueue(cbox_queue_t *queue, void *element);
extern void *cbox_queue_dequeue(cbox_queue_t *queue);

extern bool cbox_queue_empty(cbox_queue_t *queue);
extern bool cbox_queue_fill(cbox_queue_t *queue);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // CBOX_QUEUE_CBOX_QUEUE_H_
