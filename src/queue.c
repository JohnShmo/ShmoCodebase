//
// Created by scott on 9/29/2023.
//

#include "shmo/queue.h"

typedef struct QueueNode {
    u8 *elm;
    usize elm_size;
    struct QueueNode *next;
} QueueNode;

struct Queue {
    QueueNode *back;
    QueueNode *front;
    QueueNode *free_nodes;
    usize size;
    Allocator *allocator;
};

local_fn bool queue_node_init(QueueNode *node, Bytes elm, Allocator *allocator) {
    assert(node);
    assert(allocator);
    assert(!bytes_is_null(elm));

    u8 *new_elm = allocator_malloc(allocator, elm.size);
    if (!new_elm)
        return false;
    memcpy(new_elm, elm.p, elm.size);

    node->elm = new_elm;
    node->elm_size = elm.size;
    node->next = nullptr;

    return true;
}

local_fn void queue_node_destruct(QueueNode *node, Allocator *allocator) {
    assert(node);
    assert(allocator);

    allocator_free(allocator, node->elm);

    node->elm = nullptr;
    node->elm_size = 0;
    node->next = nullptr;
}

Queue *queue_create(Allocator *allocator) {
    if (!allocator)
        allocator = stdalloc;
    Queue *q = allocator_malloc(allocator, sizeof(Queue));
    if (!q)
        return nullptr;

    q->back = nullptr;
    q->front = nullptr;
    q->free_nodes = nullptr;
    q->size = 0;
    q->allocator = allocator;

    return q;
}

void queue_destroy(Queue *q) {
    if (!q)
        return;
    queue_clear(q);
    queue_shrink(q);
    allocator_free(q->allocator, q);
}

bool queue_enqueue(Queue *q, Bytes elm) {
    assert(q);
    if (bytes_is_null(elm))
        return false;

    QueueNode *node;
    if (q->free_nodes) {
        node = q->free_nodes;
        q->free_nodes = node->next;
    } else {
        node = allocator_malloc(q->allocator, sizeof(QueueNode));
        if (!node)
            return false;
    }
    if (!queue_node_init(node, elm, q->allocator)) {
        allocator_free(q->allocator, node);
        return false;
    }

    if (q->back) {
        q->back->next = node;
        q->back = node;
    } else {
        q->front = node;
        q->back = node;
    }

    q->size++;
    return true;
}

bool queue_dequeue(Queue *q) {
    assert(q);
    if (!q->front)
        return false;

    QueueNode *front = q->front;
    q->front = front->next;
    queue_node_destruct(front, q->allocator);
    front->next = q->free_nodes;
    q->free_nodes = front;
    if (!q->front)
        q->back = nullptr;

    q->size--;
    return true;
}

void queue_clear(Queue *q) {
    assert(q);
    while (!queue_empty(q))
        queue_dequeue(q);
}

void queue_shrink(Queue *q) {
    assert(q);
    while (q->free_nodes) {
        QueueNode *to_free = q->free_nodes;
        q->free_nodes = to_free->next;
        allocator_free(q->allocator, to_free);
    }
}

Bytes queue_front(const Queue *q) {
    assert(q);
    if (queue_empty(q))
        return nullbytes;
    return bytes(q->front->elm, q->front->elm_size);
}

bool queue_empty(const Queue *q) {
    assert(q);
    return (!q->front);
}

usize queue_size(const Queue *q) {
    assert(q);
    return q->size;
}
