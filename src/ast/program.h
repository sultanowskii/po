#pragma once

#include <stddef.h>

#include "ast/statement_list.h"
#include "id_provider.h"

struct Program {
    size_t         id;
    StatementList *statements;
};
typedef struct Program Program;

Program *program_create(IDProvider *idp, StatementList *statements);
void     program_print(Program *prog, size_t padding);
Program *program_destroy(Program *program);
