//
// Created by scott on 9/28/2023.
//

#ifndef SHMOCODEBASE_QUEUE_H
#define SHMOCODEBASE_QUEUE_H

#include "allocator.h"

typedef struct Queue Queue;

Queue *queue_create(Allocator *allocator);
void queue_destroy(Queue *self);
bool queue_enqueue(Queue *self, Bytes elm);
bool queue_dequeue(Queue *self);
void queue_clear(Queue *self);
void queue_shrink(Queue *self);
Bytes queue_front(const Queue *self);
bool queue_empty(const Queue *self);
usize queue_size(const Queue *self);

#endif //SHMOCODEBASE_QUEUE_H
