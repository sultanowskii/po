#include <malloc.h>
#include <stdio.h>

#include "ast/ast.h"
#include "id_provider.h"
#include "interp/interp.h"

extern int   yyparse(IDProvider *idp, Program **program);
extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s FILE", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    yyin = file;

    IDProvider *idp = id_provider_create();

    Program *prog;
    int      parse_result = yyparse(idp, &prog);
    switch (parse_result) {
    case 0:
        puts("\rparse: parsed successfully");
        break;
    case 1:
        puts("\rparse: syntax error");
        return 1;
    case 2:
        puts("\rparse: memory error");
        return 1;
    }

    program_print(prog, 0);

    char *err = interpret(prog);
    if (err != NULL) {
        puts(err);
        free(err);
    }

    program_destroy(prog);
    id_provider_destroy(idp);
    return 0;
}
