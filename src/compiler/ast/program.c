#include "compiler/ast/program.h"

#include <malloc.h>
#include <stdio.h>

#include "compiler/ast/id_provider.h"
#include "compiler/ast/statement_list.h"
#include "fmt.h"

Program *program_create(IDProvider *idp, StatementList *statements) {
    Program *prog = malloc(sizeof(Program));
    prog->statements = statements;
    prog->id = id_provider_get(idp);
    return prog;
}

Program *program_destroy(Program *program) {
    statement_list_destroy(program->statements);
    free(program);
}

void print_program(Program *prog, size_t padding) {
    printf("Program[id=%zu]\n", prog->id);

    print_padding(padding + 1);
    puts("statements:");
    statement_list_print(prog->statements, padding + 2);
}
