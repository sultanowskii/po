#pragma once

#include <stddef.h>

#include "container/destruction.h"
#include "container/print.h"

struct Stack;
typedef struct Stack Stack;

Stack *stack_create(PrintFunction elem_print, DestroyFunction elem_destroy);
void   stack_push(Stack *stack, void *elem);
void  *stack_pop(Stack *stack);
void  *stack_top(const Stack *stack);
void   stack_print(const Stack *stack, size_t padding);
void   stack_destroy(Stack *stack);
