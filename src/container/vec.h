#pragma once

#include <stddef.h>

#include "container/destruction.h"
#include "container/print.h"

struct Vec;
typedef struct Vec Vec;

Vec   *vec_create(PrintFunction elem_print, DestroyFunction elem_destroy);
void   vec_push_back(Vec *vec, void *val);
void  *vec_get_at(const Vec *vec, size_t index);
size_t vec_length(const Vec *vec);
void   vec_print(const Vec *vec, size_t padding);
void   vec_destroy(Vec *vec);
