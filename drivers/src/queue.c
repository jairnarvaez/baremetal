#include "queue.h"

void queue_init(queue_t* q, uint8_t* buf, uint32_t size)
{
    q->buf = buf;
    q->size = size;
    q->head = 0;
    q->tail = 0;
}

uint8_t queue_is_empty(const queue_t* q)
{
    return q->head == q->tail;
}

uint8_t queue_is_full(const queue_t* q)
{
    return ((q->tail + 1) % q->size) == q->head;
}

uint8_t queue_push(queue_t* q, uint8_t data)
{
    if (queue_is_full(q))
        return 0;

    q->buf[q->tail] = data;
    q->tail = (q->tail + 1) % q->size;
    return 1;
}

uint8_t queue_pop(queue_t* q, uint8_t* data)
{
    if (queue_is_empty(q))
        return 0;

    *data = q->buf[q->head];
    q->head = (q->head + 1) % q->size;
    return 1;
}
