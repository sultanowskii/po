#include "compiler/ast/program.h"

#include <malloc.h>

#include "compiler/ast/statement_list.h"

struct Program {
    StatementList *statements;
};

Program *program_create(StatementList *statements) {
    Program *prog = malloc(sizeof(Program));
    prog->statements = statements;
    return prog;
}

Program *program_destroy(Program *program) {
    statement_list_destroy(program->statements);
    free(program);
}

