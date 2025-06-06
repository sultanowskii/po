#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "container/map.h"
#include "container/vec.h"

typedef int32_t Offset;

struct VarInfo {
    Offset offset;
    bool   defined;
};
typedef struct VarInfo VarInfo;

struct Scope {
    size_t id;
    Offset base_offset;
    Map   *vars; // map[const char *var_name, VarInfo *stack_frame_base_offset]
    size_t parent_id;
    Vec   *scopes;
    size_t required_size;
};
typedef struct Scope Scope;

Scope   *scope_create(size_t id, Offset base_offset);
void     scope_add_variable(Scope *scope, const char *name, Offset offset);
VarInfo *scope_get_variable(Scope *scope, const char *name);
void     scope_add_scope(Scope *scope, uint32_t inner_scope_id);
void     scope_print(const Scope *scope, size_t padding);
void     scope_destroy(Scope *scope);
