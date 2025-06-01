#pragma once

#include <stddef.h>

// Forward declarations
typedef struct BinaryOp   BinaryOp;
typedef struct Literal    Literal;
typedef struct Identifier Identifier;

struct Expression;
typedef struct Expression Expression;

enum ExpressionType {
    EXPRESSION_BINARY_OP,
    EXPRESSION_LITERAL,
    EXPRESSION_IDENTIFIER,
};
typedef enum ExpressionType ExpressionType;

Expression *expression_create_literal(Literal *literal);
Expression *expression_create_identifier(Identifier *identifier);
Expression *expression_create_binary_op(BinaryOp *binary_op);
void        expression_print(Expression *expression, size_t padding);
void        expression_destroy(Expression *expression);
