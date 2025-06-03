#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "container/map.h"
#include "container/vec.h"

struct Scope {
    size_t  id;
    int32_t base_offset;
    Map    *vars; // map[const char *var_name, ssize_t stack_frame_base_offset]
    size_t  parent_id;
    Vec    *scopes;
    size_t  required_size;
};
typedef struct Scope Scope;

Scope *scope_create(size_t id, int32_t base_offset);
void   scope_destroy(Scope *scope);
