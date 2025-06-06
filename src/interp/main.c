#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "id_provider.h"
#include "interp/interp.h"

extern int   yyparse(IDProvider *idp, Program **program);
extern FILE *yyin;

void print_usage(const char *name) {
    printf("usage: %s COMMAND\n", name);
    puts("commands: run, ast");
}

void print_usage_run(const char *name) {
    printf("usage (run): %s run FILE\n", name);
}

void print_usage_ast(const char *name) {
    printf("usage (AST): %s ast FILE\n", name);
}

int run(int argc, char **argv) {
    if (argc < 3) {
        print_usage_run(argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[2], "r");
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
        break;
    case 1:
        puts("\rparse: syntax error");
        return 1;
    case 2:
        puts("\rparse: memory error");
        return 1;
    }

    printf("\r");
    char *err = interpret(prog);
    if (err != NULL) {
        puts(err);
        free(err);
    }

    program_destroy(prog);
    id_provider_destroy(idp);
    return 0;
}

int ast(int argc, char **argv) {
    if (argc < 3) {
        print_usage_run(argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[2], "r");
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
        break;
    case 1:
        puts("\rparse: syntax error");
        return 1;
    case 2:
        puts("\rparse: memory error");
        return 1;
    }

    printf("\r");
    program_print(prog, 0);

    program_destroy(prog);
    id_provider_destroy(idp);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *cmd = argv[1];
    if (strcmp(cmd, "run") == 0) {
        return run(argc, argv);
    } else if (strcmp(cmd, "ast") == 0) {
        return ast(argc, argv);
    } else {
        print_usage(argv[0]);
        return 1;
    }
}
