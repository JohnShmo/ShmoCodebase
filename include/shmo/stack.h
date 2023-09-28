//
// Created by scott on 9/28/2023.
//

#ifndef SHMOCODEBASE_STACK_H
#define SHMOCODEBASE_STACK_H

#include "allocator.h"

typedef struct Stack Stack;

Stack *stack_create(HeapAllocator *allocator);
void stack_destroy(Stack *sk);
bool stack_push(Stack *sk, Bytes elm);
bool stack_pop(Stack *sk);
void stack_clear(Stack *sk);
void stack_shrink(Stack *sk);
Bytes stack_top(const Stack *sk);
bool stack_empty(const Stack *sk);
usize stack_size(const Stack *sk);

#endif //SHMOCODEBASE_STACK_H
