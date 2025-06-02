#pragma once

#include <stddef.h>

#include "compiler/ast/id_provider.h"
#include "compiler/ast/statement_list.h"

struct Program {
    size_t         id;
    StatementList *statements;
};
typedef struct Program Program;

Program *program_create(IDProvider *idp, StatementList *statements);
void     print_program(Program *prog, size_t padding);
Program *program_destroy(Program *program);
