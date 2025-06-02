#include "compiler/ast/statement.h"

#include <malloc.h>
#include <stdio.h>

#include "compiler/ast/block.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/statement_list.h"
#include "fmt.h"

Statement *statement_create_empty(void) {
    return malloc(sizeof(Statement));
}

Statement *statement_create_new_variable(Identifier *identifier, Expression *expression) {
    Statement *stmt = statement_create_empty();
    *stmt = (Statement){
        .type = STATEMENT_NEW_VARIABLE,
        .new_variable = {
            .ident = identifier,
            .expr = expression,
        },
    };
    return stmt;
}

Statement *statement_create_assign(Identifier *identifier, Expression *expression) {
    Statement *stmt = statement_create_empty();
    *stmt = (Statement){
        .type = STATEMENT_ASSIGN,
        .assign = {
            .ident = identifier,
            .expr = expression,
        },
    };
    return stmt;
}

Statement *statement_create_if(Expression *cond, Block *if_block) {
    Statement *stmt = statement_create_empty();
    *stmt = (Statement){
        .type = STATEMENT_IF,
        .if_else = {
            .cond = cond,
            .if_block = if_block,
        },
    };
    return stmt;
}

Statement *statement_create_if_else(Expression *cond, Block *if_block, Block *else_block) {
    Statement *stmt = statement_create_empty();
    *stmt = (Statement){
        .type = STATEMENT_IF_ELSE,
        .if_else = {
            .cond = cond,
            .if_block = if_block,
            .else_block = else_block,
        },
    };
    return stmt;
}

Statement *statement_create_while(Expression *cond, Block *block) {
    Statement *stmt = statement_create_empty();
    *stmt = (Statement){
        .type = STATEMENT_WHILE,
        .while_ = {
            .cond = cond,
            .block = block,
        },
    };
    return stmt;
}

Statement *statement_create_block(Block *block) {
    Statement *stmt = statement_create_empty();
    *stmt = (Statement){
        .type = STATEMENT_BLOCK,
        .block = {
            .block = block,
        },
    };
    return stmt;
}

void statement_destroy(Statement *stmt) {
    switch (stmt->type) {
    case STATEMENT_NEW_VARIABLE:
        identifier_destroy(stmt->new_variable.ident);
        expression_destroy(stmt->new_variable.expr);
        break;
    case STATEMENT_ASSIGN:
        identifier_destroy(stmt->assign.ident);
        expression_destroy(stmt->assign.expr);
        break;
    case STATEMENT_IF:
        expression_destroy(stmt->if_.cond);
        block_destroy(stmt->if_.if_block);
        break;
    case STATEMENT_IF_ELSE:
        expression_destroy(stmt->if_else.cond);
        block_destroy(stmt->if_else.if_block);
        block_destroy(stmt->if_else.else_block);
        break;
    case STATEMENT_WHILE:
        expression_destroy(stmt->while_.cond);
        block_destroy(stmt->while_.block);
        break;
    case STATEMENT_BLOCK:
        block_destroy(stmt->block.block);
        break;
    };
    free(stmt);
}

static inline char *statement_get_type_str(Statement *statement) {
    switch (statement->type) {
    case STATEMENT_NEW_VARIABLE:
        return "new_variable";
    case STATEMENT_ASSIGN:
        return "assign";
    case STATEMENT_IF:
        return "if";
    case STATEMENT_WHILE:
        return "while";
    case STATEMENT_BLOCK:
        return "block";
    };
    return "unknown";
}

void statement_print(Statement *statement, size_t padding) {
    print_padding(padding);
    printf("Statement[type=%s]\n", statement_get_type_str(statement));

    switch (statement->type) {
    case STATEMENT_NEW_VARIABLE:
        print_padding(padding + 1);
        puts("ident:");
        identifier_print(statement->new_variable.ident, padding + 2);
        print_padding(padding + 1);
        puts("expr:");
        expression_print(statement->new_variable.expr, padding + 2);
        break;
    case STATEMENT_ASSIGN:
        print_padding(padding + 1);
        puts("ident:");
        identifier_print(statement->assign.ident, padding + 2);
        print_padding(padding + 1);
        puts("expr:");
        expression_print(statement->assign.expr, padding + 2);
        break;
    case STATEMENT_IF:
        print_padding(padding + 1);
        puts("cond:");
        expression_print(statement->if_.cond, padding + 2);
        print_padding(padding + 1);
        puts("if_block:");
        block_print(statement->if_.if_block, padding + 2);
        break;
    case STATEMENT_IF_ELSE:
        print_padding(padding + 1);
        puts("cond:");
        expression_print(statement->if_else.cond, padding + 2);
        print_padding(padding + 1);
        puts("if_block:");
        block_print(statement->if_else.if_block, padding + 2);
        print_padding(padding + 1);
        puts("else_block:");
        block_print(statement->if_else.else_block, padding + 2);
        break;
    case STATEMENT_WHILE:
        print_padding(padding + 1);
        puts("cond:");
        expression_print(statement->while_.cond, padding + 2);
        print_padding(padding + 1);
        puts("block:");
        block_print(statement->while_.block, padding + 2);
        break;
    case STATEMENT_BLOCK:
        print_padding(padding + 1);
        puts("block:");
        block_print(statement->while_.block, padding + 2);
        break;
    };
}
