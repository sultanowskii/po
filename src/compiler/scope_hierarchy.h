#pragma once

#include <stddef.h>

#include "compiler/ast/program.h"
#include "container/map.h"
#include "result.h"

struct ScopeHierarchy;
typedef struct ScopeHierarchy ScopeHierarchy;

DEFINE_RESULT(ScopeHierarchyBuild, scope_hierarchy_build, ScopeHierarchy *)

ScopeHierarchyBuildResult scope_hierarchy_build(const Program *prog);
void                      scope_hierarchy_print(const ScopeHierarchy *scopes, size_t padding);
void                      scope_hierarchy_destroy(ScopeHierarchy *scopes);
