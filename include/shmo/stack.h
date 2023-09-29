//
// Created by scott on 9/28/2023.
//

#ifndef SHMOCODEBASE_STACK_H
#define SHMOCODEBASE_STACK_H

#include "allocator.h"

typedef struct Stack Stack;

Stack *stack_create(Allocator *allocator);
void stack_destroy(Stack *self);
bool stack_push(Stack *self, Bytes elm);
bool stack_pop(Stack *self);
void stack_clear(Stack *self);
void stack_shrink(Stack *self);
Bytes stack_top(const Stack *self);
bool stack_empty(const Stack *self);
usize stack_size(const Stack *self);

#endif //SHMOCODEBASE_STACK_H
