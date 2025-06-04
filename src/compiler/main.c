#include <stdio.h>

#include "compiler/ast/ast.h"
#include "compiler/id_provider.h"
#include "compiler/scope_hierarchy.h"

extern int yyparse(IDProvider *idp, Program **program);

int main(int argc, char **argv) {
    IDProvider *idp = id_provider_create();

    Program *prog;
    yyparse(idp, &prog);
    puts("done!");

    ScopeHierarchy *sh = scope_hierarchy_build(prog);

    program_print(prog, 0);

    scope_hierarchy_print(sh, 0);

    scope_hierarchy_destroy(sh);
    program_destroy(prog);
    id_provider_destroy(idp);
    return 0;
}
