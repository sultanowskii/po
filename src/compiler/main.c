#include <stdio.h>

#include "compiler/ast/ast.h"
#include "compiler/ast/id_provider.h"

extern int yyparse(IDProvider *idp, Program **program);

int main(int argc, char **argv) {
    IDProvider *idp = id_provider_create();

    Program *prog;
    yyparse(idp, &prog);
    puts("done!");

    print_program(prog, 0);

    program_destroy(prog);
    id_provider_destroy(idp);
    return 0;
}
