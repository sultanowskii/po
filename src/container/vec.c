#include "container/vec.h"

#include <malloc.h>
#include <stddef.h>

#define VEC_DEFAULT_SIZE 32

struct Vec {
    size_t  length;
    size_t  size;
    size_t *data;
};

Vec *vec_create(void) {
    Vec *vec = malloc(sizeof(Vec));
    *vec = (Vec){
        .length = 0,
        .size = VEC_DEFAULT_SIZE,
        .data = malloc(VEC_DEFAULT_SIZE * sizeof(vec->data))
    };
    return vec;
}

static inline void vector_grow(Vec *vec) {
    while (vec->length >= vec->size) {
        vec->size *= 2;
    }
    vec->data = realloc(vec->data, vec->size);
}

void vec_push_back(Vec *vec, size_t val) {
    if (vec->length >= vec->size) {
        vector_grow(vec);
    }
    vec->data[vec->length] = val;
    vec->length++;
}

size_t vec_get_at(const Vec *vec, size_t index) {
    if (index < vec->length) {
        return vec->data[index];
    }
    return -1;
}

size_t vec_length(const Vec *vec) {
    return vec->length;
}

void vec_destroy(Vec *vec) {
    free(vec->data);
    free(vec);
}
