#include <stdio.h>

#include "compiler/ast/ast.h"

extern int yyparse(Program **program);

int main(int argc, char **argv) {
    Program *prog;
    yyparse(&prog);
    puts("done!");

    print_program(prog, 0);
    return 0;
}
