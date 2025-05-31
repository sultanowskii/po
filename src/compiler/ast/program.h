#pragma once

#include "compiler/ast/statement_list.h"

struct Program;
typedef struct Program Program;

Program *program_create(StatementList *statements);
Program *program_destroy(Program *program);
