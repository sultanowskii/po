#include <malloc.h>
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

    ScopeHierarchyBuildResult res = scope_hierarchy_build(prog);
    if (scope_hierarchy_build_result_is_err(res)) {
        char *err_msg = scope_hierarchy_build_result_get_err(res);
        puts(err_msg);
        free(err_msg);
        return 1;
    }
    ScopeHierarchy *sh = scope_hierarchy_build_result_get_val(res);

    program_print(prog, 0);

    scope_hierarchy_print(sh, 0);

    scope_hierarchy_destroy(sh);
    program_destroy(prog);
    id_provider_destroy(idp);
    return 0;
}
