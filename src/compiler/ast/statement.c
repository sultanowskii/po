#include "compiler/ast/statement.h"

#include <malloc.h>

#include "compiler/ast/expression.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/statement_list.h"

struct Statement {
    StatementType type;
    union {
        struct {
            Identifier *ident;
            Expression *expr;
        } new_variable;
        struct {
            Identifier *ident;
            Expression *expr;
        } assign;
        struct {
            Expression    *cond;
            StatementList *if_block;
            StatementList *else_block;
        } if_;
        struct {
            Expression    *cond;
            StatementList *block;
        } while_;
    };
};

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

Statement *statement_create_if(Expression *cond, StatementList *if_block) {
    return statement_create_if_else(cond, if_block, statement_list_create());
}

Statement *statement_create_if_else(Expression *cond, StatementList *if_block, StatementList *else_block) {
    Statement *stmt = statement_create_empty();
    *stmt = (Statement){
        .type = STATEMENT_IF,
        .if_ = {
            .cond = cond,
            .if_block = if_block,
            .else_block = else_block,
        },
    };
    return stmt;
}

Statement *statement_create_while(Expression *cond, StatementList *block) {
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
            statement_list_destroy(stmt->if_.if_block);
            statement_list_destroy(stmt->if_.else_block);
            break;
        case STATEMENT_WHILE:
            expression_destroy(stmt->while_.cond);
            statement_list_destroy(stmt->while_.block);
            break;
    };
    free(stmt);
}
