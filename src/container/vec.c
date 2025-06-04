#include "container/vec.h"

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

#include "container/destruction.h"
#include "container/print.h"
#include "fmt.h"

#define VEC_DEFAULT_SIZE 32

struct Vec {
    PrintFunction   elem_print;
    DestroyFunction elem_destroy;
    size_t          length;
    size_t          size;
    void          **data;
};

Vec *vec_create(PrintFunction elem_print, DestroyFunction elem_destroy) {
    Vec *vec = malloc(sizeof(Vec));
    *vec = (Vec){
        .elem_print = elem_print,
        .elem_destroy = elem_destroy,
        .length = 0,
        .size = VEC_DEFAULT_SIZE,
        .data = malloc(VEC_DEFAULT_SIZE * sizeof(*vec->data))
    };
    return vec;
}

void vec_destroy(Vec *vec) {
    for (size_t i = 0; i < vec->length; i++) {
        vec->elem_destroy(vec->data[i]);
    }
    free(vec->data);
    free(vec);
}

static inline void vector_grow(Vec *vec) {
    while (vec->length >= vec->size) {
        vec->size *= 2;
    }
    vec->data = realloc(vec->data, vec->size);
}

void vec_push_back(Vec *vec, void *val) {
    if (vec->length >= vec->size) {
        vector_grow(vec);
    }
    vec->data[vec->length] = val;
    vec->length++;
}

void *vec_get_at(const Vec *vec, size_t index) {
    if (index < vec->length) {
        return vec->data[index];
    }
    return NULL;
}

size_t vec_length(const Vec *vec) {
    return vec->length;
}

void vec_print(const Vec *vec, size_t padding) {
    print_padding(padding);
    printf("Vec[length=%zu, size=%zu]\n", vec->length, vec->size);

    print_padding(padding + 1);
    puts("data:");
    for (size_t i = 0; i < vec->length; i++) {
        vec->elem_print(vec->data[i], padding + 2);
    }
}
