#pragma once

#include <stddef.h>

// Forward declarations
typedef struct Identifier Identifier;
typedef struct Expression Expression;
typedef struct Block      Block;

struct Statement;
typedef struct Statement Statement;

enum StatementType {
    STATEMENT_NEW_VARIABLE,
    STATEMENT_ASSIGN,
    STATEMENT_IF,
    STATEMENT_WHILE,
    STATEMENT_BLOCK,
};
typedef enum StatementType StatementType;

Statement *statement_create_new_variable(Identifier *identifier, Expression *expression);
Statement *statement_create_assign(Identifier *identifier, Expression *expression);
Statement *statement_create_if(Expression *cond, Block *if_block);
Statement *statement_create_if_else(Expression *cond, Block *if_block, Block *else_block);
Statement *statement_create_while(Expression *cond, Block *block);
Statement *statement_create_block(Block *block);
void       statement_print(Statement *statement, size_t padding);
void       statement_destroy(Statement *stmt);
