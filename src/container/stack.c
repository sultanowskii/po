#include "container/stack.h"

#include <malloc.h>
#include <stdio.h>

#include "container/destruction.h"
#include "container/print.h"
#include "std/fmt.h"

static inline StackNode *stack_node_create(void *val) {
    StackNode *node = malloc(sizeof(StackNode));
    *node = (StackNode){
        .val = val,
        .next = NULL,
    };
    return node;
}

static inline void stack_node_destroy(const Stack *stack, StackNode *node) {
    stack->elem_destroy(node->val);
    free(node);
}

Stack *stack_create(PrintFunction elem_print, DestroyFunction elem_destroy) {
    Stack *stack = malloc(sizeof(Stack));
    *stack = (Stack){
        .elem_print = elem_print,
        .elem_destroy = elem_destroy,
        .head = NULL,
    };
    return stack;
}

void stack_destroy(Stack *stack) {
    StackNode *node = stack->head;
    while (node != NULL) {
        StackNode *next = node->next;
        stack_node_destroy(stack, node);
        node = next;
    }
    free(stack);
}

void stack_push(Stack *stack, void *elem) {
    StackNode *node = stack_node_create(elem);
    node->next = stack->head;
    stack->head = node;
}

void *stack_pop(Stack *stack) {
    if (stack->head == NULL) {
        return NULL;
    }

    void      *val = stack->head->val;
    StackNode *next = stack->head->next;
    free(stack->head);
    stack->head = next;
    return val;
}

void *stack_top(const Stack *stack) {
    if (stack->head == NULL) {
        return NULL;
    }

    return stack->head->val;
}

void stack_print(const Stack *stack, size_t padding) {
    print_padding(padding);
    puts("Stack");

    print_padding(padding + 1);
    puts("data:");

    StackNode *node = stack->head;
    while (node != NULL) {
        stack->elem_print(node->val, padding + 2);
    }
}
