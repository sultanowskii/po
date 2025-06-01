#pragma once

#include <stddef.h>

#include "compiler/ast/statement_list.h"

struct Program {
    StatementList *statements;
};
typedef struct Program Program;

Program *program_create(StatementList *statements);
void     print_program(Program *prog, size_t padding);
Program *program_destroy(Program *program);
