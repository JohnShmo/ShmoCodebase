//
// Created by scott on 9/28/2023.
//

#include "shmo/stack.h"

typedef struct StackNode {
    u8 *elm;
    usize elm_size;
    struct StackNode *next;
} StackNode;

struct Stack {
    StackNode *top;
    StackNode *free_nodes;
    usize size;
    Allocator *allocator;
};

local_fn bool stack_node_init(StackNode *node, Bytes elm, Allocator *allocator) {
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

local_fn void stack_node_destruct(StackNode *node, Allocator *allocator) {
    assert(node);
    assert(allocator);

    allocator_free(allocator, node->elm);

    node->elm = nullptr;
    node->elm_size = 0;
    node->next = nullptr;
}

Stack *stack_create(Allocator *allocator) {
    if (!allocator)
        allocator = stdalloc;
    Stack *self = allocator_malloc(allocator, sizeof(Stack));
    if (!self)
        return nullptr;

    self->top = nullptr;
    self->free_nodes = nullptr;
    self->size = 0;
    self->allocator = allocator;

    return self;
}

void stack_destroy(Stack *self) {
    if (!self)
        return;
    stack_clear(self);
    stack_shrink(self);
    allocator_free(self->allocator, self);
}

bool stack_push(Stack *self, Bytes elm) {
    assert(self);
    if (bytes_is_null(elm))
        return false;

    StackNode *node;
    if (self->free_nodes) {
        node = self->free_nodes;
        self->free_nodes = node->next;
    } else {
        node = allocator_malloc(self->allocator, sizeof(StackNode));
        if (!node)
            return false;
    }
    if (!stack_node_init(node, elm, self->allocator)) {
        allocator_free(self->allocator, node);
        return false;
    }

    node->next = self->top;
    self->top = node;

    self->size++;
    return true;
}

bool stack_pop(Stack *self) {
    assert(self);
    if (!self->top)
        return false;

    StackNode *top = self->top;
    self->top = top->next;
    stack_node_destruct(top, self->allocator);
    top->next = self->free_nodes;
    self->free_nodes = top;

    self->size--;
    return true;
}

void stack_clear(Stack *self) {
    assert(self);
    while (!stack_empty(self))
        stack_pop(self);
}

void stack_shrink(Stack *self) {
    assert(self);
    while (self->free_nodes) {
        StackNode *to_free = self->free_nodes;
        self->free_nodes = to_free->next;
        allocator_free(self->allocator, to_free);
    }
}

Bytes stack_top(const Stack *self) {
    assert(self);
    if (stack_empty(self))
        return nullbytes;
    return bytes(self->top->elm, self->top->elm_size);
}

bool stack_empty(const Stack *self) {
    assert(self);
    return (!self->top);
}

usize stack_size(const Stack *self) {
    assert(self);
    return self->size;
}
