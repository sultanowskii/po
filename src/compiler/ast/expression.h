#pragma once

#include "compiler/ast/literal.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/binary_op.h"

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
void expression_destroy(Expression *expression);
