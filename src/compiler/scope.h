#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "container/map.h"
#include "container/vec.h"

struct VarInfo {
    int32_t offset;
    bool    defined;
};
typedef struct VarInfo VarInfo;

struct Scope {
    size_t  id;
    int32_t base_offset;
    Map    *vars; // map[const char *var_name, VarInfo *stack_frame_base_offset]
    size_t  parent_id;
    Vec    *scopes;
    size_t  required_size;
};
typedef struct Scope Scope;

Scope   *scope_create(size_t id, int32_t base_offset);
void     scope_add_variable(Scope *scope, const char *name, int32_t offset);
VarInfo *scope_get_variable(Scope *scope, const char *name);
void     scope_print(const Scope *scope, size_t padding);
void     scope_destroy(Scope *scope);
