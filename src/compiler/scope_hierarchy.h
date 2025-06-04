#pragma once

#include <stddef.h>

#include "compiler/ast/program.h"
#include "container/map.h"

struct ScopeHierarchy;
typedef struct ScopeHierarchy ScopeHierarchy;

ScopeHierarchy *scope_hierarchy_build(const Program *prog);
void            scope_hierarchy_print(const ScopeHierarchy *scopes, size_t padding);
void            scope_hierarchy_destroy(ScopeHierarchy *scopes);
