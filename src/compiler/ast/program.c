#include "compiler/ast/program.h"

#include <malloc.h>
#include <stdio.h>

#include "compiler/ast/statement_list.h"
#include "fmt.h"

Program *program_create(StatementList *statements) {
    Program *prog = malloc(sizeof(Program));
    prog->statements = statements;
    return prog;
}

Program *program_destroy(Program *program) {
    statement_list_destroy(program->statements);
    free(program);
}

void print_program(Program *prog, size_t padding) {
    puts("Program");

    print_padding(padding + 1);
    puts("statements:");
    statement_list_print(prog->statements, padding + 2);
}
