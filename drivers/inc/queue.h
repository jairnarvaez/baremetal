#include <stdint.h>

typedef struct {
    uint8_t* buf;
    uint32_t head, tail, size;
} queue_t;

void queue_init(queue_t* q, uint8_t* buf, uint32_t size);
uint8_t queue_is_empty(const queue_t* q);
uint8_t queue_is_full(const queue_t* q);
uint8_t queue_push(queue_t* q, uint8_t data);
uint8_t queue_pop(queue_t* q, uint8_t* data);
