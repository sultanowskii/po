#pragma once

// Forward declarations
typedef struct Identifier    Identifier;
typedef struct Expression    Expression;
typedef struct StatementList StatementList;

struct Statement;
typedef struct Statement Statement;

enum StatementType {
    STATEMENT_NEW_VARIABLE,
    STATEMENT_ASSIGN,
    STATEMENT_IF,
    STATEMENT_WHILE,
};
typedef enum StatementType StatementType;

Statement *statement_create_new_variable(Identifier *identifier, Expression *expression);
Statement *statement_create_assign(Identifier *identifier, Expression *expression);
Statement *statement_create_if(Expression *cond, StatementList *if_block);
Statement *statement_create_if_else(Expression *cond, StatementList *if_block, StatementList *else_block);
Statement *statement_create_while(Expression *cond, StatementList *block);
void       statement_destroy(Statement *stmt);
