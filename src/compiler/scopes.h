#pragma once

#include "compiler/ast/program.h"
#include "container/map.h"

Map *scopes_build(Program *prog);
void scopes_destroy(Map *scopes);
