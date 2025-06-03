#pragma once

#include <stddef.h>

struct Vec;
typedef struct Vec Vec;

Vec   *vec_create(void);
void   vec_push_back(Vec *vec, size_t val);
size_t vec_get_at(const Vec *vec, size_t index);
size_t vec_length(const Vec *vec);
void   vec_destroy(Vec *vec);
