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

    if (node->elm)
        allocator_free(allocator, node->elm);

    node->elm = nullptr;
    node->elm_size = 0;
    node->next = nullptr;
}

Stack *stack_create(Allocator *allocator) {
    if (!allocator)
        allocator = stdalloc;
    Stack *sk = allocator_malloc(allocator, sizeof(Stack));
    if (!sk)
        return nullptr;

    sk->top = nullptr;
    sk->free_nodes = nullptr;
    sk->size = 0;
    sk->allocator = allocator;

    return sk;
}

void stack_destroy(Stack *sk) {
    if (!sk)
        return;
    stack_clear(sk);
    stack_shrink(sk);
    allocator_free(sk->allocator, sk);
}

bool stack_push(Stack *sk, Bytes elm) {
    if (!sk || bytes_is_null(elm))
        return false;

    StackNode *node;
    if (sk->free_nodes) {
        node = sk->free_nodes;
        sk->free_nodes = node->next;
    } else {
        node = allocator_malloc(sk->allocator, sizeof(StackNode));
        if (!node)
            return false;
    }
    if (!stack_node_init(node, elm, sk->allocator))
        return false;

    node->next = sk->top;
    sk->top = node;

    sk->size++;
    return true;
}

bool stack_pop(Stack *sk) {
    if (!sk || !sk->top)
        return false;

    StackNode *top = sk->top;
    sk->top = top->next;
    stack_node_destruct(top, sk->allocator);
    top->next = sk->free_nodes;
    sk->free_nodes = top;

    sk->size--;
    return true;
}

void stack_clear(Stack *sk) {
    if (!sk)
        return;
    while (!stack_empty(sk))
        stack_pop(sk);
}

void stack_shrink(Stack *sk) {
    if (!sk)
        return;
    while (sk->free_nodes) {
        StackNode *to_free = sk->free_nodes;
        sk->free_nodes = to_free->next;
        allocator_free(sk->allocator, to_free);
    }
}

Bytes stack_top(const Stack *sk) {
    if (!sk || stack_empty(sk))
        return nullbytes;
    return bytes(sk->top->elm, sk->top->elm_size);
}

bool stack_empty(const Stack *sk) {
    return (!sk || !sk->top);
}

usize stack_size(const Stack *sk) {
    return sk->size;
}
